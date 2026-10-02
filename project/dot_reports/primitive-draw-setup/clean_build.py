from pathlib import Path
import subprocess,json,time,datetime
out=Path('build/primitive-draw-setup');start=time.monotonic();stamp=datetime.datetime.now(datetime.timezone.utc).isoformat()
with (out/'final-clean-build.txt').open('w') as f:r=subprocess.run(['python','make.py','eu','-ca'],stdout=f,stderr=subprocess.STDOUT)
result={'command':['python','make.py','eu','-ca'],'started_at':stamp,'completed_at':datetime.datetime.now(datetime.timezone.utc).isoformat(),'seconds':time.monotonic()-start,'status':r.returncode};(out/'final-clean-build.json').write_text(json.dumps(result,indent=2));print(result);r.check_returncode()
