# Reproduce the ItemHolder family diagnostics

This is a scratch diagnostic recipe, not a replacement checker. Keep all binary
outputs under ignored build/. Never commit them. Use the owner's fingerprinted
private EU executable and the approved ARMCC toolchain. Start on this branch at
the final committed source and source development_environment.sh.

Run the normal `python make.py eu -ca`. Save the following script as
`build/item_holder_family/diagnose.py` and run it with argument `form7`. Its
proposed map rows exist only in memory; it does not invoke `check.py`.

```python
from pathlib import Path
from elftools.elf.elffile import ELFFile
import sys,hashlib,json,subprocess,struct
sys.path.insert(0,str(Path.cwd()))
from tools.low.checkExactBytes import _read_map,_isolate_function
out=Path('build/item_holder_family');form=sys.argv[1]
rows=_read_map(Path('data/ver/eu/map.csv'))
# Diagnostic ownership proposal only. Never feed these rows to check.py.
for r in rows:
 if r['Start']==0x3c47d0:r['End']=0x3c4878
 if r['Start']==0x3c4880:r['Start'],r['End']=0x3c4878,0x3c488c
 if r['Start']==0x3c4894:r['Start'],r['End'],r['Symbol']=0x3c488c,0x3c48a0,'_ZTV10ItemHolder'
 if r['Start']==0x275828:r['Symbol']='_ZN10ItemHolderC1Ebbi'
 if r['Start']==0x27a7b0:r['Symbol']='_ZN9dot2758287NameRefC1EPKc'
rows.append(dict(Start=0x3c48a0,End=0x3c48a8,Rank='U',Type='dc',Symbol='',SectionName=''))
obj=Path('build/eu/obj/lib/al/src/ItemHolder275828.o')
with obj.open('rb') as f:
 elf=ELFFile(f);t=elf.get_section_by_name('.constdata__ZTV10ItemHolder');sy=elf.get_section_by_name('.symtab')
 assert t.data_size==20
 print('native table extent',t.data_size)
 for s in sy.iter_symbols():
  if s.name=='_ZTV10ItemHolder' and isinstance(s['st_shndx'],int):assert s['st_size']==20
name='_ZN10ItemHolderC1Ebbi'
section,compiled,imports=_isolate_function(obj,name,rows,out/'candidate.o')
(out/'imports.json').write_text(json.dumps(imports,indent=2))
(out/'diagnostic_symbols.sym').write_text('#<SYMDEFS>#\n'+''.join('0x%08X %s %s\n'%(x['address'],x['kind'],x['symbol']) for x in imports))
(out/'candidate.sct').write_text('DIAGNOSTIC 0x00275828 { CODE 0x00275828 { candidate.o ('+section+', +FIRST) } }\n')
cmd=['data/compilers/wibo','data/compilers/4.1/791/bin/armlink.exe','--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--entry='+name,'--keep='+name,'--scatter='+str(out/'candidate.sct'),'--output='+str(out/(form+'.axf')),str(out/'candidate.o'),str(out/'diagnostic_symbols.sym')]
r=subprocess.run(cmd,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
with (out/(form+'.axf')).open('rb') as f:linked=ELFFile(f).get_section_by_name('CODE').data()
b=Path('data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(b).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
original=b[0x175828:0x176534]
bad=[i for i in range(min(len(original),len(linked))) if original[i]!=linked[i]]
result=dict(form=form,original_size=len(original),compiled_size=len(linked),differing_paired_bytes=len(bad),first_differences=[hex(0x275828+i) for i in bad[:30]],diagnostic_equal=original==linked,canonical_accepted=False,object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),linked_sha256=hashlib.sha256(linked).hexdigest(),imports=imports)
(out/(form+'.json')).write_text(json.dumps(result,indent=2)+'\n'); print(json.dumps(result,indent=2))
from capstone import *
cs=Cs(CS_ARCH_ARM,CS_MODE_ARM);cs.skipdata=True
(out/(form+'.txt')).write_text('\n'.join(f'{i.address:08x} {i.mnemonic:8s} {i.op_str}' for i in cs.disasm(linked,0x275828)))
print('\n'.join((out/(form+'.txt')).read_text().splitlines()[:40]))
```

For prior-root preservation, create an ignored manifest containing
`{"prior_checkpoint":"b16e2a0cc32e0cdacac5ba94feabe67432433391","candidates":[]}`
and run `tools/acceptance_batch.py` with that manifest and a fresh ignored
output path. This intentionally checks zero candidates. The batch tool's
empty candidate-set result must never be described as constructor acceptance.
It clean-builds and checks every previous root and actual selected canonical
definition. Canonical target grading waits for the metadata prerequisites in
the family report.

For original-callee replay, use the complete script in the carried report
`project/dot_reports/root-275828-replay.md` on branch `dot/root-275828`, commit
`cd620f97827f8a0ea3c48aeb92c76660f639f613`. Replace only the diagnostic image
path `build/dot_275828/replay.axf` with
`build/item_holder_family/form7.axf` and the default report path with an ignored
path in `build/item_holder_family`. Run `--scope heap --domain valid`, then
`--scope heap --domain abi` with a separate output. These substitutions change
input/output paths only, not fixtures, original-callee execution or assertions.
The actual adapted script has SHA-256
`7c08f542fb99445a886c716d55611f6d877251c30318fc9ff31d4005705224d5`.
The diagnostic script above has SHA-256
`2283f79fef514633dbad7b462de4a6502ae9e5ce4c27fa5d829a14fa5fff91f6`.

The actual preservation run was interrupted after 191 passing definitions. It
resumed against the same committed source, canonical map and already-built
objects. Before skipping those completed pairs, the resume driver verified all
canonical object hashes and every provenance input hash, reconstructed the
same strong/weak definition selection, and checked every remaining pair with
the unchanged `tools/check.py`. It saved the interrupted report separately and
kept candidate acceptance false. A fresh uninterrupted run of the standard
empty-candidate manifest above is the simpler full reproduction.
