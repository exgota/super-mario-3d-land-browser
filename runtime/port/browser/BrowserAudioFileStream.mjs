// SPDX-License-Identifier: GPL-2.0-or-later
// Read the unchanged audio observer's append-only file. Never alter its writer.
const MaximumAudioBytes = 64 * 1024 * 1024;
const MaximumPacketBytes = 8192;
const MaximumPackets = MaximumAudioBytes / MaximumPacketBytes;
const AudioPath = '/capture/audio_pcm_s16le.bin';

function requireCondition(condition, message) {
    if (!condition) throw new Error(message);
}

async function sha256(bytes) {
    return [...new Uint8Array(await crypto.subtle.digest('SHA-256', bytes))]
        .map(value => value.toString(16).padStart(2, '0')).join('');
}

export class AudioFileStream {
    constructor(fileSystem, captureIdentifier, deliver, nativePhase) {
        requireCondition(fileSystem && typeof fileSystem.read === 'function' &&
            typeof fileSystem.analyzePath === 'function' && typeof deliver === 'function' &&
            typeof nativePhase === 'function' && typeof captureIdentifier === 'string' &&
            /^[A-Za-z0-9_-]{1,64}$/.test(captureIdentifier), 'Invalid audio observer stream');
        this.fileSystem = fileSystem;
        this.captureIdentifier = captureIdentifier;
        this.deliver = deliver;
        this.nativePhase = nativePhase;
        this.position = 0;
        this.largestExtent = 0;
        this.records = [];
        this.consumptionReports = [];
        this.enabled = false;
        this.closed = false;
    }

    enable() {
        requireCondition(!this.closed && !this.enabled, 'Audio stream already enabled or closed');
        this.enabled = true;
    }

    extent(final = false) {
        if (!this.fileSystem.analyzePath(AudioPath).exists) {
            requireCondition(!final, 'Completed audio observer file is absent');
            return 0;
        }
        const information = this.fileSystem.stat(AudioPath);
        requireCondition(this.fileSystem.isFile(information.mode) &&
            Number.isSafeInteger(information.size) && information.size >= this.largestExtent &&
            information.size <= MaximumAudioBytes, 'Audio observer file type or extent changed');
        this.largestExtent = information.size;
        requireCondition(!final || information.size % 4 === 0, 'Completed audio observer has a partial stereo pair');
        return information.size - information.size % 4;
    }

    async packet(final) {
        const extent = this.extent(final);
        requireCondition(extent >= this.position, 'Audio observer stream moved backwards');
        if (extent === this.position || (!final && extent - this.position < MaximumPacketBytes)) return false;
        requireCondition(this.records.length < MaximumPackets, 'Audio stream packet budget exceeded');
        const bytes = new Uint8Array(Math.min(MaximumPacketBytes, extent - this.position));
        const source = this.fileSystem.open(AudioPath, 'r');
        try {
            requireCondition(this.fileSystem.read(source, bytes, 0, bytes.length, this.position) === bytes.length,
                'Audio observer prefix changed while reading');
        } finally { this.fileSystem.close(source); }
        const record = {sequence: this.records.length, first_sample_frame: this.position / 4,
            sample_frames: bytes.length / 4, source_extent_bytes: extent,
            native_phase_before_delivery: this.nativePhase(), pcm_sha256: await sha256(bytes)};
        requireCondition(!this.closed, 'Audio stream stopped before delivery');
        this.offeredFrames = (this.position + bytes.length) / 4;
        await this.deliver({capture_identifier: this.captureIdentifier, sample_rate: 32728, channels: 2,
            sequence: record.sequence, first_sample_frame: record.first_sample_frame,
            sample_frames: record.sample_frames, pcm: bytes});
        requireCondition(!this.closed, 'Audio stream stopped during delivery');
        record.native_phase_after_delivery = this.nativePhase();
        this.records.push(record);
        this.position += record.sample_frames * 4;
        return true;
    }

    async pump() {
        if (!this.enabled || this.closed) return;
        if (this.pending) return this.pending;
        this.pending = this.packet(false);
        try { return await this.pending; }
        finally { this.pending = undefined; }
    }

    reportConsumption(sampleFrames) {
        requireCondition(Number.isSafeInteger(sampleFrames) && sampleFrames >= 0 &&
            sampleFrames <= (this.offeredFrames ?? this.position / 4), 'Audio consumption exceeds offered source');
        requireCondition(sampleFrames >= (this.lastConsumption ?? 0), 'Audio consumption moved backwards');
        this.lastConsumption = sampleFrames;
        const previous = this.consumptionReports.at(-1)?.sample_frames ?? 0;
        requireCondition(sampleFrames >= previous, 'Audio consumption moved backwards');
        if (sampleFrames === previous) return;
        // Retain progress only at packet-sized intervals and at the first frame.
        if (this.consumptionReports.length && sampleFrames - previous < MaximumPacketBytes / 4) return;
        requireCondition(this.consumptionReports.length < MaximumPackets + 1,
            'Audio consumption report budget exceeded');
        this.consumptionReports.push({sample_frames: sampleFrames, native_phase: this.nativePhase(),
            source_extent_bytes: this.extent(false)});
    }

    async finish() {
        if (!this.enabled) return {enabled: false, sample_frames: 0, packets: [], consumption_reports: []};
        try { await this.pending; } catch (error) { if (!this.closed) throw error; }
        if (this.closed) return this.receipt('stopped');
        while (await this.packet(true)) {}
        requireCondition(this.position === this.extent(true), 'Audio stream did not reach the completed observer');
        return this.receipt('completed');
    }

    receipt(state) {
        return {enabled: true, state, sample_rate: 32728, channels: 2, sample_frames: this.position / 4,
            payload_bytes: this.position, packets: this.records, consumption_reports: this.consumptionReports};
    }

    stop() { this.closed = true; }
}
