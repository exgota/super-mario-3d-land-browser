from pathlib import Path
import subprocess,sys,time,json,datetime
here=Path(__file__).resolve().parent
out=Path('build/primitive-draw-setup');out.mkdir(parents=True,exist_ok=True)
start=time.monotonic();started=datetime.datetime.now(datetime.timezone.utc).isoformat()
for name in ['verify_inputs.py','clean_build.py','check.py','link_replay.py','replay.py','coverage.py','static_evidence.py','preserve.py','seal.py','verify_inputs.py']:
 subprocess.run([sys.executable,str(here/name)],check=True)
(out/'reproduce-run.json').write_text(json.dumps({'started_at':started,'completed_at':datetime.datetime.now(datetime.timezone.utc).isoformat(),'seconds':time.monotonic()-start,'status':'passed diagnostics; root remains NonMatching'},indent=2)+'\n')
