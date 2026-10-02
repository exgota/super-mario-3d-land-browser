#include <Observed/TransformStateFactory.h>

using namespace transform_state;
extern "C" {
int fn_0028A998(unsigned*);
void fn_00291470(Matrix34*, const Matrix34*);
Matrix34* fn_00254890();
void fn_002B349C(Root*, Allocator*, unsigned, signed char);
extern unsigned dat_003F389C;
extern unsigned dat_003F38A8;
extern Matrix34 dat_00430C68;
extern Transform dat_00430CF0;
// Observed overlapping scale view at Transform+0x30; allocation identity unresolved.
extern float dat_00430D20[];
extern const unsigned dat_003D86BC[];
extern const unsigned dat_003D82D0[];
}

#ifdef NON_MATCHING
namespace {
template<class T> __forceinline void transfer(Array<T>& to, Array<T>& from) {
    to.allocator=from.allocator;
    to.begin=from.begin;
    to.end=from.end;
    to.capacityAndFlags=from.capacityAndFlags;
    from.begin=0;
    from.end=0;
    from.capacityAndFlags &= 0xc0000000u;
}
template<class T> struct OwnedArray : Array<T> {
    __forceinline OwnedArray(Allocator* allocator, int count) {
        this->allocator=allocator;
        this->begin=count ? static_cast<T*>(allocator->vtable->allocate(
            allocator,static_cast<unsigned>(count)*sizeof(T),32)) : 0;
        this->end=this->begin;
        this->capacityAndFlags=static_cast<unsigned>(count)&0x3fffffffu;
    }
    __forceinline OwnedArray(Array<T>& from) { transfer<T>(*this,from); }
    __forceinline OwnedArray(OwnedArray& from) { transfer<T>(*this,from); }
    __forceinline ~OwnedArray() {
        for (int i=1;i<(this->end-this->begin)+1;++i)
            (this->end-i)->~T();
        if (this->allocator && this->begin)
            this->allocator->vtable->release(this->allocator,this->begin);
    }
};
__forceinline const Matrix34* identity() {
    if (!(dat_003F389C&1) && fn_0028A998(&dat_003F389C)) {
        dat_00430C68.m[0][0]=1.0f; dat_00430C68.m[0][1]=0.0f;
        dat_00430C68.m[0][2]=0.0f; dat_00430C68.m[0][3]=0.0f;
        dat_00430C68.m[1][0]=0.0f; dat_00430C68.m[1][1]=1.0f;
        dat_00430C68.m[1][2]=0.0f; dat_00430C68.m[1][3]=0.0f;
        dat_00430C68.m[2][0]=0.0f; dat_00430C68.m[2][1]=0.0f;
        dat_00430C68.m[2][2]=1.0f; dat_00430C68.m[2][3]=0.0f;
    }
    return &dat_00430C68;
}
__forceinline void defaultTransform(Transform& value) {
    if (!(dat_003F38A8&1) && fn_0028A998(&dat_003F38A8)) {
        fn_00291470(&dat_00430CF0.matrix,fn_00254890());
        dat_00430CF0.scale[0]=1.0f;
        dat_00430CF0.scale[1]=1.0f;
        dat_00430CF0.scale[2]=1.0f;
        dat_00430CF0.flags=0x7e1;
    }
    fn_00291470(&value.matrix,&dat_00430CF0.matrix);
    value.scale[0]=dat_00430D20[0];
    value.scale[1]=dat_00430D20[1];
    value.scale[2]=dat_00430D20[2];
    value.flags=dat_00430CF0.flags;
}
__forceinline void append(OwnedArray<Transform>& array,const Transform& value) {
    Transform* destination=array.end++;
    if (destination) {
        fn_00291470(&destination->matrix,&value.matrix);
        destination->scale[0]=value.scale[0];
        destination->scale[1]=value.scale[1];
        destination->scale[2]=value.scale[2];
        destination->flags=value.flags;
    }
}
__forceinline void append(OwnedArray<Matrix34>& array,const Matrix34& value) {
    Matrix34* destination=array.end++;
    if (destination) fn_00291470(destination,&value);
}
__forceinline void construct(Root* self, Resource* resource,unsigned word,signed char mode,
    Allocator* allocator,OwnedArray<Matrix34> auxiliary,OwnedArray<Matrix34> matrices,
    OwnedArray<Transform> calculated,OwnedArray<Transform> supplied) {
    self->vtable=dat_003D86BC;
    self->allocator=allocator;
    self->resource=resource;
    self->storage10=0;
    self->storage14=0;
    self->flag18=0;
    self->opaque0c=0;
    fn_002B349C(self,allocator,word,mode);
    self->vtable=dat_003D82D0;
    transfer(self->supplied,supplied);
    transfer(self->calculated,calculated);
    transfer(self->matrices,matrices);
    transfer(self->auxiliaryMatrices,auxiliary);
    self->source=resource;
}
}

// NonMatching: complete original interval 002A2C9C..002A34D4, both literal islands.
extern "C" Root* fn_002A2C9C(Resource* resource,unsigned word,signed char mode,
    Array<Transform>* supplied,Allocator* allocator) {
    Root* self=static_cast<Root*>(allocator->vtable->allocate(allocator,sizeof(Root),4));
    OwnedArray<Transform> calculated(allocator,resource->count);
    OwnedArray<Matrix34> matrices(allocator,resource->count);
    OwnedArray<Matrix34> auxiliary(allocator,resource->count);
    for (int i=0;i<resource->count;++i) {
        Transform transform;
        defaultTransform(transform);
        append(calculated,transform);
        Matrix34 matrix;
        fn_00291470(&matrix,identity());
        append(matrices,matrix);
        fn_00291470(&matrix,identity());
        append(auxiliary,matrix);
    }
    if (self) construct(self,resource,word,mode,allocator,auxiliary,matrices,calculated,*supplied);
    return self;
}
#endif // NON_MATCHING
