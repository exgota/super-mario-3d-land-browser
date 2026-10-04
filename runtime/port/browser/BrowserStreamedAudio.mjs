// SPDX-License-Identifier: GPL-2.0-or-later
// Original runtime PCM is the only source of streamed sound.
const SOURCE_RATE = 32728;
const CHANNELS = 2;
const CAPACITY_FRAMES = 65536;
const MAXIMUM_PACKET_FRAMES = 2048;
// Below the threshold, every legal packet must still fit without consumption.
const MAXIMUM_STARTUP_BUFFER_FRAMES = CAPACITY_FRAMES - MAXIMUM_PACKET_FRAMES + 1;
const MAXIMUM_SOURCE_FRAMES = 64 * 1024 * 1024 / 4;
const MAXIMUM_GAMEPLAY_SOURCE_FRAMES = 256 * 1024 * 1024 / 4;
const MAXIMUM_OBSERVATION_FRAMES = 1048576;
const DEADLINE_MILLISECONDS = 30000;
const TERMINAL_STATES = new Set(['ended', 'stopped', 'disposed', 'failed']);

function exactFields(value, fields) {
    return value !== null && typeof value === 'object' &&
        Reflect.ownKeys(value).length === fields.length &&
        fields.every(field => Object.prototype.hasOwnProperty.call(value, field));
}

function nonnegativeInteger(value) {
    return Number.isSafeInteger(value) && value >= 0;
}

function deferred() {
    const operation = {settled: false};
    operation.promise = new Promise((resolve, reject) => {
        operation.resolve = resolve;
        operation.reject = reject;
    });
    return operation;
}

export class StreamedAudioPlayback {
    constructor(metadata, changed, maximumSourceFrames = MAXIMUM_SOURCE_FRAMES) {
        if (!exactFields(metadata, ['capture_identifier', 'sample_rate', 'channels']) ||
            typeof metadata.capture_identifier !== 'string' || !metadata.capture_identifier.length ||
            metadata.sample_rate !== SOURCE_RATE || metadata.channels !== CHANNELS)
            throw new Error('The streamed sound has invalid capture metadata.');
        if (changed !== undefined && typeof changed !== 'function')
            throw new Error('The sound change callback must be a function.');
        if (maximumSourceFrames !== MAXIMUM_SOURCE_FRAMES && maximumSourceFrames !== MAXIMUM_GAMEPLAY_SOURCE_FRAMES)
            throw new Error('The streamed sound source limit is invalid.');
        this.maximumSourceFrames = maximumSourceFrames;
        this.metadata = Object.freeze({...metadata});
        this.changed = changed;
        this.state = 'idle';
        this.context = null;
        this.node = null;
        this.events = [];
        this.disposed = false;
        this.outputObservation = null;
        this.observationMaximumFrames = 0;
        this.startupBufferFrames = 0;
        this.acceptedSourceFrames = 0;
        this.acceptedPacketCount = 0;
        this.lifecycle = 0;
        this.lastNotification = -1;
        this.workletStatistics = {accepted_source_frames: 0, consumed_source_frames: 0,
            buffered_source_frames: 0, high_water_mark_frames: 0, startup_silence_frames: 0,
            underrun_frames: 0, silence_frames: 0, accepted_packet_count: 0,
            state: 'idle', context_time: 0, context_rate: null};
        this.record('created');
    }

    enableObservation(maximumFrames) {
        if (this.state !== 'idle' || this.context)
            throw new Error('Output observation must be enabled before starting sound.');
        if (!Number.isSafeInteger(maximumFrames) || maximumFrames < 1 ||
            maximumFrames > MAXIMUM_OBSERVATION_FRAMES)
            throw new Error('The output observation capacity is invalid.');
        this.observationMaximumFrames = maximumFrames;
    }

    enableStartupBuffer(minimumFrames) {
        if (this.state !== 'idle' || this.context)
            throw new Error('Startup buffering must be enabled before starting sound.');
        if (!Number.isSafeInteger(minimumFrames) || minimumFrames < 1 || minimumFrames > MAXIMUM_STARTUP_BUFFER_FRAMES)
            throw new Error('The startup sound buffer extent is invalid.');
        this.startupBufferFrames = minimumFrames;
    }

