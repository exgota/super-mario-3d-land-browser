#!/usr/bin/env python3
"""Run the unchanged canonical checker and restore map bytes in all outcomes."""
from pathlib import Path
import subprocess, os, json, time, hashlib, sys
out=Path('build/item-spawn-dispatcher');out.mkdir(parents=True,exist_ok=True)
p=Path('data/ver/eu/map.csv');before=p.read_bytes();lines=[]
for line in before.decode().splitlines(True):
    fields=line.split(',')
    if fields[0]=='0x002CDEA4':
        assert fields[6].strip() in ('','fn_002CDEA4')
        fields[6]='fn_002CDEA4'
    lines.append(','.join(fields))
try:
    p.write_text(''.join(lines));start=time.time()
    result=subprocess.run(['python','tools/check.py','fn_002CDEA4','--object','build/eu/obj/Game/backup/src/MapObj/ItemSpawnDispatcher2CDEA4.o'],capture_output=True,text=True,env=dict(os.environ,TMP='/tmp'))
    report={'commit':subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),'seconds':time.time()-start,'returncode':result.returncode,'output':result.stdout+result.stderr,'map_before_sha256':hashlib.sha256(before).hexdigest()}
finally:
    p.write_bytes(before)
report['map_after_sha256']=hashlib.sha256(p.read_bytes()).hexdigest()
(out/('check-'+sys.argv[1]+'.json')).write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
