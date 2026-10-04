// SPDX-License-Identifier: GPL-2.0-or-later
// A bounded source ring. No capture or reference samples are embedded here.
const SOURCE_RATE = 32728;
const CHANNELS = 2;
const CAPACITY_FRAMES = 65536;
const MAXIMUM_PACKET_FRAMES = 2048;
const MAXIMUM_STARTUP_BUFFER_FRAMES = CAPACITY_FRAMES - MAXIMUM_PACKET_FRAMES + 1;
const MAXIMUM_SOURCE_FRAMES = 64 * 1024 * 1024 / 4;
const MAXIMUM_GAMEPLAY_SOURCE_FRAMES = 256 * 1024 * 1024 / 4;
const MAXIMUM_OBSERVATION_FRAMES = 1048576;
const PROGRESS_INTERVAL_SECONDS = 0.1;

function nonnegativeInteger(value) {
    return Number.isSafeInteger(value) && value >= 0;
}

class OriginalStreamedAudioProcessor extends AudioWorkletProcessor {
    constructor(options) {
        super();
        const metadata = options.processorOptions;
        const maximumSourceFrames = metadata?.maximum_source_frames ?? MAXIMUM_SOURCE_FRAMES;
        const startupBufferFrames = metadata?.startup_buffer_frames ?? 0;
        if (!metadata || typeof metadata.capture_identifier !== 'string' || !metadata.capture_identifier.length ||
            metadata.sample_rate !== SOURCE_RATE || metadata.channels !== CHANNELS || sampleRate !== SOURCE_RATE ||
            !nonnegativeInteger(metadata.observation_maximum_frames) ||
            metadata.observation_maximum_frames > MAXIMUM_OBSERVATION_FRAMES ||
            !nonnegativeInteger(startupBufferFrames) || startupBufferFrames > MAXIMUM_STARTUP_BUFFER_FRAMES ||
            (maximumSourceFrames !== MAXIMUM_SOURCE_FRAMES && maximumSourceFrames !== MAXIMUM_GAMEPLAY_SOURCE_FRAMES))
            throw new Error('The streamed sound processor metadata is invalid.');
        this.captureIdentifier = metadata.capture_identifier;
        this.maximumSourceFrames = maximumSourceFrames;
        this.startupBufferFrames = startupBufferFrames;
        this.playbackStarted = startupBufferFrames === 0;
        this.storage = new Int16Array(CAPACITY_FRAMES * CHANNELS);
        this.observationMaximumFrames = metadata.observation_maximum_frames;
        this.observation = this.observationMaximumFrames ?
            new Float32Array(this.observationMaximumFrames * CHANNELS) : null;
        this.readPosition = 0;
        this.writePosition = 0;
        this.bufferedFrames = 0;
        this.acceptedSourceFrames = 0;
        this.consumedSourceFrames = 0;
        this.acceptedPacketCount = 0;
        this.highWaterMarkFrames = 0;
        this.startupSilenceFrames = 0;
        this.underrunFrames = 0;
        this.notificationSequence = 0;
        this.lastProgressTime = currentTime;
        this.producerEnded = false;
        this.completionPending = false;
        this.state = 'running';
        this.port.onmessage = event => this.receive(event.data);
        this.send('ready');
    }

    send(kind, extra, transfer) {
        this.port.postMessage({kind, capture_identifier: this.captureIdentifier,
            notification_sequence: this.notificationSequence++,
            accepted_source_frames: this.acceptedSourceFrames,
            consumed_source_frames: this.consumedSourceFrames,
            buffered_source_frames: this.bufferedFrames,
            high_water_mark_frames: this.highWaterMarkFrames,
            startup_silence_frames: this.startupSilenceFrames,
            underrun_frames: this.underrunFrames,
            silence_frames: this.startupSilenceFrames + this.underrunFrames,
            accepted_packet_count: this.acceptedPacketCount,
            state: this.state, context_time: currentTime, context_rate: sampleRate,
            ...extra}, transfer ?? []);
    }

    refuse(error) {
        this.send('rejected', {error});
    }

