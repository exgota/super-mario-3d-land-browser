from pathlib import Path
from capstone import *
import csv,struct,json,hashlib
out=Path('build/primitive-draw-setup');code=Path('data/ver/eu/code.bin').read_bytes();rows=[{k.strip():v.strip() for k,v in r.items()} for r in csv.DictReader(open('data/ver/eu/map.csv'))]
md=Cs(CS_ARCH_ARM,CS_MODE_ARM);md.detail=True
ranges=[(0x2e0d2c,0x2e1124),(0x2e1194,0x2e1530)];calls=[];poolreads=[]
for lo,hi in ranges:
 for i in md.disasm(code[lo-0x100000:hi-0x100000],lo):
  if i.mnemonic.startswith('bl') or (i.mnemonic=='b' and i.address==0x2e152c):
   entry={'address':hex(i.address),'mnemonic':i.mnemonic,'target':i.op_str}
   if i.operands and i.operands[0].type==CS_OP_IMM:
    target=i.operands[0].imm;entry['map_row']=next((r for r in rows if int(r['Start'],16)==target),None)
   calls.append(entry)
fnrows={int(r['Start'],16):r for r in rows if r['Type'].startswith('f')}
refs=[]
for target in [0x10349c,0x2e0d2c]:
 for off in range(0,0x2a0000,4):
  word=struct.unpack_from('<I',code,off)[0];imm=word&0xffffff;imm-=0x1000000 if imm&0x800000 else 0
  if (word>>24)&15 in (10,11) and 0x100008+off+4*imm==target:
   refs.append({'address':hex(off+0x100000),'target':hex(target),'kind':'branch'})
for target in [0x2e0d2c,0x3da3b4]:
 for off in range(0,len(code)-3,4):
  if struct.unpack_from('<I',code,off)[0]==target:refs.append({'address':hex(off+0x100000),'target':hex(target),'kind':'word_reference'})
result={'source_hash':hashlib.sha256(code).hexdigest(),'root_map':fnrows[0x2e0d2c],'executable_ranges':[[hex(a),hex(b)] for a,b in ranges],'literal_islands':[['0x002E1124','0x002E1194'],['0x002E1530','0x002E1544']],'direct_and_indirect_calls':calls,'references':refs,'heap_vtable_offsets':{'allocate':0x14,'resize':0x20,'largest_block':0x38},'minimum_receiver_extent_from_caller':0x48ec,'temporary_shader_allocation':0x128e8}
(out/'static-evidence.json').write_text(json.dumps(result,indent=2));print({'calls':len(calls),'refs':refs})
