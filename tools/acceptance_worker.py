#!/usr/bin/env python3
"""Run the unchanged object checker serially with fresh per-job module state."""
import contextlib
import csv
import hashlib
import importlib
import io
import json
import os
from pathlib import Path
import select
import signal
import subprocess
import sys
import tempfile
import time
import traceback


class WorkerError(RuntimeError):
    pass


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


class FrozenInputs:
    def __init__(self, root):
        self.root = root
        self.head = self.current_head()
        self.map_path = root / 'data/ver/eu/map.csv'
        self.map_bytes = self.map_path.read_bytes()
        paths = set((root / 'tools').rglob('*.py'))
        paths.update(root / name for name in ('data/config.json', 'data/config.user.json',
                     'data/ver/eu/config.json', 'data/ver/eu/code.bin', 'data/ver/eu/exh.bin'))
        self.units = {}
        for object_path in sorted((root / 'build/eu/obj').rglob('*.o')):
            sidecar = object_path.with_suffix('.provenance.json')
            paths.update((object_path, sidecar))
            self.units[object_path.resolve()] = [object_path, sidecar]
            if not sidecar.is_file():
                continue
            paths.add(sidecar)
            try:
                record = json.loads(sidecar.read_text())
            except (ValueError, OSError):
                continue  # The unchanged provenance verifier refuses invalid records.
            dependencies = [root / name for name in record.get('inputs', {})]
            self.units[object_path.resolve()] = [object_path, sidecar, *dependencies]
            paths.update(dependencies)
        configuration = json.loads((root / 'data/config.json').read_text())
        versions = {configuration['compiler']}
        versions.update(module.get('compiler', configuration['compiler'])
                        for module in configuration['modules'].values())
        paths.update(root / 'data/compilers' / version / 'bin/armcc.exe' for version in versions)
        paths.update(root / 'data/compilers' / version / 'bin/armlink.exe' for version in versions)
        paths.add(root / 'data/compilers/wibo')
        self.hashes = {path: digest(path) for path in paths if path.is_file()}
        self.absent = {path for path in paths if not path.exists()}
        self.global_paths = {path for path in self.hashes if path.parts[:len((root / 'tools').parts)] == (root / 'tools').parts}
        self.global_paths.update(root / name for name in ('data/config.json', 'data/config.user.json',
                    'data/ver/eu/config.json', 'data/ver/eu/code.bin', 'data/ver/eu/exh.bin'))
        self.global_paths.update(path for path in self.hashes if 'compilers' in path.parts)

    def current_head(self):
        return subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=self.root, text=True).strip()

    def verify(self, paths):
        if self.current_head() != self.head:
            raise WorkerError('Frozen HEAD changed during acceptance.')
        for path in paths:
            if path in self.absent:
                if path.exists():
                    raise WorkerError('An absent frozen input appeared: ' + str(path))
            elif path not in self.hashes or not path.is_file() or digest(path) != self.hashes[path]:
                raise WorkerError('Frozen input changed: ' + str(path))
        if self.map_path.read_bytes() != self.map_bytes:
            raise WorkerError('Map changed outside the serialized checker job.')

    def before_job(self, object_path):
        self.verify(self.global_paths | set(self.units.get(object_path.resolve(), [])))

    def after_job(self, symbol, object_path):
        current = self.map_path.read_bytes()
        if current != self.map_bytes:
            before = list(csv.reader(io.StringIO(self.map_bytes.decode())))
            after = list(csv.reader(io.StringIO(current.decode())))
            if len(before) != len(after):
                raise WorkerError('Checker job changed the map row count.')
            for old, new in zip(before, after):
                if old == new:
                    continue
                if len(old) != 8 or len(new) != 8 or old[6] != symbol or new[6] != symbol:
                    raise WorkerError('Checker job changed an unrelated map row.')
                old_fields = [value.strip() for value in old]
                new_fields = [value.strip() for value in new]
                # Original updateSingle normalizes an end-valued Pool to blank.
                old_fields[1] = old_fields[1] or old_fields[2]
                new_fields[1] = new_fields[1] or new_fields[2]
                old_fields[4] = new_fields[4]
                if old_fields != new_fields or new[4] not in ('O', 'm', 'M'):
                    raise WorkerError('Checker job changed a protected map identity or rank.')
            self.map_bytes = current
        self.before_job(object_path)

    def close(self):
        self.verify(set(self.hashes) | self.absent)


