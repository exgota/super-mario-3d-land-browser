#!/usr/bin/env python3
"""Verify bounded queue progress after a slow periodic check, entirely in temporary fixtures."""
import hashlib
import importlib.util
import json
import os
import pathlib
import queue
import tempfile
from unittest.mock import patch

candidate = pathlib.Path(os.environ['FACTORY_UNDER_TEST'])
baseline_path = pathlib.Path(os.environ['FACTORY_BASELINE'])
result_path = pathlib.Path(os.environ.get('QUEUE_PROGRESS_RESULTS', str(pathlib.Path(__file__).with_name('queue_progress_results.json'))))

def load_module(path, name):
    specification = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(specification)
    specification.loader.exec_module(module)
    return module

def run_case(module, timeout_first=False, stop_without_proposal=False):
    with tempfile.TemporaryDirectory(prefix='integrator_queue_progress_') as directory:
        proposal = pathlib.Path(directory) / 'proposal'
        proposal.mkdir()
        (proposal / 'matched.json').write_text('{}')
        supervisor = module.Supervisor.__new__(module.Supervisor)
        supervisor.stopping = False
        supervisor.halted = ''
        supervisor.last_sync = 0
        supervisor.next_sync_retry_at = 0
        supervisor.integration_queue = queue.Queue()
        if not stop_without_proposal:
            supervisor.integration_queue.put(proposal)
        clock = [2 * module.SYNC_INTERVAL_SECONDS]
        events = []
        calls = [0]
        def synchronize():
            calls[0] += 1
            if calls[0] > 2:
                raise AssertionError('Repeated periodic checks starved the queued proposal')
            events.append('sync')
            supervisor.last_sync = clock[0]
            clock[0] += module.SYNC_INTERVAL_SECONDS + 1
            if timeout_first and calls[0] == 1:
                return False
            if stop_without_proposal:
                supervisor.stopping = True
            return True
        def integrate(proposals):
            assert proposals == [proposal]
            events.append('integrate')
            supervisor.stopping = True
            return True
        supervisor.sync_with_timeout_recovery = synchronize
        supervisor.integrate_batch = integrate
        supervisor.retire = lambda path, kind: events.append('retire')
        supervisor.land_quiet_submissions = lambda: None
        supervisor.land_facts = lambda: None
        with patch.object(module.time, 'time', lambda: clock[0]):
            supervisor.integrator_loop()
        return events

baseline = load_module(baseline_path, 'queue_progress_baseline')
fixed = load_module(candidate, 'queue_progress_candidate')
try:
    run_case(baseline)
except AssertionError as error:
    assert 'starved' in str(error)
else:
    raise AssertionError('The baseline failed to reproduce starvation')
assert run_case(fixed) == ['sync', 'integrate']
assert run_case(fixed, timeout_first=True) == ['sync', 'sync', 'integrate']
assert run_case(fixed, stop_without_proposal=True) == ['sync']
result = {'candidate_sha256': hashlib.sha256(candidate.read_bytes()).hexdigest(),
          'baseline_starvation_reproduced': True, 'slow_sync_allows_one_queue_batch': True,
          'timeout_retries_before_acceptance': True, 'completed_stop_does_not_wait_for_empty_queue': True,
          'production_paths_mutated': False, 'passed': True}
result_path.write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result, indent=2))
