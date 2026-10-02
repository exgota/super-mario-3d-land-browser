from pathlib import Path
import subprocess,os,json,hashlib
p=Path('data/ver/eu/map.csv');before=p.read_bytes()
imports={0x1c440c:'fn_001C440C',0x24e47c:'fn_0024E47C',0x1d956c:'fn_001D956C',0x24e598:'fn_0024E598',0x24e568:'fn_0024E568',0x24e49c:'fn_0024E49C',0x293088:'fn_00293088',0x26ac60:'fn_0026AC60',0x2a63d8:'fn_002A63D8',0x3d62a0:'dat_003D62A0',0x3d8008:'dat_003D8008',0x3d82a4:'dat_003D82A4',0x3d8500:'dat_003D8500',0x3e23b8:'dat_003E23B8'}
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
 r=subprocess.run(['python','tools/check.py','fn_001C440C','--object','build/eu/obj/lib/al/src/Model/alSkeletalAnimationConstruction.o'],capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'))
 print(r.stdout+r.stderr)
 Path('build/skeletal_animation_validation/canonical-check.log').write_text(r.stdout+r.stderr)
finally:
 p.write_bytes(before);assert p.read_bytes()==before
 print('Map restored',hashlib.sha256(before).hexdigest())
