// SPDX-License-Identifier: GPL-2.0-or-later
function requireCondition(condition, message) {
    if (!condition) throw new Error(message);
}

// Cancellation prevents runtime admission. The owning page remains responsible
// for disposing its worker, including the generated runtime's pthread workers.
export function beginRuntimeInitialization(moduleUrl, configuration) {
    requireCondition(configuration?.noInitialRun === true && Array.isArray(configuration.preRun),
        'Runtime initialization requires explicit admission');
    const started = performance.now();
    const checkpoints = [{name: 'module_import_started', elapsed_milliseconds: 0}];
    let state = 'importing';
    let cancellation;
    let rejectCancellation;
    const cancelled = new Promise((resolve, reject) => { rejectCancellation = reject; });

    function checkpoint(name) {
        requireCondition(typeof name === 'string' && checkpoints.length < 16 &&
            !checkpoints.some(item => item.name === name), 'Invalid initialization checkpoint');
        checkpoints.push({name, elapsed_milliseconds: performance.now() - started});
    }

    function requireActive() {
        if (cancellation) throw cancellation;
    }

    const completed = (async () => {
        try {
            const factory = (await import(moduleUrl)).default;
            requireActive();
            requireCondition(typeof factory === 'function', 'Generated ES module has no default factory');
            checkpoint('module_import_completed');
            state = 'initializing';
            const runtime = await factory({...configuration, preRun: configuration.preRun.map(callback =>
                instance => { requireActive(); return callback(instance); })});
            requireActive();
            checkpoint('runtime_initialized');
            state = 'ready';
            return runtime;
        } catch (error) {
            if (!cancellation) state = 'failed';
            throw error;
        }
    })();

    return {
        ready: Promise.race([completed, cancelled]),
        checkpoint,
        observation: () => ({schema_version: 1, state, clock: 'worker_performance_now',
            checkpoints: checkpoints.map(item => ({...item}))}),
        cancel(reason = new DOMException('Runtime initialization was cancelled', 'AbortError')) {
            if (cancellation || state === 'ready' || state === 'failed') return false;
            cancellation = reason instanceof Error ? reason : new Error(String(reason));
            state = 'cancelled';
            checkpoint('initialization_cancelled');
            rejectCancellation(cancellation);
            return true;
        }
    };
}
