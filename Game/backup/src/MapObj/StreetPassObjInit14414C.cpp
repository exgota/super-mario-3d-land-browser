// StreetPassObj initialization reconstructed from the EU executable.
// Private observed ABI prefixes only; no shared class layout is changed.
#include <stddef.h>
#include <new>

namespace dot14414c
{
struct NameRef { const void* dispatch; const char* text; NameRef(const char*); };
struct Iter { const void* data; const void* node; Iter(); explicit Iter(const void*); int size() const; bool iter(Iter*, int) const; bool string(const char**, const char*) const; bool integer(int*, const char*) const; };
struct InitInfo { unsigned int words[6]; InitInfo(); };
struct Factory { void* resource; void* names; Factory(); };
struct Vec3 { float x, y, z; };
struct Quat { float x, y, z, w; };
struct Actor { void** dispatch; unsigned char prefix04[0x5c]; };
struct Entry { const char* type; int reward; Entry() : type(0), reward(0) {} };
struct Array { int size, capacity; void** buffer; Array():size(0),capacity(0),buffer(0){} };
template<class T> struct Pool : Array {
    void* head; void* storage;
    Pool():head(0),storage(0){}
    void allocate(int count);
    T* acquire();
    T* at(unsigned int index) const { return (unsigned int)size > index ? (T*)buffer[index] : 0; }
};
struct StreetPassObj : Actor {
    Pool<Pool<Entry> >* table;
    Array* rewards;
    unsigned int selected;
    unsigned char enabled;
    Actor* indicator;
};
struct Tower : Actor { void* unknown60; void* parts; int count; unsigned char rest6c[0x38]; };
typedef void (StreetPassObj::*Method)();
struct Callback { const void* dispatch; StreetPassObj* actor; Method method; Callback(StreetPassObj*, Method); };
struct SavedState { unsigned char prefix[0x60]; signed char selected; };
typedef Actor* (*Creator)(const char*);
}
using namespace dot14414c;
extern "C" {
extern const unsigned int _ZTVN4sead14SafeStringBaseIcEE[];
extern const Method dat_003BF430[2];
extern const unsigned int dat_003D5B9C[];
void _ZN2al24initActorWithArchiveNameEPNS_9LiveActorERKNS_13ActorInitInfoERKN4sead14SafeStringBaseIcEEPKc(Actor*,const InitInfo*,const NameRef*,const char*);
void* _ZnwjRKSt9nothrow_t(unsigned int, const void*);
void* _ZnajPN4sead4HeapEi(unsigned int,void*,int);
void fn_0026AC60(Array*,int,void*,int);
SavedState* fn_0032AEFC();
void* _ZN2al20findOrCreateResourceERKN4sead14SafeStringBaseIcEE(const NameRef*);
const void* _ZNK2al8Resource7getBymlERKN4sead14SafeStringBaseIcEE(void*,const NameRef*);
void _ZN2al9ByamlIterC1EPKh(Iter*,const void*);
void _ZN2al9ByamlIterC1Ev(Iter*);
int _ZNK2al9ByamlIter7getSizeEv(const Iter*);
bool _ZNK2al9ByamlIter17tryGetIterByIndexEPS0_i(const Iter*,Iter*,int);
bool _ZNK2al9ByamlIter17tryGetStringByKeyEPPKcS2_(const Iter*,const char**,const char*);
bool _ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc(const Iter*,int*,const char*);
void _ZN2al12ActorFactoryC1Ev(Factory*);
void _ZN4sead12PtrArrayImpl9setBufferEiPv(Array*,int,void*);
int _ZN2al16calcLinkChildNumERKNS_13ActorInitInfoE(const InitInfo*);
void _ZN2al19getLinksInfoByIndexEPNS_9ByamlIterERKNS_13ActorInitInfoEi(Iter*,const InitInfo*,int);
void _ZN2al13ActorInitInfoC1Ev(InitInfo*);
void _ZN2al17initActorInitInfoEPNS_13ActorInitInfoEPKNS_9ByamlIterERKS0_(InitInfo*,const Iter*,const InitInfo*);
bool _ZN2al12isObjectNameERKNS_9ByamlIterEPKc(const Iter*,const char*);
bool _ZN2al13isEqualStringEPKcS1_(const char*,const char*);
Actor* fn_0012F204(void*,const NameRef*);
Actor* fn_0026C290(void*,const NameRef*);
Actor* fn_0016743C(void*,const NameRef*);
Actor* fn_001B88B8(void*,int);
void _ZN2al32initCreateActorWithPlacementInfoEPNS_9LiveActorERKNS_13ActorInitInfoE(Actor*,const InitInfo*);
Actor* fn_00326A04(Actor*,int);
void fn_0016F8A4(Actor*);
const Vec3* _ZN2al10getGravityEPKNS_9LiveActorE(Actor*);
const Vec3* _ZN2al8getTransEPKNS_9LiveActorE(Actor*);
const Vec3* fn_0026CCD0();
void _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(Vec3*,const Vec3*,const Vec3*);
void fn_002702EC(Quat*,const Vec3*,const Vec3*);
void fn_00271028(Actor*,const Quat*);
Creator _ZNK2al12ActorFactory10getCreatorEPKc(const Factory*,const char*);
const char* _ZN2al23getLinksActorObjectNameERKNS_13ActorInitInfoEi(const InitInfo*,int);
void fn_0027A9D4(Actor*,const InitInfo*,int);
int fn_00166F3C(Actor*);
Actor* fn_0016742C(Actor*,int);
int fn_00327E90();
void fn_0027B51C(Actor*,const InitInfo*,int);
void fn_0011C224(Actor*,const InitInfo*);
void fn_00277E5C(Actor*,const InitInfo*,int);
bool fn_00280474(void*,const Callback*,const Callback*);
}
namespace dot14414c {
inline NameRef::NameRef(const char* value): dispatch(_ZTVN4sead14SafeStringBaseIcEE+2),text(value) {}
inline Iter::Iter() { _ZN2al9ByamlIterC1Ev(this); }
inline Iter::Iter(const void* value) { _ZN2al9ByamlIterC1EPKh(this,value); }
inline int Iter::size() const { return _ZNK2al9ByamlIter7getSizeEv(this); }
inline bool Iter::iter(Iter* out,int index)const {return _ZNK2al9ByamlIter17tryGetIterByIndexEPS0_i(this,out,index);}
inline bool Iter::string(const char** out,const char* key)const{return _ZNK2al9ByamlIter17tryGetStringByKeyEPPKcS2_(this,out,key);}
inline bool Iter::integer(int* out,const char* key)const{return _ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc(this,out,key);}
inline InitInfo::InitInfo(){_ZN2al13ActorInitInfoC1Ev(this);}
inline Factory::Factory(){_ZN2al12ActorFactoryC1Ev(this);}
inline Callback::Callback(StreetPassObj* owner, Method target): dispatch(dat_003D5B9C),actor(owner),method(target) {}
inline void append(Array* a,void* p) { if(a->size<a->capacity) { a->buffer[a->size]=p; ++a->size; } }
template<class T> inline void Pool<T>::allocate(int count) {
    if(count<=0) return;
    void* allocation=_ZnajPN4sead4HeapEi(count*(sizeof(T)+sizeof(T*)),0,4);
    if(!allocation) return;
    head=allocation;
    unsigned int stride=sizeof(T)/4;
    unsigned int* words=(unsigned int*)allocation;
    for(int i=0;i<count-1;++i) words[i*stride]=(unsigned int)(words+(i+1)*stride);
    words[(count-1)*stride]=0;
    storage=allocation;
    _ZN4sead12PtrArrayImpl9setBufferEiPv(this,count,(char*)allocation+count*sizeof(T));
}
template<class T> inline T* Pool<T>::acquire() {
    if(size>=capacity) return 0;
    T* value=(T*)head;
    if(value) {
        head=*(void**)value;
        new (value) T;
    }
    append(this,value);
    return value;
}
inline int towerSize(Tower* tower) { return tower->parts ? *(int*)((char*)tower->parts+0x28)+1 : 0; }
inline Actor* rewardAt(Array* a,unsigned int i){return (unsigned int)a->size>i?(Actor*)a->buffer[i]:0;}
}

