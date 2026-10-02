#!/usr/bin/env python3
"""Reproduce semantic actor construction evidence from an owner-provided EU executable."""
import argparse,csv,io,pathlib,subprocess,struct,collections,json,re,hashlib,functools,bisect,shutil
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--repository',type=pathlib.Path,default=pathlib.Path(__file__).resolve().parents[2])
parser.add_argument('--revision',default='e2336037d4f709acc082b3322daedadf9500564a')
parser.add_argument('--binary',type=pathlib.Path)
arguments=parser.parse_args()
repository=arguments.repository.resolve()
binary_path=arguments.binary or repository/'data/ver/eu/code.bin'
revision=arguments.revision
map_text=subprocess.check_output(['git','-C',str(repository),'show',revision+':data/ver/eu/map.csv'],text=True)
raw_rows=list(csv.reader(io.StringIO(map_text)))[1:]
rows=[{'start':int(r[0],16),'pool':int(r[1],16) if r[1].strip() else int(r[2],16),'end':int(r[2],16),'kind':r[5].strip(),'symbol':r[6].strip()} for r in raw_rows]
functions={r['start']:r for r in rows if 'f' in r['kind']}
data_rows={r['start']:r for r in rows if 'f' not in r['kind']}
data_starts=sorted(data_rows)
code=binary_path.read_bytes()
assert hashlib.sha256(code).hexdigest()=='e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64'
def word(address):
 return struct.unpack_from('<I',code,address-0x100000)[0]
def text_at(address):
 offset=address-0x100000
 if not 0<=offset<len(code):return None
 end=code.find(b'\x00',offset,min(offset+96,len(code)))
 if end<0:return None
 value=code[offset:end]
 return value.decode() if re.fullmatch(rb'[A-Za-z][A-Za-z0-9_:]{1,90}',value) else None
def direct_calls(address):
 result=[]
 for pc in range(address,functions[address]['pool'],4):
  value=word(pc)
  if value&0x0f000000==0x0b000000:
   displacement=value&0xffffff
   if displacement&0x800000:displacement-=1<<24
   target=(pc+8+displacement*4)&0xffffffff
   result.append({'site':pc,'target':target,'symbol':functions.get(target,{}).get('symbol','')})
 return result
entries=[]
for address in range(0x3b99f0,0x3ba0f8,8):
 name,creator=struct.unpack_from('<II',code,address-0x100000)
 entries.append({'name':text_at(name),'creator':creator,'calls':direct_calls(creator)})
entries.sort(key=lambda entry:entry['name'])

def branch_target(pc,value):
 displacement=value&0xffffff
 if displacement&0x800000:displacement-=1<<24
 return (pc+8+displacement*4)&0xffffffff
def transfer_sites(address):
 result=[]
 row=functions[address]
 for pc in range(address,row['pool'],4):
  value=word(pc)
  if value&0x0e000000==0x0a000000:
   target=branch_target(pc,value)
   if value&0x01000000 or not row['start']<=target<row['pool']:
    result.append({'site':pc,'target':target,'call':bool(value&0x01000000),'condition':value>>28})
 return result
for entry in entries:
 transfers=transfer_sites(entry['creator'])
 entry['constructor_transfers']=[site for site in transfers if site['target']!=0x2932b0]
def value_operand(value,registers,pc):
 if value&0x02000000:
  immediate=value&255;rotate=((value>>8)&15)*2
  return ('integer',((immediate>>rotate)|(immediate<<(32-rotate)))&0xffffffff)
 if value&0x00000ff0:return None
 register=value&15
 return ('integer',pc+8) if register==15 else registers.get(register)
@functools.lru_cache(maxsize=None)
def table_identity(address):
 if not isinstance(address,int) or not 0x3a2008<=address<0x3e0e50:return None
 row=data_rows.get(address-8)
 if row and row['end']<=address:row=None
 row=row or data_rows.get(address)
 if not row:
  index=bisect.bisect_right(data_starts,address)-1
  candidate=data_rows[data_starts[index]] if index>=0 else None
  row=candidate if candidate and candidate['start']<address<candidate['end'] else None
 if not row or row['end']<=address or not 0x3a2000<=row['start']<0x3e0e50:return None
 references=[word(a) for a in range(address,row['end']-3,4)]
 count=sum(target in functions for target in references)
 if word(address-8)!=0 or word(address-4)!=0 or count<3:return None
 return {'table_start':address-8,'address_point':address,'table_end':row['end'],'map_row_start':row['start'],'map_symbol':row['symbol'],'function_reference_count':count,'word_count':len(references),'primary_header_confirmed':True}