def run_job(root, guard, request):
    symbol = request['symbol']
    object_path = Path(request['object'])
    arguments = ['tools/check.py', symbol, '--object', str(object_path)]
    started = time.monotonic()
    output, error = io.StringIO(), io.StringIO()
    response = {'symbol': symbol, 'object': str(object_path), 'returncode': 1,
                'argv': arguments, 'phases': {}}
    previous_arguments, previous_path = list(sys.argv), list(sys.path)
    try:
        phase = time.monotonic()
        guard.before_job(object_path)
        response['phases']['guard_before_seconds'] = time.monotonic() - phase
        sys.argv = arguments
        phase = time.monotonic()
        with contextlib.redirect_stdout(output), contextlib.redirect_stderr(error):
            for name in ('tools.low.readSymMap', 'tools.low.updateMap', 'tools.check'):
                if name in sys.modules:
                    importlib.reload(sys.modules[name])
                else:
                    importlib.import_module(name)
            checker = sys.modules['tools.check']
            response['phases']['module_setup_seconds'] = time.monotonic() - phase
            phase = time.monotonic()
            try:
                response['returncode'] = checker.main()
            except SystemExit as exception:
                response['returncode'] = exception.code if isinstance(exception.code, int) else 1
            response['phases']['unchanged_checker_main_seconds'] = time.monotonic() - phase
        phase = time.monotonic()
        guard.after_job(symbol, object_path)
        response['phases']['guard_after_seconds'] = time.monotonic() - phase
    except BaseException:
        response['returncode'] = 1
        response['worker_error'] = True
        traceback.print_exc(file=error)
    finally:
        sys.argv, sys.path = previous_arguments, previous_path
    response.update(stdout=output.getvalue(), stderr=error.getvalue(), seconds=time.monotonic() - started)
    return response


def serve():
    root = Path(__file__).resolve().parents[1]
    if Path.cwd() != root:
        raise WorkerError('Run the worker from its repository root.')
    sys.path.insert(0, str(root))
    started = time.monotonic()
    guard = FrozenInputs(root)
    print(json.dumps({'ready': True, 'head': guard.head, 'frozen_paths': len(guard.hashes),
                      'startup_seconds': time.monotonic() - started}), flush=True)
    for line in sys.stdin:
        request = json.loads(line)
        if request.get('close'):
            guard.close()
            print(json.dumps({'closed': True}), flush=True)
            return 0
        result = run_job(root, guard, request)
        print(json.dumps(result), flush=True)
        if result.get('worker_error'):
            return 1  # No later job may continue after a guard or process failure.
    raise WorkerError('Acceptance worker input ended without a guarded close.')


class AcceptanceWorker:
    """One serial transport session; never decides rank or accepts a cached result."""
    def __init__(self, root):
        self.root = Path(root)
        self.error_file = tempfile.TemporaryFile(mode='w+')
        self.process = subprocess.Popen([sys.executable, str(self.root / 'tools/acceptance_worker.py'), '--serve'],
                                        cwd=self.root, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                        stderr=self.error_file, text=True, start_new_session=os.name != 'nt')
        try:
            self.ready = self.receive()
            if not self.ready.get('ready'):
                raise WorkerError('Acceptance worker did not initialize.')
        except BaseException:
            self.terminate()
            raise

    def receive(self):
        available, _, _ = select.select([self.process.stdout], [], [], 120)
        if not available:
            self.stop_processes()
            raise WorkerError('Acceptance worker response timed out.')
        line = self.process.stdout.readline()
        if not line:
            self.error_file.seek(0)
            raise WorkerError('Acceptance worker ended unexpectedly: ' + self.error_file.read())
        return json.loads(line)

    def check(self, symbol, object_path):
        try:
            self.process.stdin.write(json.dumps({'symbol': symbol, 'object': str(object_path)}) + '\n')
            self.process.stdin.flush()
            response = self.receive()
            if response.get('symbol') != symbol or response.get('object') != str(object_path):
                raise WorkerError('Acceptance worker returned a different job identity.')
            return subprocess.CompletedProcess(response['argv'], response['returncode'],
                                               response['stdout'], response['stderr']), response
        except BaseException:
            self.terminate()
            raise

    def close(self):
        try:
            self.process.stdin.write(json.dumps({'close': True}) + '\n')
            self.process.stdin.flush()
            response = self.receive()
            if not response.get('closed') or self.process.wait(timeout=10):
                raise WorkerError('Acceptance worker failed its final frozen-input guard.')
            self.close_streams()
        except BaseException:
            self.terminate()
            raise

    def signal_process_group(self, requested_signal):
        try:
            os.killpg(self.process.pid, requested_signal)
        except ProcessLookupError:
            return
        except PermissionError:
            # macOS may report EPERM for a group containing only dead children.
            groups = subprocess.check_output(['ps', '-axo', 'pgid=,stat='], text=True)
            if any(int(fields[0]) == self.process.pid and not fields[1].startswith('Z')
                   for line in groups.splitlines() if len(fields := line.split()) == 2):
                raise

    def stop_processes(self):
        # A killed worker must not leave its original-address linker running.
        if os.name != 'nt':
            self.signal_process_group(signal.SIGTERM)
        elif self.process.poll() is None:
            self.process.terminate()
        try:
            self.process.wait(timeout=10)
        except subprocess.TimeoutExpired:
            self.process.kill()
            self.process.wait(timeout=10)
        finally:
            if os.name != 'nt':
                self.signal_process_group(signal.SIGKILL)

    def close_streams(self):
        for stream in (self.process.stdin, self.process.stdout, self.error_file):
            with contextlib.suppress(OSError):
                stream.close()

    def terminate(self):
        try:
            self.stop_processes()
        finally:
            self.close_streams()


if __name__ == '__main__':
    if sys.argv[1:] != ['--serve']:
        raise SystemExit('This module is an internal serialized acceptance transport.')
    raise SystemExit(serve())
