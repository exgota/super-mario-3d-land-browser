from pathlib import Path
import struct,json,hashlib
from replay import machine,run,V,BIN,OUT
from unicorn import UC_HOOK_MEM_WRITE
pairs=[];a=0x3a421c
while True:
 index,value=struct.unpack_from('<II',BIN,a-0x100000);a+=8
 if index==0xffffffff:break
 pairs.append((index,value))
assert len(pairs)==189 and sorted(i for i,v in pairs)==list(range(189))
expected=bytearray(756)
for i,v in pairs:struct.pack_into('<I',expected,4*i,v)
def bits(a):
 out=[0]*6;count=0
 while True:
  i=struct.unpack_from('<I',BIN,a-0x100000)[0];a+=4
  if i==189:break
  assert 0<=i<189;out[i>>5]|=1<<(i&31);count+=1
 return out,count
p0,c0=bits(0x3e2e50);p1,c1=bits(0x3e2eb0);p2,c2=bits(0x3e2ec8);p3,c3=bits(0x3e2ee0)
expected+=struct.pack('<18I',*(p0+[x|y for x,y in zip(p1,p2)]+p3))
results=[]
for fill in [0,0x5a,0xa6,0xff]:
 u,_=machine(False);u.mem_write(0x420f4c,bytes([fill])*828);u.mem_write(0x421288,b'guard___');writes=[]
 def hook(m,access,address,size,value,data):
  if 0x420f4c<=address<0x421300:writes.append((address,size))
 u.hook_add(UC_HOOK_MEM_WRITE,hook)
 assert run(u,0x1064e4,V)=={'returned':True}
 actual=bytes(u.mem_read(0x420f4c,828));assert actual==expected;assert bytes(u.mem_read(0x421288,8))==b'guard___'
 covered=set(x for address,size in writes for x in range(address,address+size))
 assert covered==set(range(0x420f4c,0x421288))
 results.append({'initial_fill':fill,'writes':len(writes),'unique_written_bytes':len(covered),'minimum_write':hex(min(covered)),'maximum_write_exclusive':hex(max(covered)+1),'guard_preserved':True,'expected_output_sha256':hashlib.sha256(actual).hexdigest()})
out={'producer':'_shm_initializeShaderManager 001064E4..00106B74','lookup_source_start':'003A421C','lookup_source_sentinel':'003A4804','lookup_source_end':'003A480C','lookup_entries':len(pairs),'unique_indices':189,'minimum_index':0,'maximum_index':188,'lookup_range':['00420F4C','00421240'],'mask_ranges':[['00421240','00421258'],['00421258','00421270'],['00421270','00421288']],'list_lengths':[c0,c1,c2,c3],'dynamic_initializations':results,'qualification':'Observed producer and independent consumers establish exact accessed ranges. Original source-level object names and ownership remain unknown; current canonical map has no row. Main-owned separate metadata review only.'}
(OUT/'metadata.json').write_text(json.dumps(out,indent=2));print(json.dumps(out,indent=2))