preserves_receiver=set()
def inspect_constructor(address):
 registers={0:('object',0)}
 events=[];unknown=[];branches=[];call_sites=[];return_values=[]
 row=functions[address]
 for pc in range(address,row['pool'],4):
  value=word(pc);condition=value>>28
  if value&0x0e000000==0x0a000000:
   target=branch_target(pc,value);call=bool(value&0x01000000)
   if call or not address<=target<row['pool']:
    call_sites.append({'site':pc,'target':target,'receiver':registers.get(0),'tail':not call})
    receiver=registers.get(0)
    for register in (0,1,2,3,12):registers.pop(register,None)
    if target in preserves_receiver and receiver:registers[0]=receiver
    if not call:return_values.append(registers.get(0))
   else:branches.append({'site':pc,'target':target,'condition':condition})
   continue
  if value&0x0ffffff0 in (0x012fff10,0x012fff30):
   if value&0x0ffffff0==0x012fff30:
    for register in (0,1,2,3,12):registers.pop(register,None)
   elif value&15==14:return_values.append(registers.get(0))
   continue
  if value&0x0e000000==0x08000000:
   base=(value>>16)&15;base_value=registers.get(base)
   selected=[register for register in range(16) if value&(1<<register)]
   if base_value and base_value[0]=='object' and not value&0x00100000:
    initial=(4 if value&0x01000000 else 0) if value&0x00800000 else (-4*len(selected)+(0 if value&0x01000000 else 4))
    for index,register in enumerate(selected):
     stored=registers.get(register)
     if stored and stored[0]=='integer':
      identity=table_identity(stored[1])
      if identity:events.append({'site':pc,'object_offset':base_value[1]+initial+index*4,'condition':condition,**identity})
   if value&0x00200000 and base_value:
    registers[base]=(base_value[0],base_value[1]+4*len(selected)*(1 if value&0x00800000 else -1))
   if value&0x00100000:
    if value&0x8000:return_values.append(registers.get(0))
    for register in range(16):
     if value&(1<<register):registers.pop(register,None)
   continue
  if value&0x0c000000==0x04000000:
   destination=(value>>12)&15;base=(value>>16)&15;load=bool(value&0x00100000)
   if value&0x02000000:
    if load:registers.pop(destination,None)
    continue
   displacement=value&4095
   if not value&0x00800000:displacement=-displacement
   base_value=('integer',pc+8) if base==15 else registers.get(base)
   preindexed=bool(value&0x01000000);offset=displacement if preindexed else 0
   if load:
    if base_value and base_value[0]=='integer' and 0x100000<=base_value[1]+offset<=0x100000+len(code)-4:
     registers[destination]=('integer',word(base_value[1]+offset))
    else:registers.pop(destination,None)
   else:
    stored=registers.get(destination)
    if base_value and base_value[0]=='object' and stored and stored[0]=='integer':
     identity=table_identity(stored[1])
     if identity:events.append({'site':pc,'object_offset':base_value[1]+offset,'condition':condition,**identity})
   if value&0x00200000 or not preindexed:
    registers[base]=(base_value[0],base_value[1]+displacement) if base_value else None
   continue
  if value&0x0e1000f0==0x000000f0:
   base=(value>>16)&15;destination=(value>>12)&15;base_value=registers.get(base)
   displacement=(((value>>4)&240)|(value&15)) if value&0x00400000 else None
   if displacement is not None:
    if not value&0x00800000:displacement=-displacement
    offset=displacement if value&0x01000000 else 0
    for index in (0,1):
     stored=registers.get(destination+index)
     if base_value and base_value[0]=='object' and stored and stored[0]=='integer':
      identity=table_identity(stored[1])
      if identity:events.append({'site':pc,'object_offset':base_value[1]+offset+index*4,'condition':condition,**identity})
    if value&0x00200000 or not value&0x01000000:
     registers[base]=(base_value[0],base_value[1]+displacement) if base_value else None
   continue
  if value&0x0c000000==0:
   if value&0x0f0000f0 in (0x00000090,0x01000090):continue
   operation=(value>>21)&15;destination=(value>>12)&15;base=(value>>16)&15
   if operation in (8,9,10,11):continue
   right=value_operand(value,registers,pc)
   left=('integer',pc+8) if base==15 else registers.get(base)
   result=None
   if operation==13:result=right
   elif operation in (2,4) and left and right and right[0]=='integer':
    result=(left[0],(left[1]+(right[1] if operation==4 else -right[1]))&0xffffffff)
   elif operation==14 and left and right and left[0]==right[0]=='integer':result=('integer',left[1]&~right[1])
   if result:registers[destination]=result
   else:registers.pop(destination,None)
 return {'events':events,'branches':branches,'calls':call_sites,'return_values':return_values}
