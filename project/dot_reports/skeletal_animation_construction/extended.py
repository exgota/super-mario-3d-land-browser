from replay import *
start=time.time();cases=[]
for count in [1,2,4]:
 base=dict(count=count,tracks=[2,4,0],joints=3,models=3,flags=0,index=1,bindings=3,stride=2)
 original=run(base,False)
 for fail in range(len(original['allocs'])):cases.append(dict(base,fail=fail))
 for caller in [True]:cases.append(dict(base,caller=caller))
 for bad in ['resource_null','resource_type','skeleton_null','dictionary_null','record_null','name_null']:cases.append(dict(base,bad=bad))
 for alias in ['records','binding_self']:cases.append(dict(base,alias=alias,index=0))
 cases.append(dict(base,repeats=2))
results=[];mismatches=[]
for c in cases:
 try:results.append({'case':c,'result':compare(c)})
 except Exception as e:
  mismatches.append({'case':c,'error':str(e)[:2000]})
(D/'extended.json').write_text(json.dumps({'seconds':time.time()-start,'results':results,'mismatches':mismatches,'coverage':sorted(coverage)},indent=2))
print(json.dumps({'cases':len(cases),'equal':len(results),'mismatches':len(mismatches),'coverage':len(coverage),'seconds':time.time()-start}))
for m in mismatches:print(m)

assert not mismatches, "Differential replay diverged; see extended.json"
expected=set(range(0x1c440c,0x1c4900,4)) | set(range(0x1c4930,0x1c4c7c,4))
assert coverage <= expected, "A literal pool was executed"
assert len(coverage)==466, "Unexpected instruction coverage"
