from pathlib import Path
import re,subprocess,json,hashlib,time,os
out=Path('build/primitive-draw-setup');p=Path('data/ver/eu/map.csv');old=p.read_bytes()
assert old==(Path(os.environ.get('PRIMITIVE_BASELINE','../mario-main861'))/'data/ver/eu/map.csv').read_bytes()
src='\n'.join(Path(f).read_text() for f in ['lib/al/src/Graphics/retail_PrimitiveDrawSetup.cpp','lib/al/include/Observed/PrimitiveDrawSetup.h']);names={int(a,16):prefix+a for prefix,a in re.findall(r'\b(fn_|dat_)([0-9A-F]{8})\b',src)}
lines=[];changed=[]
for l in old.decode().splitlines(True):
 r=l.rstrip('\n').split(',')
 if r and r[0].startswith('0x') and int(r[0],16) in names:
  assert not r[6].strip() or r[6]==names[int(r[0],16)],l
  r[6]=names[int(r[0],16)];changed.append(r[6]);l=','.join(r)+'\n'
 lines.append(l)
begin=time.monotonic()
try:
 p.write_text(''.join(lines))
 command=['python','tools/check.py','fn_002E0D2C','--object','build/eu/obj/lib/al/src/Graphics/retail_PrimitiveDrawSetup.o']
 r=subprocess.run(command,text=True,capture_output=True);(out/'form1-check.txt').write_text(r.stdout+r.stderr);print(r.stdout,r.stderr)
 command=['python','tools/low/checkExactBytes.py','fn_002E0D2C','build/eu/obj/lib/al/src/Graphics/retail_PrimitiveDrawSetup.o','--compiler','4.1/791']
 r=subprocess.run(command,text=True,capture_output=True);(out/'form1-result.json').write_text(r.stdout)
 result=json.loads(r.stdout)
 assert r.returncode==1 and result['exact'] is False and result['evidence']['compiled_section_size']==1968 and result['evidence']['original_size']==2072, result
finally:p.write_bytes(old)
assert p.read_bytes()==old
(out/'form1-check-meta.json').write_text(json.dumps({'elapsed_seconds':time.monotonic()-begin,'map_restored_sha256':hashlib.sha256(old).hexdigest(),'renamed_existing_rows':changed},indent=2))
