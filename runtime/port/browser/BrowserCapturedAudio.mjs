// SPDX-License-Identifier: GPL-2.0-or-later
// Playback uses the completed browser capture. No reference PCM enters this module.
async function sha256(bytes) {
    return [...new Uint8Array(await crypto.subtle.digest('SHA-256', bytes))]
        .map(value => value.toString(16).padStart(2, '0')).join('');
}

export class CapturedAudioPlayback {
    constructor(metadata, pcm, changed) {
        if (metadata.sample_rate !== 32728 || metadata.channels !== 2 ||
            !Number.isSafeInteger(metadata.sample_frames) || metadata.sample_frames < 1 ||
            !(pcm instanceof Uint8Array) || pcm.byteLength !== metadata.sample_frames * 4 ||
            pcm.byteLength !== metadata.payload_bytes || pcm.byteLength > 64 * 1024 * 1024)
            throw new Error('The captured sound has an invalid extent. Run the preview again.');
        this.metadata = {...metadata};
        this.pcm = pcm;
        this.buffer = new AudioBuffer({numberOfChannels: 2, length: metadata.sample_frames, sampleRate: 32728});
        const view = new DataView(pcm.buffer, pcm.byteOffset, pcm.byteLength);
        for (let channel = 0; channel < 2; ++channel) {
            const samples = this.buffer.getChannelData(channel);
            for (let index = 0; index < samples.length; ++index)
                samples[index] = view.getInt16(index * 4 + channel * 2, true) / 32768;
        }
        this.changed = changed;
        this.events = [];
        this.disposed = false;
    }

    record(kind) {
        if (this.events.length >= 256) this.events.shift();
        this.events.push({kind, context_state: this.context?.state ?? null,
            context_time: this.context?.currentTime ?? null, context_rate: this.context?.sampleRate ?? null,
            source_rate: this.buffer.sampleRate, sample_frames: this.buffer.length});
    }

    async identity() {
        const channelHashes = [];
        for (let channel = 0; channel < 2; ++channel) {
            const samples = this.buffer.getChannelData(channel);
            const bytes = new ArrayBuffer(samples.length * 4);
            const view = new DataView(bytes);
            for (let index = 0; index < samples.length; ++index) view.setFloat32(index * 4, samples[index], true);
            channelHashes.push(await sha256(bytes));
        }
        return {...this.metadata, pcm_sha256: await sha256(this.pcm), channel_float32le_sha256: channelHashes};
    }

    async play() {
        if (this.disposed || this.source || this.starting) return;
        this.starting = true;
        try {
            this.context ??= new AudioContext();
            await this.context.resume();
            if (this.disposed) return;
            if (this.context.state !== 'running') throw new Error('Sound is paused by the browser. Press Play recorded sound again.');
            const source = this.context.createBufferSource();
            source.buffer = this.buffer;
            source.connect(this.context.destination);
            source.onended = () => {
                source.disconnect();
                if (this.source !== source) return;
                this.source = undefined;
                this.record('ended'); this.changed();
            };
            source.start();
            this.source = source;
            this.record('started'); this.changed();
        } finally { this.starting = false; }
    }

    stop() {
        if (!this.source) return;
        const source = this.source;
        this.source = undefined;
        source.onended = null;
        source.stop(); source.disconnect();
        this.record('stopped'); this.changed();
    }

    dispose() {
        this.disposed = true;
        this.stop();
        if (this.context && this.context.state !== 'closed') void this.context.close().catch(() => {});
    }
}