    receive(message) {
        if (this.state === 'stopped' || this.state === 'ended' || this.state === 'failed') return;
        if (!message || message.capture_identifier !== this.captureIdentifier) {
            this.refuse('The sound packet belongs to a different capture.');
            return;
        }
        if (message.kind === 'stop') {
            this.state = 'stopped';
            this.storage.fill(0);
            this.bufferedFrames = 0;
            this.observation = null;
            return;
        }
        if (message.kind === 'finish') {
            if (this.producerEnded || !nonnegativeInteger(message.total_sample_frames) ||
                message.total_sample_frames !== this.acceptedSourceFrames) {
                this.refuse('The final sound frame count is invalid.');
                return;
            }
            this.producerEnded = true;
            this.state = 'finishing';
            this.completionPending = this.bufferedFrames === 0;
            return;
        }
        if (message.kind !== 'append' || this.producerEnded ||
            message.sample_rate !== SOURCE_RATE || message.channels !== CHANNELS ||
            !nonnegativeInteger(message.sequence) || message.sequence !== this.acceptedPacketCount ||
            !nonnegativeInteger(message.first_sample_frame) || message.first_sample_frame !== this.acceptedSourceFrames ||
            !Number.isSafeInteger(message.sample_frames) || message.sample_frames < 1 ||
            message.sample_frames > MAXIMUM_PACKET_FRAMES || !(message.pcm instanceof ArrayBuffer) ||
            message.pcm.byteLength !== message.sample_frames * CHANNELS * 2 ||
            this.acceptedSourceFrames + message.sample_frames > this.maximumSourceFrames ||
            this.bufferedFrames + message.sample_frames > CAPACITY_FRAMES ||
            (this.observationMaximumFrames &&
                this.acceptedSourceFrames + message.sample_frames > this.observationMaximumFrames)) {
            this.refuse('The sound packet has invalid order, format, extent or available capacity.');
            return;
        }
        const view = new DataView(message.pcm);
        for (let frame = 0; frame < message.sample_frames; ++frame) {
            const position = this.writePosition * CHANNELS;
            this.storage[position] = view.getInt16(frame * 4, true);
            this.storage[position + 1] = view.getInt16(frame * 4 + 2, true);
            this.writePosition = (this.writePosition + 1) % CAPACITY_FRAMES;
        }
        this.bufferedFrames += message.sample_frames;
        this.acceptedSourceFrames += message.sample_frames;
        ++this.acceptedPacketCount;
        this.highWaterMarkFrames = Math.max(this.highWaterMarkFrames, this.bufferedFrames);
        this.send('accepted', {sequence: message.sequence, first_sample_frame: message.first_sample_frame,
            sample_frames: message.sample_frames});
    }

    process(inputs, outputs) {
        const output = outputs[0];
        if (output) for (let channel = 0; channel < output.length; ++channel) output[channel].fill(0);
        if (this.state === 'stopped' || this.state === 'ended' || this.state === 'failed') return false;
        if (!output || output.length !== CHANNELS || output[0].length !== output[1].length) {
            this.state = 'failed';
            this.send('error', {error: 'The sound output does not contain two equal channels.'});
            return false;
        }
        if (this.completionPending) {
            this.state = 'ended';
            if (this.observation) {
                const observation = this.observation.buffer;
                this.observation = null;
                this.send('drained', {observation, observation_frames: this.consumedSourceFrames}, [observation]);
            } else {
                this.send('drained', {observation_frames: 0});
            }
            return false;
        }
        const left = output[0];
        const right = output[1];
        const outputFrames = left.length;
        // Wait only before the first source frame. Finish releases a short stream;
        // later starvation remains an underrun, without dropping or stretching PCM.
        if (!this.playbackStarted && (this.bufferedFrames >= this.startupBufferFrames || this.producerEnded))
            this.playbackStarted = true;
        const sourceFrames = this.playbackStarted ? Math.min(outputFrames, this.bufferedFrames) : 0;
        const firstConsumedFrame = this.consumedSourceFrames;
        for (let frame = 0; frame < sourceFrames; ++frame) {
            const position = this.readPosition * CHANNELS;
            left[frame] = this.storage[position] / 32768;
            right[frame] = this.storage[position + 1] / 32768;
            if (this.observation) {
                const observationPosition = (firstConsumedFrame + frame) * CHANNELS;
                // Copy from the actual output arrays, including legitimate source zeros.
                this.observation[observationPosition] = left[frame];
                this.observation[observationPosition + 1] = right[frame];
            }
            this.readPosition = (this.readPosition + 1) % CAPACITY_FRAMES;
        }
        this.bufferedFrames -= sourceFrames;
        this.consumedSourceFrames += sourceFrames;
        if (!this.producerEnded) {
            const unavailableFrames = outputFrames - sourceFrames;
            if (this.consumedSourceFrames === 0) this.startupSilenceFrames += unavailableFrames;
            else this.underrunFrames += unavailableFrames;
        }
        // Let the final source quantum return before announcing drain in the next call.
        if (this.producerEnded && this.bufferedFrames === 0) this.completionPending = true;
        if (currentTime - this.lastProgressTime >= PROGRESS_INTERVAL_SECONDS) {
            this.lastProgressTime = currentTime;
            this.send('progress');
        }
        return true;
    }
}

registerProcessor('original-streamed-audio', OriginalStreamedAudioProcessor);