for pass_index in range(8):
 for function_address in list(functions):
  if 0x110000<=function_address<=0x330000 and functions[function_address]['end']-function_address<=2048:
   analysis=inspect_constructor(function_address)
   if analysis['return_values'] and all(value==('object',0) for value in analysis['return_values']) and not analysis['branches']:preserves_receiver.add(function_address)
for entry in entries:
 entry['construction_evidence']=[{'constructor':site['target'],'transfer':site,'analysis':inspect_constructor(site['target']) if site['target'] in functions else None} for site in entry['constructor_transfers']]

legacy_rows=sorted((row for row in rows if row['kind']=='f'),key=lambda row:row['start'])
legacy_starts=[row['start'] for row in legacy_rows]
legacy_functions={row['start']:row for row in legacy_rows}
legacy_text_end=legacy_rows[-1]['end']
def legacy_function(address):
 index=bisect.bisect_right(legacy_starts,address)-1
 return legacy_rows[index] if index>=0 and legacy_rows[index]['start']<=address<legacy_rows[index]['end'] else None
def legacy_string(address):
 offset=address-0x100000
 if not 0<=offset<len(code):return None
 end=code.find(b'\0',offset)
 raw=code[offset:end]
 return raw.decode() if 0<len(raw)<48 and all(32<character<127 for character in raw) else None
def legacy_branch_calls(row):
 return [branch_target(pc,word(pc)) for pc in range(row['start'],row['end'],4) if word(pc)&0x0f000000==0x0b000000]
def legacy_tables(row):
 found=[]
 for pc in range(row['start'],row['end'],4):
  value=word(pc)
  if legacy_text_end<=value<0x100000+len(code)-12 and value%4==0 and all(word(value+4*index) in legacy_functions for index in range(3)):
   found.append({'address_point':value,'candidate_source_function':row['start'],'literal_reference_site':pc})
 return found
def registry_kind(address):
 for low,high,name in [(0x3b99f0,0x3ba0f8,'actor'),(0x3b896c,0x3b8a8c,'area'),(0x3a8944,0x3a89bc,'camera'),(0x3b6b68,0x3b6bd8,'demo'),(0x3b78f4,0x3b796c,'start_event')]:
  if low<=address<high:return name
 return 'other'
legacy_entries=[]
for address in range(0x3a8000,0x3bb000,4):
 name_address,creator=word(address),word(address+4);name=legacy_string(name_address)
 if name and creator in legacy_functions:
  initial=legacy_function(creator)
  candidates=[initial]+[legacy_function(target) for target in legacy_branch_calls(initial) if legacy_function(target)]
  for row in list(candidates[1:]):candidates +=[legacy_function(target) for target in legacy_branch_calls(row) if legacy_function(target)]
  table_candidates=[]
  for row in candidates[:12]:table_candidates +=legacy_tables(row)
  legacy_entries.append({'registry_name':name,'registry_kind':registry_kind(address),'creator_address':creator,'candidate_tables':table_candidates,'chosen_heuristic_table':table_candidates[0]['address_point'] if table_candidates else None})
legacy_entries.sort(key=lambda entry:(entry['registry_kind'],entry['registry_name'],entry['creator_address']))

disassembler=shutil.which('arm-none-eabi-objdump')
if not disassembler:raise SystemExit('arm-none-eabi-objdump is required')
disassembler_version=subprocess.check_output([disassembler,'--version'],text=True).splitlines()[0]
private_decoding=subprocess.check_output([disassembler,'-D','-b','binary','-m','arm','-EL','--adjust-vma=0x100000','--start-address=0x110000','--stop-address=0x398e00','--no-show-raw-insn',str(binary_path)],text=True)
decoded={}
for line in private_decoding.splitlines():
 match=re.match(r'\s*([0-9a-f]+):\s+(\S+)\s*(.*)',line)
 if match:decoded[int(match[1],16)]=(match[2],match[3])
del private_decoding
def operand_text(address):
 return decoded[address][1].split('@')[0].strip()