    statistics() {
        return {...this.workletStatistics, capture_identifier: this.metadata.capture_identifier,
            state: this.state, worklet_state: this.workletStatistics.state,
            accepted_source_frames: this.acceptedSourceFrames,
            queued_source_frames: this.acceptedSourceFrames,
            accepted_packet_count: this.acceptedPacketCount,
            buffered_source_frames: TERMINAL_STATES.has(this.state) ? 0 : this.workletStatistics.buffered_source_frames,
            last_observed_buffered_source_frames: this.workletStatistics.buffered_source_frames,
            capacity_frames: CAPACITY_FRAMES, maximum_source_frames: this.maximumSourceFrames,
            source_rate: SOURCE_RATE, channels: CHANNELS,
            context_state: this.context?.state ?? null,
            context_time: this.context?.currentTime ?? null,
            context_rate: this.context?.sampleRate ?? null,
            pending_append: Boolean(this.pendingAppend),
            ...(this.startupBufferFrames ? {startup_buffer_frames: this.startupBufferFrames} : {}),
            observation_maximum_frames: this.observationMaximumFrames};
    }

    record(kind, detail) {
        if (this.events.length === 256) this.events.shift();
        this.events.push({kind, ...this.statistics(), ...(detail === undefined ? {} : {detail})});
    }

    notify() {
        // A page callback cannot change acceptance or interrupt owned graph cleanup.
        try { this.changed?.(this.statistics()); } catch {}
    }

    operation(milliseconds, message) {
        const operation = deferred();
        operation.timer = setTimeout(() => this.fail(new Error(message)), milliseconds);
        return operation;
    }

    settle(operation, error, value) {
        if (!operation || operation.settled) return;
        operation.settled = true;
        clearTimeout(operation.timer);
        if (error) operation.reject(error);
        else operation.resolve(value);
    }

    start() {
        if (TERMINAL_STATES.has(this.state))
            return Promise.reject(new Error('This sound stream has ended. Run the preview again.'));
        if (this.pendingStart) return this.pendingStart.promise;
        if (this.state === 'finishing')
            return Promise.reject(new Error('This sound stream is draining.'));
        if (this.state === 'running' && this.context.state === 'running')
            return Promise.resolve(this.statistics());
        const operation = this.operation(DEADLINE_MILLISECONDS, 'Starting streamed sound timed out.');
        this.pendingStart = operation;
        this.state = 'starting';
        const lifecycle = this.lifecycle;
        try {
            this.context ??= new AudioContext({sampleRate: SOURCE_RATE});
            // resume() is called in the gesture's synchronous stack, before any await.
            const resumed = this.context.resume();
            this.context.onstatechange = () => {
                if (lifecycle !== this.lifecycle || TERMINAL_STATES.has(this.state)) return;
                if (this.context.state === 'closed') {
                    this.fail(new Error('The sound context closed before the stream drained.'));
                    return;
                }
                this.record('context-state-changed');
                this.notify();
            };
            void this.initialize(resumed, lifecycle).catch(error => {
                if (lifecycle === this.lifecycle) this.fail(error);
            });
            this.record('starting');
            this.notify();
        } catch (error) { this.fail(error); }
        return operation.promise;
    }

