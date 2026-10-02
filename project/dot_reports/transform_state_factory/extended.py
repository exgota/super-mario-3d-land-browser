"""Bounded alias and allocator-mutation controls, separated from ordinary inputs."""
import replay as r
from pathlib import Path
import json,time
cases=[]
for count in [0,1,3]:
 for address in [r.DONOR,r.DONOR+4,r.DONORBUF]:
  cases.append(dict(count=count,word=2,mode=1,self_address=address,category='receiver/descriptor-or-donor-storage overlap'))
 for address in [r.DONORBUF,r.RESOURCE+0x40]:
  cases.append(dict(count=count,word=2,mode=1,donor_address=address,category='separate descriptor placement'))
 for allocator in [0,r.ALLOC]:
  cases.append(dict(count=count,word=2,mode=1,donor_allocator=allocator,donor_buffer=0,donor_count=0,flags=0xc0000000,category='null donor storage'))
for callback in [0,1,2,3]:
 for after in [0,1,3]:
  cases.append(dict(count=3,word=2,mode=1,mutate_counts={callback:after},category='allocator changes later-observed resource count'))
start=time.time();results=[r.compare(c)for c in cases]
report={'results':results,'seconds':time.time()-start,'cases':len(cases),'matched':sum(not x['differences']for x in results),'divergences':sum(bool(x['differences'])for x in results),'returns':sum(x['original']['stopped']=='return'for x in results),'faults':sum(x['original']['stopped']=='fault'for x in results),'budgets':sum(x['original']['stopped']=='budget'for x in results),'coverage':sorted(r.coverage)}
Path(r.D/'extended-result.json').write_text(json.dumps(report,indent=2));print(json.dumps({k:report[k]for k in ['cases','matched','divergences','returns','faults','budgets','seconds']}))
for x in results:
 if x['differences']:print(json.dumps(x['differences']))
if report['divergences']:raise SystemExit(1)
