# Diagnostic link reproduction

Run from the repository root after the committed source project build. This is diagnostic evidence only: canonical unresolved source closure remains rejected. It requires the local owner-supplied game binary and the approved ARMCC toolchain. No game bytes are embedded.

```python
from pathlib import Path
import json,subprocess,hashlib,os,csv
from elftools.elf.elffile import ELFFile
r=Path.cwd();o=r/'build/eu/obj/lib/al/src/Framework/seadDualScreenMethodTreeMgr.o';out=r/'build/dot_method_tree_diagnostic';out.mkdir(exist_ok=True)
name='_ZN4sead23DualScreenMethodTreeMgrC1Ev';base=0x2e3144
imports={'_ZN4sead13MethodTreeMgrC2Ev':(0x2dbf14,'A'),'_ZN4sead14MethodTreeNode13pushBackChildEPS0_':(0x223aa8,'A'),'_ZN4sead14MethodTreeNode5lock_Ev':(0x28cbec,'A'),'_ZN4sead14MethodTreeNode7unlock_Ev':(0x28cbd8,'A'),'_ZN4sead14MethodTreeNodeC1EPNS_15CriticalSectionE':(0x28b818,'A'),'_ZTVN4sead23DualScreenMethodTreeMgrE':(0x3da670,'D')}
with o.open('rb') as f:
 e=ELFFile(f);s=e.get_section_by_name('i.'+name);size=s['sh_size'];sy=e.get_section_by_name('.symtab');rel=e.get_section_by_name('.reli.'+name)
 assert {sy.get_symbol(x['r_info_sym']).name for x in rel.iter_relocations()}<=imports.keys()
(out/'original_symbols.sym').write_text('#<SYMDEFS>#\n'+''.join(f'0x{a:08X} {k} {n}\n' for n,(a,k) in imports.items()))
(out/'candidate.sct').write_text(f'CANDIDATE_LOAD 0x{base:08X}\n{{\n CANDIDATE_CODE 0x{base:08X}\n {{\n seadDualScreenMethodTreeMgr.o (i.{name}, +FIRST)\n }}\n}}\n')
cmd=[str(r/'data/compilers/wibo'),str(r/'data/compilers/4.1/791/bin/armlink.exe'),'--cpu=MPCore','--fpu=VFPv2','--arm_only','--no_exceptions','--inline','--datacompressor=off','--no_debug','--no_scanlib','--mangled','--symbols','--map','--entry='+name,'--keep='+name,f'--scatter={out}/candidate.sct',f'--output={out}/candidate.axf',f'--list={out}/candidate.map',str(o),str(out/'original_symbols.sym')]
p=subprocess.run(cmd,env=dict(os.environ,TMP='/tmp'),capture_output=True,text=True);(out/'link.log').write_text(p.stdout+p.stderr);assert p.returncode==0,p.stdout+p.stderr
with (out/'candidate.axf').open('rb') as f:
 e=ELFFile(f);ss=[s for s in e.iter_sections() if s['sh_flags']&2 and s['sh_size']];assert len(ss)==1 and ss[0]['sh_addr']==base and ss[0]['sh_size']==size;linked=ss[0].data()
binary=(r/'data/ver/eu/code.bin').read_bytes();assert hashlib.sha256(binary).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64';original=binary[base-0x100000:base-0x100000+1284]
evidence={'object_sha256':hashlib.sha256(o.read_bytes()).hexdigest(),'size':size,'target_size':len(original),'linked_sha256':hashlib.sha256(linked).hexdigest(),'original_sha256':hashlib.sha256(original).hexdigest(),'differing_bytes':sum(a!=b for a,b in zip(linked,original))+abs(len(linked)-len(original)),'command':cmd,'imports':imports,'diagnostic_only':'Unchanged project-built full object linked at original address using independently observed but unaccepted vtable identity; no target/object/checker/map data row changes. Canonical checker rejects unresolved source closure. Not exact acceptance evidence.'};(out/'link-evidence.json').write_text(json.dumps(evidence,indent=2));print(json.dumps(evidence,indent=2))
```
