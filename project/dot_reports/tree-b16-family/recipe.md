# Executed TreeNode revalidation recipe

Run from the submitted repository root. The intended source must already be
committed. Supply the owner's approved local EU code/exheader, configured
compilers and Python environment in their ignored paths. The source-only patch
was separately checked with `git apply --check` against b16 before submission.

The following clean-build command was executed:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python - <<'PY'
import subprocess,time,json,datetime
from pathlib import Path
out=Path('build/tree_b16_family');out.mkdir(parents=True)
s={'start_utc':datetime.datetime.now(datetime.timezone.utc).isoformat()};t=time.monotonic()
with (out/'clean_build.log').open('w') as log:
 r=subprocess.run(['python','make.py','eu','-ca'],stdout=log,stderr=subprocess.STDOUT)
s.update(end_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),elapsed_seconds=time.monotonic()-t,returncode=r.returncode)
(out/'build_timing.json').write_text(json.dumps(s,indent=2)+'\n')
print(json.dumps(s));raise SystemExit(r.returncode)
PY
```

The complete verification driver below was then executed. It scans every
canonical source object, checks all accepted symbol definitions without filtering
weak copies, gates the candidate on all prior passes, and restores original map
bytes in a finally block. It calls the unmodified project checker; it does not
infer or write matching ranks itself.

```sh
cat > /tmp/verify_tree_b16_family.py <<'PY'
import csv,datetime,hashlib,json,os,re,subprocess,sys,time
from pathlib import Path
from elftools.elf.elffile import ELFFile
root=Path.cwd()
base='b16e2a0cc32e0cdacac5ba94feabe67432433391'
source='lib/al/src/Util/seadTreeNode.cpp'
obj='build/eu/obj/lib/al/src/Util/seadTreeNode.o'
candidate='_ZN4sead8TreeNode14pushFrontChildEPS0_'
out=root/'build/tree_b16_family'
out.mkdir(parents=True,exist_ok=True)
hashfile=lambda p:hashlib.sha256((root/p).read_bytes()).hexdigest()
now=lambda:datetime.datetime.now(datetime.timezone.utc).isoformat()
map_path=root/'data/ver/eu/map.csv';before=map_path.read_bytes()
assert before==subprocess.check_output(['git','show',base+':data/ver/eu/map.csv'])
assert not subprocess.check_output(['git','diff','--name-only','HEAD','--','Game','lib','data','tools']).strip()
rows=[{k.strip():v.strip() for k,v in row.items()} for row in csv.DictReader(before.decode().splitlines())]
accepted=[r for r in rows if r['Rank']=='O' and r['Type'].startswith('f')]
by_name={r['Symbol']:r for r in rows if r['Symbol']}
assert len(accepted)==667 and sum(int(r['End'],16)-int(r['Start'],16) for r in accepted)==37224
assert by_name[candidate]['Rank']=='U'
found={r['Symbol']:[] for r in accepted};family_symbols=[]
for p in sorted((root/'build/eu/obj').rglob('*.o')):
 rel=p.relative_to(root/'build/eu/obj')
 if rel.parts[0] not in ('Game','lib') or not (root/rel.with_suffix('.cpp')).is_file():continue
 with p.open('rb') as stream:
  elf=ELFFile(stream);table=elf.get_section_by_name('.symtab')
  if table is None:continue
  for definition in table.iter_symbols():
   if not isinstance(definition['st_shndx'],int) or definition['st_info']['type']!='STT_FUNC':continue
   entry={'object':str(p.relative_to(root)),'bind':definition['st_info']['bind'],'section':elf.get_section(definition['st_shndx']).name,'value':definition['st_value'],'size':definition['st_size']}
   if definition.name in found:found[definition.name].append(entry)
   if str(p.relative_to(root))==obj:family_symbols.append(dict(symbol=definition.name,**entry))
assert all(found.values()),{n:v for n,v in found.items() if not v}
(out/'inventory.json').write_text(json.dumps({'prior_definitions':found,'family_symbols':family_symbols},indent=2)+'\n')
print('Inventory:',len(found),'prior roots;',sum(len(v) for v in found.values()),'actual prior definitions;',len(family_symbols),'Tree function symbol entries',flush=True)
report={'base':base,'source_checkpoint':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'prior_roots':667,'prior_bytes':37224,'prior_checks':[],'candidate_checks':[],'family_symbols':family_symbols,'start_utc':now(),'build':json.loads((out/'build_timing.json').read_text()),'hashes':{p:hashfile(p) for p in [source,obj,'data/ver/eu/code.bin','data/ver/eu/map.csv','data/config.json','make.py','tools/check.py','tools/acceptance_batch.py','tools/low/checkExactBytes.py','tools/low/buildProvenance.py','data/compilers/4.1/791/bin/armcc.exe']}}
t=time.monotonic()
clean=lambda s:re.sub(r'\x1b\[[0-9;]*[A-Za-z]','',s).strip()
def save():
 report['elapsed_seconds']=time.monotonic()-t
 (out/'checks.json').write_text(json.dumps(report,indent=2)+'\n')
def check(symbol,path):
 run=subprocess.run([sys.executable,'tools/check.py',symbol,'--object',path],capture_output=True,text=True)
 text=clean(run.stdout+run.stderr)
 return {'symbol':symbol,'object':path,'object_sha256':hashfile(path),'returncode':run.returncode,'exact':run.returncode==0 and 'complete source-generated function interval matches byte for byte' in text,'output':text}
try:
 for i,(symbol,definitions) in enumerate(found.items()):
  for definition in definitions:
   result=check(symbol,definition['object']);report['prior_checks'].append(result)
   assert result['exact'],result
  save()
  if (i+1)%50==0:print('Preserved',i+1,'/',len(found),'roots',flush=True)
 report['prior_complete_utc']=now();save()
 assert len(report['prior_checks'])==sum(len(v) for v in found.values())
 result=check(candidate,obj);report['candidate_checks'].append(result);save();assert result['exact'],result
 # A separate byte comparison records pool-inclusive lengths and zero differing bytes.
 from tools.low.checkExactBytes import check_exact_bytes
 from tools.low.buildProvenance import verify_build_output
 assert verify_build_output(root/obj)['compiler']=='4.1/791'
 report['family_byte_checks']={}
 for symbol in [candidate,'_ZN4sead8TreeNode13detachSubTreeEv','_ZN4sead8TreeNode27clearChildLinksRecursively_Ev','_ZN4sead8TreeNodeC2Ev']:
  result=check_exact_bytes(symbol,root/obj,output_directory=out/'byte_checks'/symbol)
  report['family_byte_checks'][symbol]=result
  assert result['exact'] and result['evidence']['different_bytes']==0,result
 report['finished_utc']=now();report['all_exact']=True
finally:
 map_path.write_bytes(before)
 report['map_restored']=map_path.read_bytes()==before
 report['final_map_sha256']=hashfile('data/ver/eu/map.csv')
 save()
assert report['map_restored']
print('Exact:',len(report['prior_checks']),'prior definitions,',len(report['candidate_checks']),'candidate; seconds',report['elapsed_seconds'],flush=True)
PY
. ./development_environment.sh
export DEVKITARM=/usr
PYTHONPATH=. python /tmp/verify_tree_b16_family.py > build/tree_b16_family/check_driver.log 2>&1
```

Read the fresh checks.json and driver log under build/tree_b16_family. A failure
leaves no acceptance claim; main must run its own enrollment and acceptance gate.
The driver deliberately requires the b16 map and 667-root/37,224-byte baseline.
Do not relax these assertions for a later checkpoint without new review.