    async initialize(resumed, lifecycle) {
        await resumed;
        if (lifecycle !== this.lifecycle) return;
        if (this.context.sampleRate !== SOURCE_RATE)
            throw new Error('The browser did not provide the original sound sample rate.');
        if (this.context.state !== 'running')
            throw new Error('Sound is paused by the browser. Start sound from a user gesture.');
        if (this.node) {
            this.state = 'running';
            const operation = this.pendingStart;
            this.pendingStart = null;
            this.record('resumed');
            this.settle(operation, null, this.statistics());
            this.notify();
            return;
        }
        await this.context.audioWorklet.addModule(new URL('./BrowserAudioWorklet.mjs', import.meta.url));
        if (lifecycle !== this.lifecycle) return;
        this.node = new AudioWorkletNode(this.context, 'original-streamed-audio', {
            numberOfInputs: 0, numberOfOutputs: 1, outputChannelCount: [CHANNELS],
            channelCount: CHANNELS, channelCountMode: 'explicit', channelInterpretation: 'discrete',
            processorOptions: {...this.metadata, observation_maximum_frames: this.observationMaximumFrames,
                ...(this.startupBufferFrames ? {startup_buffer_frames: this.startupBufferFrames} : {}),
                ...(this.maximumSourceFrames !== MAXIMUM_SOURCE_FRAMES ?
                    {maximum_source_frames:this.maximumSourceFrames} : {})}
        });
        this.node.port.onmessage = event => {
            if (lifecycle !== this.lifecycle || TERMINAL_STATES.has(this.state)) return;
            try { this.receive(event.data); } catch (error) { this.fail(error); }
        };
        this.node.port.onmessageerror = () => {
            if (lifecycle === this.lifecycle) this.fail(new Error('The sound worklet message could not be read.'));
        };
        this.node.onprocessorerror = () => {
            if (lifecycle === this.lifecycle) this.fail(new Error('The sound worklet stopped unexpectedly.'));
        };
        this.node.connect(this.context.destination);
    }

    async append(packet) {
        if (!exactFields(packet, ['capture_identifier', 'sequence', 'first_sample_frame',
            'sample_frames', 'sample_rate', 'channels', 'pcm']) ||
            packet.capture_identifier !== this.metadata.capture_identifier ||
            packet.sample_rate !== SOURCE_RATE || packet.channels !== CHANNELS ||
            !nonnegativeInteger(packet.sequence) || packet.sequence !== this.acceptedPacketCount ||
            !nonnegativeInteger(packet.first_sample_frame) || packet.first_sample_frame !== this.acceptedSourceFrames ||
            !Number.isSafeInteger(packet.sample_frames) || packet.sample_frames < 1 ||
            packet.sample_frames > MAXIMUM_PACKET_FRAMES || !(packet.pcm instanceof Uint8Array) ||
            packet.pcm.byteLength !== packet.sample_frames * CHANNELS * 2 ||
            this.acceptedSourceFrames + packet.sample_frames > this.maximumSourceFrames)
            throw new Error('The streamed sound packet has invalid identity, order, format or extent.');
        if (this.state !== 'running') throw new Error('Start sound before appending source frames.');
        if (this.pendingAppend) throw new Error('Only one streamed sound append may be pending.');
        if (this.observationMaximumFrames &&
            this.acceptedSourceFrames + packet.sample_frames > this.observationMaximumFrames)
            throw new Error('The source exceeds the enabled output observation capacity.');
        // Copy now. A caller retains its buffer, and changes during backpressure cannot alter playback.
        const payload = new Uint8Array(packet.pcm);
        const operation = this.operation(DEADLINE_MILLISECONDS, 'Waiting for streamed sound capacity timed out.');
        Object.assign(operation, {sequence: packet.sequence, firstSampleFrame: packet.first_sample_frame,
            sampleFrames: packet.sample_frames, payload, posted: false});
        this.pendingAppend = operation;
        const lifecycle = this.lifecycle;
        void this.submitAppend(operation, lifecycle).catch(error => {
            if (lifecycle === this.lifecycle) this.fail(error);
        });
        return await operation.promise;
    }

    async submitAppend(operation, lifecycle) {
        while (CAPACITY_FRAMES - this.workletStatistics.buffered_source_frames < operation.sampleFrames) {
            await new Promise(resolve => { this.capacityResolver = resolve; });
            if (lifecycle !== this.lifecycle || this.pendingAppend !== operation) return;
        }
        if (lifecycle !== this.lifecycle || this.pendingAppend !== operation) return;
        operation.posted = true;
        this.node.port.postMessage({kind: 'append', ...this.metadata,
            sequence: operation.sequence, first_sample_frame: operation.firstSampleFrame,
            sample_frames: operation.sampleFrames, pcm: operation.payload.buffer}, [operation.payload.buffer]);
        operation.payload = null;
    }

