"""Clean normal build, unchanged canonical refusal, diagnostic replay, preservation."""
from pathlib import Path
import hashlib,json,os,subprocess,sys,time,datetime
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2];os.chdir(ROOT)
D=Path('build/transform_state_validation');D.mkdir(parents=True,exist_ok=True)
manifest=json.loads((HERE/'manifest.json').read_text())
for path,expected in manifest['inputs'].items():
 assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==expected,path
assert hashlib.sha256(Path('data/ver/eu/code.bin').read_bytes()).hexdigest()==manifest['original_sha256']
env=dict(os.environ,TMP='/tmp',DEVKITARM='/usr');timings={}
def run(args,log):
 started=time.time()
 p=subprocess.run(args,env=env,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
 (D/log).write_text(p.stdout);timings[log]=time.time()-started
 print(log,p.returncode,round(timings[log],3),'seconds',flush=True)
 assert p.returncode==0,p.stdout[-3000:]
run([sys.executable,'make.py','eu','-ca'],'clean-build-reproduction.log')
run([sys.executable,str(HERE/'check_candidate.py')],'check-reproduction.log')
check=(D/'canonical-check.log').read_text()
assert 'Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.' in check,check
obj=Path('build/eu/obj/lib/al/src/Model/alTransformStateFactory.o')
assert hashlib.sha256(obj.read_bytes()).hexdigest()==manifest['object_sha256']
run(['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_002A2C9C','--keep=fn_002A2C9C','--ro_base=0x00500000','--output='+str(D/'candidate.axf'),'--list='+str(D/'candidate.map'),str(obj),str(HERE/'original_symbols.sym')],'diagnostic-link-reproduction.log')
run([sys.executable,str(HERE/'replay.py'),'full'],'replay-reproduction.log')
run([sys.executable,str(HERE/'extended.py')],'extended-reproduction.log')
run([sys.executable,str(HERE/'audit.py')],'audit-reproduction.log')
# This optional baseline is read only: never rebuilt, checked, or map-mutated here.
if len(sys.argv)>1:run([sys.executable,str(HERE/'preserve.py'),sys.argv[1]],'preservation-reproduction.log')
summary={'finished_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'timings':timings,'object_sha256':manifest['object_sha256'],'kind':'NonMatching; canonical missing-row refusal; zero exact credit','map_sha256':hashlib.sha256(Path('data/ver/eu/map.csv').read_bytes()).hexdigest(),'artifacts':{p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in D.iterdir()if p.is_file()and(p.name.endswith('result.json')or p.name in ['preservation.json','object-audit.json'])}}
(D/'validation-summary.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary))