store_forms=collections.Counter()
for entry in entries:
 assert len(entry['constructor_transfers'])==1
 evidence=entry['construction_evidence'][0]
 stores=[event for event in evidence['analysis']['events'] if event['object_offset']==0]
 assert len(stores)==1
 event=stores[0];mnemonic,operands=decoded[event['site']]
 assert mnemonic in ('str','stm','stmia','strd'),(entry['name'],mnemonic)
 store_forms[mnemonic]+=1
 transfer=entry['constructor_transfers'][0]
 assert decoded[transfer['site']][0]==('bl' if transfer['call'] else 'b')
 assert int(operand_text(transfer['site']).split()[0],16)==transfer['target']
 assert not any(word(pc)>>28!=14 for pc in range(evidence['constructor'],event['site']+4,4))
 assert not any(branch['site']<event['site'] for branch in evidence['analysis']['branches'])
def address_text(address):return f'0x{address:08X}'
symbol_names=sorted({row['symbol'] for row in rows if row['symbol'].startswith('_Z')})
demangled_text=subprocess.check_output(['c++filt'],input='\n'.join(symbol_names),text=True).splitlines()
demangled=dict(zip(symbol_names,demangled_text))
source_registry=subprocess.check_output(['git','-C',str(repository),'show',revision+':lib/al/src/Factory/alActorFactory.cpp'],text=True)
source_bindings=dict(re.findall(r'\{\s*"([^"]+)",\s*createActorFunction<([^>]+)>',source_registry))
def constructor_identity(address):
 symbol=functions[address]['symbol']
 if re.search(r'C[12]E',symbol):
  text=demangled.get(symbol,'').split('(',1)[0]
  if '::' in text:return text.rsplit('::',1)[0]
 return None
def creator_evidence(entry):
 creator=entry['creator'];transfer=entry['constructor_transfers'][0]
 allocator=[site for site in transfer_sites(creator) if site['target']==0x2932b0]
 assert len(allocator)==1
 allocation_call=allocator[0]
 allocation_size=None;allocation_load_site=None
 for pc in range(creator,allocation_call['site'],4):
  value=word(pc)
  if value&0x0fe0f000==0x03a00000:
   allocation_size=value_operand(value,{},pc)[1];allocation_load_site=pc
 assert allocation_size is not None
 internal_branches=[{'site':pc,'target':branch_target(pc,word(pc)),'condition':word(pc)>>28} for pc in range(allocation_call['site']+4,functions[creator]['pool'],4) if word(pc)&0x0f000000==0x0a000000 and creator<=branch_target(pc,word(pc))<functions[creator]['pool']]
 assert len(internal_branches)==1 and internal_branches[0]['condition']==0
 guard=internal_branches[0]
 assert guard['site']<transfer['site']<guard['target']
 assert decoded[guard['site']][0]=='beq'
 comparison_site=guard['site']-4
 assert decoded[comparison_site][0]=='cmp' and operand_text(comparison_site)=='r0, #0'
 for pc in range(allocation_call['site']+4,transfer['site'],4):
  value=word(pc)
  if value&0x0c000000==0 and (value>>12)&15==0:assert (value>>21)&15 in (8,9,10,11)
  if value&0x0c000000==0x04000000 and value&0x00100000:assert (value>>12)&15!=0
  if value&0x0e000000==0x08000000 and value&0x00100000:assert not value&1
 return {'allocation_function':address_text(allocation_call['target']),'allocation_call_site':address_text(allocation_call['site']),'allocation_size_bytes':allocation_size,'allocation_failure_guard_site':address_text(guard['site']),'construction_transfer_site':address_text(transfer['site']),'construction_transfer_kind':'call' if transfer['call'] else 'tail_call','allocated_object_passed_at_offset':0,'receiver_proof':'No r0 write or intervening call between the allocator result and the constructor transfer. The zero-result branch skips construction.'}