    finish(totalSampleFrames) {
        if (!nonnegativeInteger(totalSampleFrames) || totalSampleFrames !== this.acceptedSourceFrames)
            return Promise.reject(new Error('The final sound frame count does not equal accepted source frames.'));
        if (this.pendingFinish) return this.pendingFinish.promise;
        if (this.state === 'ended') return Promise.resolve(this.receipt);
        if (this.state !== 'running' || this.pendingAppend)
            return Promise.reject(new Error('Finish sound after starting it and awaiting every append.'));
        const duration = this.workletStatistics.buffered_source_frames / SOURCE_RATE * 1000;
        const operation = this.operation(duration + DEADLINE_MILLISECONDS, 'Draining streamed sound timed out.');
        this.pendingFinish = operation;
        this.state = 'finishing';
        this.record('finishing');
        try {
            this.node.port.postMessage({kind: 'finish', capture_identifier: this.metadata.capture_identifier,
                total_sample_frames: totalSampleFrames});
        } catch (error) { this.fail(error); }
        this.notify();
        return operation.promise;
    }

    receive(message) {
        if (!message || message.capture_identifier !== this.metadata.capture_identifier ||
            !nonnegativeInteger(message.notification_sequence) ||
            message.notification_sequence !== this.lastNotification + 1)
            throw new Error('The sound worklet returned an invalid generation or notification sequence.');
        const previous = this.workletStatistics;
        const counters = ['accepted_source_frames', 'consumed_source_frames', 'high_water_mark_frames',
            'startup_silence_frames', 'underrun_frames', 'silence_frames', 'accepted_packet_count'];
        if (counters.some(name => !nonnegativeInteger(message[name]) || message[name] < previous[name]) ||
            !nonnegativeInteger(message.buffered_source_frames) ||
            message.accepted_source_frames > this.maximumSourceFrames ||
            message.consumed_source_frames > message.accepted_source_frames ||
            message.buffered_source_frames !== message.accepted_source_frames - message.consumed_source_frames ||
            message.buffered_source_frames > CAPACITY_FRAMES ||
            message.high_water_mark_frames < message.buffered_source_frames ||
            message.high_water_mark_frames > CAPACITY_FRAMES ||
            message.silence_frames !== message.startup_silence_frames + message.underrun_frames ||
            message.context_rate !== SOURCE_RATE || !Number.isFinite(message.context_time) ||
            message.context_time < previous.context_time)
            throw new Error('The sound worklet returned inconsistent frame statistics.');
        if (message.kind === 'accepted') {
            const operation = this.pendingAppend;
            if (!operation?.posted || message.sequence !== operation.sequence ||
                message.first_sample_frame !== operation.firstSampleFrame ||
                message.sample_frames !== operation.sampleFrames ||
                message.accepted_source_frames !== this.acceptedSourceFrames + operation.sampleFrames ||
                message.accepted_packet_count !== this.acceptedPacketCount + 1)
                throw new Error('The sound worklet returned an invalid packet acknowledgement.');
            this.acceptedSourceFrames = message.accepted_source_frames;
            this.acceptedPacketCount = message.accepted_packet_count;
        } else if (message.accepted_source_frames !== this.acceptedSourceFrames ||
            message.accepted_packet_count !== this.acceptedPacketCount)
            throw new Error('The sound worklet changed its source count without accepting a packet.');
        this.lastNotification = message.notification_sequence;
        this.workletStatistics = Object.fromEntries([...counters, 'buffered_source_frames',
            'state', 'context_time', 'context_rate'].map(name => [name, message[name]]));
        if (message.kind === 'ready') {
            if (!this.pendingStart || this.state !== 'starting')
                throw new Error('The sound worklet returned an unexpected ready message.');
            this.state = 'running';
            const operation = this.pendingStart;
            this.pendingStart = null;
            this.record('started');
            this.settle(operation, null, this.statistics());
        } else if (message.kind === 'accepted') {
            const operation = this.pendingAppend;
            this.pendingAppend = null;
            this.record('accepted');
            this.settle(operation, null, this.statistics());
        } else if (message.kind === 'drained') {
            this.receiveObservation(message);
            const lifecycle = this.lifecycle;
            void this.complete(lifecycle).catch(error => {
                if (lifecycle === this.lifecycle) this.fail(error);
            });
        } else if (message.kind === 'rejected' || message.kind === 'error') {
            throw new Error(message.error || 'The sound worklet refused the source.');
        } else if (message.kind !== 'progress') {
            throw new Error('The sound worklet returned an unknown message.');
        }
        const resolveCapacity = this.capacityResolver;
        this.capacityResolver = null;
        resolveCapacity?.();
        this.notify();
    }

