from pathlib import Path
import json
out=Path('build/primitive-draw-setup')
r=json.loads((out/'replay-result.json').read_text())
expected=set(range(0x2e0d2c,0x2e1124,4))|set(range(0x2e1194,0x2e1530,4))
observed={int(a,16) for a in r['root_instruction_addresses']}
result={'expected_instruction_count':len(expected),'observed_instruction_count':len(observed),'missed':[hex(a) for a in sorted(expected-observed)],'unexpected':[hex(a) for a in sorted(observed-expected)]}
(out/'coverage.json').write_text(json.dumps(result,indent=2)+'\n')
assert observed==expected,result
print(result)