actor_records=[]
for entry in entries:
 evidence=entry['construction_evidence'][0];constructor=evidence['constructor'];analysis=evidence['analysis']
 store=next(event for event in analysis['events'] if event['object_offset']==0)
 primary=store['address_point'];map_start=store['map_row_start'];map_end=store['table_end']
 identity_evidence=[]
 if entry['name'] in source_bindings:identity_evidence.append({'source':'committed_creator_template_binding','cpp_class_name':source_bindings[entry['name']]})
 accepted_constructor=constructor_identity(constructor)
 if accepted_constructor:identity_evidence.append({'source':'accepted_map_constructor_symbol','cpp_class_name':accepted_constructor,'symbol':functions[constructor]['symbol']})
 table_cpp_name=None
 if map_start==primary-8 and store['map_symbol'].startswith('_ZTV'):
  table_cpp_name=demangled.get(store['map_symbol'],'').removeprefix('vtable for ')
  identity_evidence.append({'source':'accepted_map_table_symbol_at_matching_primary_header','cpp_class_name':table_cpp_name,'symbol':store['map_symbol']})
 identities=sorted({identity['cpp_class_name'] for identity in identity_evidence})
 assert len(identities)<=1,(entry['name'],identities)
 loads=[]
 for pc in range(constructor,store['site']+4,4):
  value=word(pc)
  if value&0x0e5f0000==0x041f0000:
   literal_site=pc+8+((value&4095) if value&0x800000 else -(value&4095))
   if word(literal_site)==primary:loads.append(pc)
 assert len(loads)==1,(entry['name'],loads)
 dependencies=[call for call in analysis['calls'] if call['site']<store['site'] and call['receiver'] and call['receiver'][0]=='object']
 assert all(call['target'] in preserves_receiver for call in dependencies)
 later_calls=[call for call in analysis['calls'] if call['site']>store['site'] and call['receiver']==('object',0)]
 actor_records.append({
  'registry_name':entry['name'],
  'registry_to_primary_table_confidence':'high_direct_constructor_store',
  'accepted_cpp_class_name':identities[0] if identities else None,
  'cpp_identity_evidence':identity_evidence,
  'original_cpp_class_spelling_proven':False,
  'creator_address':address_text(entry['creator']),
  'creator_map_symbol':functions[entry['creator']]['symbol'] or None,
  'constructor_address':address_text(constructor),
  'constructor_map_symbol':functions[constructor]['symbol'] or None,
  'construction_edge':creator_evidence(entry),
  'primary_vtable':{
   'primary_address_point':address_text(primary),
   'primary_header_address':address_text(primary-8),
   'installation_site':address_text(store['site']),
   'address_load_site':address_text(loads[0]),
   'object_offset_bytes':0,
   'unconditional_installation':True,
   'installation_precedes_all_constructor_branches':True,
   'primary_header_zero_checks_pass':True,
   'containing_map_row_start':address_text(map_start),
   'containing_map_row_end':address_text(map_end),
   'containing_map_symbol':store['map_symbol'] or None,
   'map_row_primary_address_point_offset_bytes':primary-map_start,
   'map_row_symbol_applies_to_this_primary_header':bool(table_cpp_name),
   'complete_vtable_group_boundary_claimed':False
  },
  'receiver_return_dependencies':[{'call_site':address_text(call['site']),'function_address':address_text(call['target']),'receiver_offset_bytes':call['receiver'][1],'return_offset_bytes':call['receiver'][1]} for call in dependencies],
  'constructor_internal_branches':[{'branch_site':address_text(branch['site']),'destination':address_text(branch['target']),'precedes_primary_installation':False} for branch in analysis['branches']],
  'later_calls_with_same_receiver':[{'call_site':address_text(call['site']),'function_address':address_text(call['target']),'accepted_symbol':functions.get(call['target'],{}).get('symbol') or None} for call in later_calls],
  'method_ownership_inferred':False
 })
actor_by_name={record['registry_name']:record for record in actor_records}
legacy_records=[]
for entry in legacy_entries:
 unique_candidates={candidate['address_point'] for candidate in entry['candidate_tables']}
 chosen=entry['chosen_heuristic_table']
 verified=actor_by_name.get(entry['registry_name']) if entry['registry_kind']=='actor' else None
 agrees=bool(verified and chosen and address_text(chosen)==verified['primary_vtable']['primary_address_point'])
 legacy_records.append({
  'registry_kind':entry['registry_kind'],
  'registry_name':entry['registry_name'],
  'creator_address':address_text(entry['creator_address']),
  'legacy_first_candidate_address_point':address_text(chosen) if chosen else None,
  'legacy_candidate_address_points':sorted(address_text(address) for address in unique_candidates),
  'legacy_candidate_count':len(entry['candidate_tables']),
  'legacy_status':'literal_pool_candidate_only' if chosen else 'no_candidate_found',
  'confidence':'historical_heuristic',
  'independently_verified_actor_primary_table':agrees,
  'corrected_actor_primary_address_point':verified['primary_vtable']['primary_address_point'] if verified else None,
  'method_ownership_inferred':False
 })

print(json.dumps({"actor_records":actor_records,"legacy_records":legacy_records},indent=2))
