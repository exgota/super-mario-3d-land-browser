#!/usr/bin/env python3
"""Read-only snapshot of exact neutral-import declarations in local dot refs."""
from pathlib import Path
import subprocess,json,re,hashlib
root=Path(__file__).resolve().parents[3];names=[x['symbol'] for x in json.loads((root/'project/dot_reports/course-select-scene-init/imports.json').read_text()) if x['symbol'].startswith(('fn_','dat_'))]
refs=subprocess.check_output(['git','for-each-ref','--format=%(refname)','refs/heads/dot','refs/remotes/origin/dot'],text=True).splitlines()
heads={ref:subprocess.check_output(['git','rev-parse',ref],text=True).strip() for ref in refs}
blobs={}
for ref in refs:
 for line in subprocess.check_output(['git','ls-tree','-r',heads[ref],'--','Game','lib/al','lib/sead'],text=True).splitlines():
  attrs,path=line.split('\t',1);mode,kind,sha=attrs.split()
  if kind=='blob' and Path(path).suffix in ['.h','.cpp'] and path!='Game/backup/src/Scene/CourseSelectSceneInit.cpp':blobs.setdefault(sha,[]).append([ref,path])
patterns=re.compile(rb'\b('+b'|'.join(n.encode() for n in names)+rb')\b')
records=[]
for sha,locations in blobs.items():
 raw=subprocess.check_output(['git','cat-file','blob',sha]);matches=set(m.decode() for m in patterns.findall(raw))
 if not matches:continue
 lines=raw.decode('cp932',errors='replace').splitlines()
 for name in matches:
  for i,line in enumerate(lines):
   if re.search(r'\b'+name+r'\b',line):records.append({'symbol':name,'blob':sha,'paths':sorted(set(p for _,p in locations)),'refs':sorted(set(r for r,_ in locations)),'line':i+1,'text':'\n'.join(lines[max(0,i-1):i+2])})
result={'ref_heads':heads,'refs_screened':len(refs),'unique_source_header_blobs':len(blobs),'exact_symbol_hits':records}
out=root/'project/dot_reports/course-select-scene-init/import-compatibility.json';out.write_text(json.dumps(result,indent=2)+'\n')
print('refs',len(refs),'blobs',len(blobs),'hits',len(records))
for r in records:print(r['symbol'],r['paths'],r['text'])