    receiveObservation(message) {
        if (this.state !== 'finishing' || !this.pendingFinish ||
            message.buffered_source_frames !== 0 || message.consumed_source_frames !== this.acceptedSourceFrames ||
            message.state !== 'ended')
            throw new Error('The sound worklet ended before consuming all accepted source frames.');
        if (this.observationMaximumFrames) {
            if (!(message.observation instanceof ArrayBuffer) ||
                message.observation.byteLength !== this.observationMaximumFrames * CHANNELS * 4 ||
                message.observation_frames !== this.acceptedSourceFrames)
                throw new Error('The actual sound output observation has an invalid extent.');
            this.outputObservation = Object.freeze({...this.metadata,
                sample_frames: message.observation_frames,
                interleaved_samples: new Float32Array(message.observation, 0, message.observation_frames * CHANNELS)});
        } else if (message.observation !== undefined || message.observation_frames !== 0) {
            throw new Error('The sound worklet returned an unrequested output observation.');
        }
    }

    async complete(lifecycle) {
        await this.closeGraph();
        if (lifecycle !== this.lifecycle) return;
        this.state = 'ended';
        const operation = this.pendingFinish;
        this.pendingFinish = null;
        this.receipt = Object.freeze({...this.statistics(), total_sample_frames: this.acceptedSourceFrames,
            output_observation_frames: this.outputObservation?.sample_frames ?? 0});
        this.record('ended');
        this.settle(operation, null, this.receipt);
        this.notify();
    }

    closeGraph() {
        if (this.cleanupPromise) return this.cleanupPromise;
        const node = this.node;
        this.node = null;
        if (node) {
            node.port.onmessage = null;
            node.port.onmessageerror = null;
            node.onprocessorerror = null;
            try { node.disconnect(); } catch {}
            node.port.close();
        }
        if (this.context) this.context.onstatechange = null;
        try {
            this.cleanupPromise = this.context && this.context.state !== 'closed' ?
                this.context.close() : Promise.resolve();
        } catch (error) { this.cleanupPromise = Promise.reject(error); }
        return this.cleanupPromise;
    }

    terminate(state, error) {
        if (TERMINAL_STATES.has(this.state)) return this.cleanupPromise ?? Promise.resolve();
        this.state = state;
        ++this.lifecycle;
        this.settle(this.pendingStart, error);
        this.settle(this.pendingAppend, error);
        this.settle(this.pendingFinish, error);
        if (this.pendingAppend) this.pendingAppend.payload = null;
        this.pendingStart = this.pendingAppend = this.pendingFinish = null;
        const resolveCapacity = this.capacityResolver;
        this.capacityResolver = null;
        resolveCapacity?.();
        if (this.node) {
            try { this.node.port.postMessage({kind: 'stop', capture_identifier: this.metadata.capture_identifier}); } catch {}
        }
        const cleanup = this.closeGraph();
        this.record(state, error.message);
        this.notify();
        return cleanup;
    }

    fail(error) {
        const failure = error instanceof Error ? error : new Error(String(error));
        this.error = failure.message;
        void this.terminate('failed', failure).catch(() => {});
    }

    stop() {
        return this.terminate('stopped', new Error('The streamed sound was stopped.'));
    }

    dispose() {
        this.disposed = true;
        return this.terminate('disposed', new Error('The streamed sound was disposed.'));
    }
}