// NonMatching proposal: whole-root caller wiring has bounded replay evidence.
#ifdef NON_MATCHING
extern "C" void fn_0014414C(StreetPassObj* self,const InitInfo* info)
{
    _ZN2al24initActorWithArchiveNameEPNS_9LiveActorERKNS_13ActorInitInfoERKN4sead14SafeStringBaseIcEEPKc(self,info,&NameRef("StreetPassObj"),0);
    self->rewards=new Array;
    fn_0026AC60(self->rewards,10,0,4);
    self->selected=fn_0032AEFC()->selected;
    void* resource=_ZN2al20findOrCreateResourceERKN4sead14SafeStringBaseIcEE(&NameRef("ObjectData/StreetPassObj"));
    Iter data(_ZNK2al8Resource7getBymlERKN4sead14SafeStringBaseIcEE(resource,&NameRef("ObjectList")));
    Factory factory;
    self->table=new Pool<Pool<Entry> >;
    self->table->allocate(data.size());
    for(int i=0;i<data.size();++i) {
        Iter group;
        data.iter(&group,i);
        Pool<Entry>* row=self->table->acquire();
        row->allocate(group.size());
        for(int j=0;j<group.size();++j) {
            Iter item;
            group.iter(&item,j);
            Entry* entry=row->acquire();
            entry->type=0;
            entry->reward=0;
            item.string(&entry->type,"Type");
            item.integer(&entry->reward,"Reward");
        }
    }
    for(int i=0;i<_ZN2al16calcLinkChildNumERKNS_13ActorInitInfoE(info);++i) {
        Iter placement;
        _ZN2al19getLinksInfoByIndexEPNS_9ByamlIterERKNS_13ActorInitInfoEi(&placement,info,i);
        InitInfo child;
        _ZN2al17initActorInitInfoEPNS_13ActorInitInfoEPKNS_9ByamlIterERKS0_(&child,&placement,info);
        int column;
        if(_ZN2al12isObjectNameERKNS_9ByamlIterEPKc(&placement,"StreetPassObj1"))column=0;
        else if(_ZN2al12isObjectNameERKNS_9ByamlIterEPKc(&placement,"StreetPassObj2"))column=1;
        else if(_ZN2al12isObjectNameERKNS_9ByamlIterEPKc(&placement,"StreetPassObj3"))column=2;
        else if(_ZN2al12isObjectNameERKNS_9ByamlIterEPKc(&placement,"StreetPassObj4"))column=3;
        else continue;
        const char* type=self->table->at(self->selected)->at(column)->type;
        int reward=self->table->at(self->selected)->at(column)->reward;
        if(!type || _ZN2al13isEqualStringEPKcS1_(type,""))continue;
        if(_ZN2al13isEqualStringEPKcS1_(type,"KuriboTower")) {
            void* storage=_ZnwjRKSt9nothrow_t(0xa4,0);
            Tower* tower=storage?(Tower*)fn_0012F204(storage,&NameRef("KuriboTower")):0;
            tower->count=2;
            _ZN2al32initCreateActorWithPlacementInfoEPNS_9LiveActorERKNS_13ActorInitInfoE(tower,&child);
            for(int j=0;j<towerSize(tower);++j)append(self->rewards,fn_00326A04(tower,j));
        } else if(_ZN2al13isEqualStringEPKcS1_(type,"KuriboTailSearch")) {
            void* storage=_ZnwjRKSt9nothrow_t(0x78,0);
            Actor* actor=storage?fn_0026C290(storage,&NameRef("KuriboTailSearch")):0;
            _ZN2al32initCreateActorWithPlacementInfoEPNS_9LiveActorERKNS_13ActorInitInfoE(actor,&child);
            fn_0016F8A4(actor);
            const Vec3* gravity=_ZN2al10getGravityEPKNS_9LiveActorE(actor);
            Vec3 up={-gravity->x,-gravity->y,-gravity->z};
            const Vec3* trans=_ZN2al8getTransEPKNS_9LiveActorE(actor);
            const Vec3* player=fn_0026CCD0();
            Vec3 direction;
            _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(&direction,trans,player);
            Vec3 copy=direction;
            Quat rotation;
            fn_002702EC(&rotation,&up,&copy);
            fn_00271028(actor,&rotation);
            append(self->rewards,actor);
        } else {
            Creator creator=_ZNK2al12ActorFactory10getCreatorEPKc(&factory,type);
            if(!creator) continue;
            Actor* actor=0;
            for(int j=0;j<_ZN2al16calcLinkChildNumERKNS_13ActorInitInfoE(&child);++j) {
                if(!_ZN2al13isEqualStringEPKcS1_(type,_ZN2al23getLinksActorObjectNameERKNS_13ActorInitInfoEi(&child,j)))continue;
                if(_ZN2al13isEqualStringEPKcS1_(type,"TentenGenerator")) {
                    void* storage=_ZnwjRKSt9nothrow_t(0x84,0);
                    actor=storage?fn_0016743C(storage,&NameRef("TentenGenerator")):0;
                    fn_0027A9D4(actor,&child,j);
                    for(int k=0;k<fn_00166F3C(actor);++k)append(self->rewards,fn_0016742C(actor,k));
                }else {
                    actor=creator(type);
                    fn_0027A9D4(actor,&child,j);
                }
                break;
            }
            if(!actor) {
                actor=creator(type);
                _ZN2al32initCreateActorWithPlacementInfoEPNS_9LiveActorERKNS_13ActorInitInfoE(actor,&child);
            }
            if(reward)append(self->rewards,actor);
        }
    }
    if(self->rewards->size==0) {
        ((void(*)(StreetPassObj*))self->dispatch[6])(self);
        return;
    }
    self->enabled=fn_00327E90();
    fn_0027B51C(self,info,15);
    fn_0011C224(self,info);
    void* storage=_ZnwjRKSt9nothrow_t(0x78,self);
    self->indicator=storage?fn_001B88B8(storage,15):0;
    ((void(*)(Actor*,const InitInfo*))self->indicator->dispatch[1])(self->indicator,info);
    if(self->enabled) for(int i=0;i<self->rewards->size;++i)fn_00277E5C(rewardAt(self->rewards,i),info,1);
    Callback appear(self,dat_003BF430[0]);
    Callback dead(self,dat_003BF430[1]);
    bool registered=fn_00280474((char*)self+0xc,&appear,&dead);
    if(registered)((void(*)(StreetPassObj*))self->dispatch[6])(self);
    else ((void(*)(StreetPassObj*))self->dispatch[4])(self);
}
#endif
static_assert(offsetof(Tower,parts)==0x64,"tower parts ABI");
static_assert(offsetof(Tower,count)==0x68,"tower count ABI");
static_assert(offsetof(StreetPassObj,table)==0x60,"table ABI");
static_assert(offsetof(StreetPassObj,indicator)==0x70,"indicator ABI");
static_assert(sizeof(Pool<Entry>)==0x14,"pool ABI");
