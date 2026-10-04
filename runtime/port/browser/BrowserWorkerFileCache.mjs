// SPDX-License-Identifier: GPL-2.0-or-later
const PageBytes = 1024 * 1024;
const MaximumResidentBytes = 16 * PageBytes;

function requireCondition(condition, message) {
    if (!condition) throw new Error(message);
}

// Cache only the immutable Files mounted in WORKERFS. Capture files and the
// append-only audio observer retain their original filesystem operations.
export function createWorkerFileCache(runtime, paths) {
    const fileSystem = runtime.FS;
    const workerFileSystem = runtime.WORKERFS;
    requireCondition(fileSystem && workerFileSystem &&
        typeof workerFileSystem.reader?.readAsArrayBuffer === 'function' &&
        Array.isArray(paths) && paths.length > 0, 'Invalid worker file cache');
    const nodes = paths.map(path => fileSystem.lookupPath(path).node);
    requireCondition(new Set(nodes).size === nodes.length && nodes.every(node =>
        fileSystem.isFile(node.mode) && node.contents instanceof Blob &&
        node.size === node.contents.size && node.stream_ops === workerFileSystem.stream_ops),
        'The file cache needs distinct immutable WORKERFS files');
    const pages = new Map();
    const counters = {read_requests: 0, requested_bytes: 0, delivered_bytes: 0,
        physical_reads: 0, physical_bytes: 0, page_hits: 0, page_misses: 0,
        evictions: 0, resident_bytes: 0, peak_resident_bytes: 0};
    const originals = new Map();
    let disposed = false;

    function page(node, position) {
        const firstByte = Math.floor(position / PageBytes) * PageBytes;
        const key = `${node.id}:${firstByte}`;
        let bytes = pages.get(key);
        if (bytes) {
            ++counters.page_hits;
            pages.delete(key);
            pages.set(key, bytes);
            return {firstByte, bytes};
        }
        ++counters.page_misses;
        const extent = Math.min(PageBytes, node.size - firstByte);
        while (counters.resident_bytes + extent > MaximumResidentBytes) {
            const oldestKey = pages.keys().next().value;
            counters.resident_bytes -= pages.get(oldestKey).byteLength;
            pages.delete(oldestKey);
            ++counters.evictions;
        }
        bytes = new Uint8Array(workerFileSystem.reader.readAsArrayBuffer(
            node.contents.slice(firstByte, firstByte + extent)));
        requireCondition(bytes.byteLength === extent, 'Immutable worker file returned a short page');
        ++counters.physical_reads;
        counters.physical_bytes += extent;
        pages.set(key, bytes);
        counters.resident_bytes += extent;
        counters.peak_resident_bytes = Math.max(counters.peak_resident_bytes, counters.resident_bytes);
        return {firstByte, bytes};
    }

    function read(stream, buffer, offset, length, position) {
        requireCondition(!disposed, 'Worker file cache is disposed');
        ++counters.read_requests;
        counters.requested_bytes += length;
        const available = Math.max(0, Math.min(length, stream.node.size - position));
        let copied = 0;
        while (copied < available) {
            const current = position + copied;
            const {firstByte, bytes} = page(stream.node, current);
            const start = current - firstByte;
            const extent = Math.min(available - copied, bytes.byteLength - start);
            buffer.set(bytes.subarray(start, start + extent), offset + copied);
            copied += extent;
        }
        counters.delivered_bytes += copied;
        return copied;
    }

    for (const node of nodes) {
        originals.set(node, node.stream_ops);
        node.stream_ops = {...node.stream_ops, read};
    }
    return {
        observation: () => ({schema_version: 1, page_bytes: PageBytes,
            maximum_resident_bytes: MaximumResidentBytes, cached_files: nodes.length,
            disposed, ...counters}),
        dispose() {
            if (disposed) return;
            disposed = true;
            for (const [node, operations] of originals) node.stream_ops = operations;
            pages.clear();
            counters.resident_bytes = 0;
        }
    };
}
