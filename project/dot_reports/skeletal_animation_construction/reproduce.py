"""Rebuild/check/replay the unchanged committed candidate; never enroll its row."""
from pathlib import Path
import hashlib,json,os,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2];os.chdir(ROOT)
D=Path('build/skeletal_animation_validation');D.mkdir(parents=True,exist_ok=True)
manifest=json.loads((HERE/'manifest.json').read_text())
for path,expected in manifest['inputs'].items():
 assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==expected,path
assert hashlib.sha256(Path('data/ver/eu/code.bin').read_bytes()).hexdigest()==manifest['original_sha256']
env=dict(os.environ,TMP='/tmp',DEVKITARM='/usr')
def run(args,log):
 started=time.time()
 p=subprocess.run(args,env=env,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
 (D/log).write_text(p.stdout)
 print(log,p.returncode,round(time.time()-started,3),'seconds')
 assert p.returncode==0,p.stdout[-3000:]
run([sys.executable,'make.py','eu','-ca'],'clean-build-reproduction.log')
run([sys.executable,str(HERE/'check_candidate.py')],'check-reproduction.log')
obj=Path('build/eu/obj/lib/al/src/Model/alSkeletalAnimationConstruction.o')
assert hashlib.sha256(obj.read_bytes()).hexdigest()==manifest['object_sha256']
run(['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry=fn_001C440C','--keep=fn_001C440C','--ro_base=0x00500000','--output='+str(D/'candidate.axf'),'--list='+str(D/'candidate.map'),str(obj),str(HERE/'original_symbols.sym')],'diagnostic-link.log')
run([sys.executable,str(HERE/'replay.py'),'full'],'returning-replay.log')
run([sys.executable,str(HERE/'extended.py')],'extended-replay.log')
# The pristine 861 baseline is optional; it is never edited or rebuilt here.
if len(sys.argv)>1:run([sys.executable,str(HERE/'preserve.py'),sys.argv[1]],'preservation-reproduction.log')
print('NonMatching; zero exact credit. Row bytes restored by checker wrapper.')
