"""Probe only existing map-row spellings, and restore all map bytes afterward."""
from pathlib import Path
import subprocess,os,json,hashlib,sys
p=Path('data/ver/eu/map.csv');before=p.read_bytes()
imports={0x2a2c9c:'fn_002A2C9C',0x254890:'fn_00254890',0x28a998:'fn_0028A998',0x291470:'fn_00291470',0x2b349c:'fn_002B349C',0x3f389c:'dat_003F389C',0x3f38a8:'dat_003F38A8',0x3d82d0:'dat_003D82D0',0x3d86bc:'dat_003D86BC'}
try:
 lines=[];found=set()
 for line in before.decode().splitlines(True):
  f=line.split(',')
  try:a=int(f[0],16)
  except:lines.append(line);continue
  if a in imports:
   assert f[6].strip() in ('',imports[a]),(a,f)
   f[6]=imports[a];found.add(a)
  lines.append(','.join(f))
 assert found==set(imports),set(imports)-found
 p.write_text(''.join(lines))
 r=subprocess.run([sys.executable,'tools/check.py','fn_002A2C9C','--object','build/eu/obj/lib/al/src/Model/alTransformStateFactory.o'],capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'))
 print(r.stdout+r.stderr)
 Path('build/transform_state_validation/canonical-check.log').write_text(r.stdout+r.stderr)
finally:
 p.write_bytes(before);assert p.read_bytes()==before
 print('Map restored',hashlib.sha256(before).hexdigest())
