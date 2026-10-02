// Original EU 0x002CA5E4. Clean-room command-list initialization.
// NON_MATCHING until established by the canonical project checker.
#include <stddef.h>
namespace shader2ca5e4 {
typedef unsigned Word;
struct Buffer { Word words[4]; };
struct State { Word unknown0; Buffer main; Buffer secondary; Word tail[(0x678-0x24)/4]; void* commandBuffer; void* vertexBuffer; Word physicalBase; };
struct Binary { Word signature, programWords, shaderBytes; };
struct RegisterEntry { unsigned short semantic, slot, mask, unknown; };
struct Constant { unsigned short unknown, index; Word parts[4]; };
struct Packet4 { Word words[4]; };
struct Packet6 { Word words[6]; };
struct Packet7 { Word words[7]; };
struct Packet14 { Word words[14]; };
struct Vertex { float glyph, corner, x, y; };
static_assert_(offsetof(State, main) == 4);
static_assert_(offsetof(State, secondary) == 0x14);
static_assert_(sizeof(State) == 0x684);
static_assert_(offsetof(State, vertexBuffer) == 0x67c);
static_assert_(offsetof(State, physicalBase) == 0x680);
inline Word& field(State* state, unsigned offset) { return reinterpret_cast<Word*>(state)[offset/4]; }
inline Word* words(Word address) { return reinterpret_cast<Word*>(address); }
inline const void* data(Word address) { return reinterpret_cast<const void*>(address); }
template<class T> inline const T& packet(Word address) { return *reinterpret_cast<const T*>(address); }
inline Word packetWords(Word count) {
    Word size = (count >> 7) * 132;
    if (count & 127) size += ((count & 127) + 4) & ~1u;
    return size;
}
}
extern "C" {
void __rt_memcpy(void*, const void*, unsigned);
void fn_00296048(void*, unsigned);
void fn_00298460(shader2ca5e4::Buffer*, void*, unsigned, unsigned);
void fn_002284B0(shader2ca5e4::Buffer*, const void*, unsigned);
void fn_0029848C(shader2ca5e4::Buffer*, unsigned);
unsigned nngxGetPhysicalAddr(void*);
}
#ifdef NON_MATCHING
extern "C" void fn_002CA5E4(shader2ca5e4::State* state, void* vertexBuffer,
                              void* commandBuffer, shader2ca5e4::Binary* binary,
                              unsigned unused, bool uploadVertices) {
    using namespace shader2ca5e4;
    Word* program = reinterpret_cast<Word*>(binary) + binary->programWords + 2;
    Word* shader = reinterpret_cast<Word*>(reinterpret_cast<char*>(binary) + binary->shaderBytes);
    state->vertexBuffer = vertexBuffer;
    if (uploadVertices) {
        unsigned char* initialized = reinterpret_cast<unsigned char*>(0x003f03e0);
        if (!*initialized) {
            Vertex* vertices = reinterpret_cast<Vertex*>(0x00429af0);
            for (int glyph=0; glyph<26; ++glyph) {
                const int* indices = reinterpret_cast<const int*>(0x003aef20);
                Vertex* vertex=vertices+glyph*6;
                for (int remaining=6; remaining; --remaining, ++vertex) {
                    int index=*indices++;
                    vertex->glyph=static_cast<float>(glyph);
                    vertex->corner=static_cast<float>(index);
                    vertex->x=(index & 1) ? 1.0f : 0.0f;
                    vertex->y=(static_cast<unsigned>(index)+1 < 3) ? 0.0f : -1.0f;
                }
            }
            *initialized=1;
        }
        __rt_memcpy(vertexBuffer,data(0x00429af0),0x9c0);
        fn_00296048(vertexBuffer,0x9c0);
    }
    state->commandBuffer=commandBuffer;
    unsigned size=(packetWords(program[3])+packetWords(program[5]))*4+0x320;
    fn_00298460(&state->main,commandBuffer,size,1);
    fn_00298460(&state->secondary,reinterpret_cast<char*>(state->commandBuffer)+size,0xd8,1);
    fn_002284B0(&state->main,data(0x003aef38),0x140);
    Word* code=reinterpret_cast<Word*>(reinterpret_cast<char*>(program)+program[2]);
    for (unsigned offset=0; offset<program[3];) {
        unsigned count=program[3]-offset;
        if (count>=128) count=128;
        Packet4 command=packet<Packet4>(0x003af078);
        command.words[0]=offset;
        command.words[2]=code[offset];
        command.words[3]=((count<<20)-0x100000)|0xf02cc;
        fn_002284B0(&state->main,&command,16);
        fn_002284B0(&state->main,code+offset+1,count*4-4);
        fn_0029848C(&state->main,8);
        offset+=count;
    }
    fn_002284B0(&state->main,data(0x003aed20),8);
    Word* descriptors=reinterpret_cast<Word*>(reinterpret_cast<char*>(program)+program[4]);
    for (unsigned offset=0; offset<program[5];) {
        unsigned count=program[5]-offset;
        if (count>=128) count=128;
        Packet4 command=packet<Packet4>(0x003af088);
        command.words[0]=offset;
        command.words[2]=descriptors[offset*2];
        command.words[3]=((count<<20)-0x100000)|0xf02d6;
        fn_002284B0(&state->main,&command,16);
        for (unsigned i=1; i<count; ++i) fn_002284B0(&state->main,descriptors+(offset+i)*2,4);
        fn_0029848C(&state->main,8);
        offset+=count;
    }
    Constant* constants=reinterpret_cast<Constant*>(reinterpret_cast<char*>(shader)+shader[6]);
    for (unsigned i=0; i<shader[7]; ++i) {
        Packet6 command=packet<Packet6>(0x003af098);
        Constant& c=constants[i];
        command.words[0]=c.index & 255;
        command.words[2]=((c.parts[2]>>16)&255)|(c.parts[3]<<8);
        command.words[3]=(c.parts[2]<<16)|((c.parts[1]<<8)>>16);
        command.words[4]=(c.parts[0]&0xffffff)|(c.parts[1]<<24);
        fn_002284B0(&state->main,&command,24);
    }
    unsigned address=nngxGetPhysicalAddr(state->vertexBuffer);
    unsigned aligned=address & ~15u;
    unsigned low=(address-aligned)&0x0fffffff;
    unsigned packedBase=(aligned>>4)<<1;
    {
        Packet7 command=packet<Packet7>(0x003af1a4);
        command.words[0]=packedBase;
        command.words[3]=0;
        command.words[4]=low;
        fn_002284B0(&state->main,&command,28);
    }
    fn_002284B0(&state->main,data(0x003af110),0x94);
    {
        Packet6 command=packet<Packet6>(0x003af0b0);
        command.words[4]=shader[2]|0x7fff0000;
        fn_002284B0(&state->main,&command,24);
    }
    unsigned char masks[8]={0,0,0,0,0,0,0,0};
    signed char used[7];
    union RegisterMap { Word words[7]; unsigned char bytes[28]; } mapping;
    for (unsigned i=0;i<7;++i) { used[i]=0;mapping.words[i]=0x1f1f1f1f; }
    RegisterEntry* registers=reinterpret_cast<RegisterEntry*>(reinterpret_cast<char*>(shader)+shader[10]);
    for (unsigned i=0;i<(shader[11]>7?7:shader[11]);++i) {
        RegisterEntry& entry=registers[i];
        if (entry.semantic==9) continue;
        used[entry.slot]=1;
        masks[entry.semantic]=entry.mask;
        unsigned char value=reinterpret_cast<const unsigned char*>(0x003aed28)[entry.semantic];
        unsigned char* slot=mapping.bytes+entry.slot*4;
        if (entry.mask&1) {slot[0]=value;++value;}
        if (entry.mask&2) {slot[1]=value;++value;}
        if (entry.mask&4) {slot[2]=value;++value;}
        if (entry.mask&8) slot[3]=value;
    }
    unsigned usedCount=0;
    for (unsigned i=0;i<7;++i) if (used[i]) ++usedCount;
    {
        Packet14 command=packet<Packet14>(0x003af0c8);
        command.words[0]=reinterpret_cast<unsigned short*>(shader)[9];
        command.words[2]=usedCount-1;
        command.words[8]=usedCount-1;
        command.words[10]=usedCount-1;
        command.words[12]=usedCount;
        fn_002284B0(&state->main,&command,56);
    }
    {
        Word command[2]={mapping.words[0],words(0x003aed20)[5]};
        fn_002284B0(&state->main,command,8);
    }
    fn_002284B0(&state->main,mapping.words+1,24);
    unsigned components=0;
    if (masks[0]&1) ++components;
    if (masks[0]&2) ++components;
    if (masks[0]&4) ++components;
    if (masks[0]&8) ++components;
    unsigned flags=(components>=3) | ((masks[2]!=0)<<1) | ((masks[3]!=0)<<8)
                  | ((masks[5]!=0)<<9) | ((masks[6]!=0)<<10) | ((masks[4]!=0)<<16)
                  | (((masks[7]!=0)||(masks[1]!=0))<<24);
    {
        Packet4 command=packet<Packet4>(0x003af100);
        command.words[0]=(flags&0x10700)!=0;
        command.words[2]=flags;
        fn_002284B0(&state->main,&command,16);
    }
    fn_002284B0(&state->main,data(0x003aee28),0x90);
    state->physicalBase=aligned;
    {
        Packet7 command=packet<Packet7>(0x003af1a4);
        command.words[0]=packedBase;
        command.words[3]=0;
        command.words[4]=low;
        fn_002284B0(&state->secondary,&command,28);
    }
    fn_002284B0(&state->secondary,data(0x003af110),0x94);
    fn_002284B0(&state->secondary,data(0x003aeeb8),0x28);
    field(state,0x28)=0x80000000;
    field(state,0x2c)=0xf02c0;
    reinterpret_cast<float*>(state)[0x74/4]=0.0f;
    reinterpret_cast<float*>(state)[0x78/4]=0.0f;
    reinterpret_cast<float*>(state)[0x7c/4]=0.0f;
    reinterpret_cast<float*>(state)[0x80/4]=0.0f;
    field(state,0x88)=0x80000006;
    field(state,0x8c)=0xf02c0;
    field(state,0x238)=0x80000020;
    field(state,0x23c)=0xf02c0;
    field(state,0x448)=0x80000040;
    field(state,0x44c)=0xf02c0;
}
#endif
