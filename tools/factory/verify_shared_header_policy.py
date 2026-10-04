"""Exercise header-edit collection and integrator rejection using an isolated git repository."""
import importlib.util
import json
import pathlib
import sqlite3
import subprocess
import tempfile
import threading

factory_path = pathlib.Path(__file__).resolve().parent / 'factory.py'
spec = importlib.util.spec_from_file_location('candidate', factory_path)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
results = {}
with tempfile.TemporaryDirectory(prefix='shared-header-verification-') as directory:
    root = pathlib.Path(directory)
    def git(*args):
        return subprocess.check_output(['git', '-C', str(root), *args], text=True).strip()
    def write(path, text):
        destination = root / path
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(text)
    git('init', '-q', '-b', 'main')
    git('config', 'user.email', 'verification@localhost')
    git('config', 'user.name', 'Verification')
    header = 'lib/example/include/SharedRecord.h'
    implementation = 'Game/backup/src/Factory/fn_00100000.cpp'
    write(header, 'namespace example { struct SharedRecord { int value; }; }\n')
    write('Game/backup/src/Existing.cpp', 'namespace { struct SharedRecord { int legacy; }; }\n')
    write('data/ver/eu/map.csv', 'Start,Pool,End,Section,Rank,Type,Symbol,SectionName\n0x00100000,,0x00100004,,U,f,fn_00100000,\n')
    git('add', '.'); git('commit', '-qm', 'Isolated fixture')
    base = git('rev-parse', 'HEAD')
    duplicate = 'namespace { struct SharedRecord { int value; }; }\nextern "C" void fn_00100000() {}\n'
    assert module.shared_type_violations(root, base, {implementation: duplicate})
    assert not module.shared_type_violations(root, base, {implementation: '#include <SharedRecord.h>\nexample::SharedRecord* record;\n'})
    assert not module.shared_type_violations(root, base, {implementation: '// struct SharedRecord { };\nconst char* x="struct SharedRecord { }";\nstruct SharedRecord;\n'})
    assert not module.shared_type_violations(root, base, {'Game/backup/src/Existing.cpp': 'namespace { struct SharedRecord { int legacy; }; }\n'})
    assert module.shared_type_violations(root, base, {'lib/example/include/NewRecord.h':'struct NewRecord { int value; };', implementation:'struct NewRecord { int value; };'})
    results['duplicate_rejected_shared_use_and_forward_allowed_legacy_preserved'] = True
    write(header, 'namespace example { struct SharedRecord { int value; int extension; }; }\n')
    write('lib/example/include/NewRecord.hpp', 'struct NewRecord { int value; };\n')
    write(implementation, '#include <SharedRecord.h>\nextern "C" void fn_00100000() {}\n')
    assert set(module.changed_shared_headers(root, base)) == {header, 'lib/example/include/NewRecord.hpp'}
    assert not module.changed_tracked_files(root, base)
    module.commit_attempt(root)
    assert git('show', 'HEAD:lib/example/include/NewRecord.hpp').startswith('struct NewRecord')
    assert 'extension' in git('show', 'HEAD:'+header)
    results['new_and_extended_headers_committed_for_provenance'] = True
    # A submitted batch bypassing worker feedback must still be rejected before build or ref movement.
    git('checkout', '-q', '--detach', base); git('update-ref', 'refs/heads/main', base)
    proposal = root / 'proposal'
    (proposal / implementation).parent.mkdir(parents=True)
    (proposal / implementation).write_text(duplicate)
    (proposal / 'matched.json').write_text(json.dumps({'addresses':[0x100000],'symbols':{str(0x100000):'fn_00100000'}}))
    module.INTEGRATION = root; module.REPOSITORY = root
    supervisor = module.Supervisor.__new__(module.Supervisor)
    supervisor.integration_lock = threading.Lock()
    supervisor.database = sqlite3.connect(':memory:')
    supervisor.database.execute('CREATE TABLE events(time REAL,kind TEXT,text TEXT)')
    supervisor.target_moved_externally = lambda: False
    supervisor.restore_integration = lambda: None
    retired = []
    supervisor.retire = lambda path, reason: retired.append((path, reason))
    module.tool = lambda *args, **kwargs: (_ for _ in ()).throw(AssertionError('Rejected proposal reached build'))
    supervisor.integrate_batch([proposal])
    assert retired == [(proposal, 'rejected')]
    assert git('rev-parse', 'main') == base
    results['batch_bypass_rejected_before_build_ref_unchanged'] = True
    # Source submissions use the same gate, independently of the worker proposal format.
    git('checkout', '-qb', 'integrator/rejected-type', base)
    write('Game/backup/src/NewSource.cpp', duplicate)
    git('add', 'Game/backup/src/NewSource.cpp'); git('commit', '-qm', 'Candidate duplicate')
    commit = git('rev-parse', 'HEAD'); git('checkout', '-q', '--detach', base)
    module.target_commit = lambda: base
    try:
        supervisor.merge_submission({'branch':'integrator/rejected-type','commit':commit}, 'test')
        raise AssertionError('Submission duplicate was accepted')
    except module.SubmissionDecision as decision:
        assert decision.result['outcome'] == 'rejected' and 'shared type policy' in decision.result['reason']
    assert git('rev-parse', 'main') == base
    results['submission_bypass_rejected_before_merge'] = True
print(json.dumps(results, indent=2))
