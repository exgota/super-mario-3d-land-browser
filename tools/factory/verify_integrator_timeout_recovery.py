#!/usr/bin/env python3
"""Verify timeout recovery, process cleanup and ref preservation in temporary fixtures."""
import datetime,hashlib,importlib.util,json,os,pathlib,queue,sqlite3,subprocess,sys,tempfile,threading,time
from unittest.mock import patch
candidate=pathlib.Path(os.environ['FACTORY_UNDER_TEST'])
spec=importlib.util.spec_from_file_location('factory_timeout_verification',candidate);factory=importlib.util.module_from_spec(spec);spec.loader.exec_module(factory)
results={}
with tempfile.TemporaryDirectory(prefix='integrator_timeout_verification_') as temporary:
 root=pathlib.Path(temporary);repository=root/'repository';repository.mkdir();logs=root/'logs';logs.mkdir()
 factory.REPOSITORY=repository;factory.INTEGRATION=repository;factory.LOGS=logs;factory.PROPOSALS=root/'proposals';factory.PROPOSALS.mkdir();factory.SUBMISSIONS=root/'submissions';factory.SUBMISSIONS.mkdir();factory.SPLIT_DISABLED_FILE=root/'SPLIT_DISABLED';factory.VENV_PYTHON=pathlib.Path(sys.executable)
 def git(*args):return subprocess.check_output(['git','-C',str(repository),*args],text=True).strip()
 git('init','-q','-b','main');git('config','user.name','Timeout Verification');git('config','user.email','verification@localhost')
 (repository/'Game').mkdir();(repository/'Game/candidate.cpp').write_text('int fixture() { return 1; }\n');(repository/'data/ver/eu').mkdir(parents=True);(repository/'data/ver/eu/map.csv').write_text('fixture map\n');git('add','.');git('commit','-qm','Fixture base');base=git('rev-parse','HEAD');git('switch','-qc',factory.CANDIDATE_BRANCH)
 database=sqlite3.connect(':memory:',check_same_thread=False);database.row_factory=sqlite3.Row;database.execute('create table events(time real,kind text,text text)')
 supervisor=factory.Supervisor.__new__(factory.Supervisor);supervisor.database=database;supervisor.integration_lock=threading.Lock();supervisor.integration_queue=queue.Queue();supervisor.last_sync=123.0;supervisor.last_completed_sync=120.0;supervisor.next_sync_retry_at=0;supervisor.halted='';supervisor.stopping=False
 proposal=factory.PROPOSALS/'job_1_run_1';proposal.mkdir();(proposal/'matched.json').write_text('{"preserved":true}\n');submission=factory.SUBMISSIONS/'pending.json';submission.write_text('{"claims":["fixture"]}\n');before={str(p):p.read_bytes() for p in (proposal/'matched.json',submission)}
 def timed_out_sync():
  supervisor.last_sync=time.time();(repository/'Game/candidate.cpp').write_text('int fixture() { return 2; }\n');git('add','Game/candidate.cpp');git('commit','-qm','Unchecked candidate');(repository/'data/ver/eu/map.csv').write_text('unverified scratch rank\n');raise subprocess.TimeoutExpired(['tools/check.py','-q','-w'],1800)
 supervisor.sync=timed_out_sync
 assert supervisor.sync_with_timeout_recovery() is False
 assert git('rev-parse','main')==base and git('rev-parse','HEAD')==base and not git('status','--porcelain')
 assert supervisor.last_sync==123.0 and supervisor.last_completed_sync==120.0 and supervisor.next_sync_retry_at>time.time()
 assert all(pathlib.Path(p).read_bytes()==b for p,b in before.items())
 assert database.execute("select count(*) from events where kind='error' and text like '%timed out%'").fetchone()[0]==1
 results['timeout_rolls_back_candidate_preserves_refs_and_pending_work']=True
 calls=[]
 def retry_sync():
  calls.append(time.time())
  if len(calls)==1:timed_out_sync()
  supervisor.last_sync=time.time();supervisor.last_completed_sync=supervisor.last_sync;supervisor.stopping=True
 supervisor.sync=retry_sync;supervisor.last_sync=0;supervisor.next_sync_retry_at=0
 with patch.object(factory,'SYNC_RETRY_SECONDS',0):
  thread=threading.Thread(target=supervisor.integrator_loop);supervisor.integrator_thread=thread;thread.start();thread.join(5)
 assert not thread.is_alive() and len(calls)==2 and supervisor.next_sync_retry_at==0
 assert git('rev-parse','main')==base and all(pathlib.Path(p).read_bytes()==b for p,b in before.items())
 results['integrator_loop_survives_timeout_and_retries']=True
 # A real timed-out tool with a real child may not write later into a restored candidate.
 marker=root/'orphan_marker';script=repository/'slow_check.py';script.write_text('import subprocess,sys,time\nsubprocess.Popen([sys.executable,"-c",'+repr('import pathlib,time;time.sleep(1);pathlib.Path('+repr(str(marker))+').write_text("unsafe")')+'])\nprint("started",flush=True)\ntime.sleep(30)\n')
 began=time.time()
 try:factory.tool(repository,'slow_check.py',timeout=.2)
 except subprocess.TimeoutExpired as error:assert 'started' in error.output
 else:raise AssertionError('Timeout did not propagate')
 time.sleep(1.1);assert not marker.exists();assert time.time()-began<5
 results['timeout_terminates_compiler_children_before_recovery']=True
 calls=[];supervisor.last_audit_day=datetime.datetime.now(factory.EASTERN).date()
 def fake_tool(worktree,*args,timeout=900):calls.append((args,timeout));return 0,''
 with patch.object(factory,'tool',fake_tool),patch.object(factory,'full_image_compare',lambda _:None),patch.object(factory,'lost_exact',lambda _:[]),patch.object(factory,'gained_exact',lambda _:[]):
  assert supervisor.full_check(repository)==(True,'',[],set())
 assert calls==[(('make.py','eu','-ca'),7200),(('tools/check.py','-q','-w'),7200)]
 results['full_check_uses_two_hour_stage_budgets']=True
 # Status must report completion time, rather than a failed attempt's start time.
 supervisor.last_completed_sync=120;supervisor.last_sync=999
 status=factory.integrator_queue(supervisor,None,time.time());assert status['thread_alive'] is False and status['last_completed_sync']==datetime.datetime.fromtimestamp(120,factory.EASTERN).isoformat(timespec='seconds')
 results['status_reports_thread_liveness_and_completed_sync']=True
 results['production_paths_used']=False
output={'candidate_sha256':hashlib.sha256(candidate.read_bytes()).hexdigest(),'checks':results,'passed':all(results[k] for k in results if k!='production_paths_used')}
print(json.dumps(output,indent=2));pathlib.Path(__file__).with_name('integrator_timeout_recovery_results.json').write_text(json.dumps(output,indent=2)+'\n')
