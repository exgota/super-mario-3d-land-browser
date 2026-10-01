#include <clean/TransformBlendRoot.h>
#include <math.h>

#ifdef NON_MATCHING
using namespace transform_blend;
extern "C" {
int fn_00215E20(float);
int fn_0028A998(unsigned*);
void fn_00291470(Matrix34*, const Matrix34*);
void fn_002A52EC(Transform*, bool, const Matrix34*);
void fn_002A55D4(Transform*);
extern unsigned dat_003F389C;
extern TypeNode dat_003F151C;
extern Matrix34 dat_00430C68;
extern float dat_003B6694;
// Only the first 256 records are consumed; the mapped data object is larger.
extern float dat_003A48F4[][4];
}
namespace nw { namespace gfx {
struct CalculatedTransform {
    void UpdateTranslateFlags();
    void UpdateCompositeFlags();
};
}}
namespace {
union FloatWord { float f; unsigned u; int i; };
__forceinline unsigned bits(float f) { FloatWord w; w.f=f; return w.u; }
__forceinline bool near(float f) { return (int)bits(fabsf(f)) <= 0x3a83126f; }
__forceinline int count(Root* p) { return p->childrenEnd-p->children; }
__forceinline Evaluator* special(Evaluator* e) {
    if (!e) return 0;
    TypeNode* t=e->vtable->type(e);
    do { if (t==&dat_003F151C) return e; t=t->parent; } while(t);
    return 0;
}
__forceinline bool zeroVector(const float* p) {
    // The retail helper preserves subtraction-by-zero exceptions under FZ.
    return fn_00215E20(p[0]) && fn_00215E20(p[1]) && fn_00215E20(p[2]);
}
__forceinline float reciprocal(float x) {
    if (near(x-1.0f) || fn_00215E20(x)) return 1.0f;
    return 1.0f/x;
}
__forceinline void identity(Transform& t) {
    if (!(dat_003F389C&1) && fn_0028A998(&dat_003F389C)) {
        dat_00430C68.m[0][0]=1.0f; dat_00430C68.m[0][1]=0.0f;
        dat_00430C68.m[0][2]=0.0f; dat_00430C68.m[0][3]=0.0f;
        dat_00430C68.m[1][0]=0.0f; dat_00430C68.m[1][1]=1.0f;
        dat_00430C68.m[1][2]=0.0f; dat_00430C68.m[1][3]=0.0f;
        dat_00430C68.m[2][0]=0.0f; dat_00430C68.m[2][1]=0.0f;
        dat_00430C68.m[2][2]=1.0f; dat_00430C68.m[2][3]=0.0f;
    }
    fn_00291470(&t.matrix,&dat_00430C68);
    t.scale[0]=t.scale[1]=t.scale[2]=1.0f;
    t.flags=0x801;
}
__forceinline void sincosIndex(float value, float& s, float& c) {
    FloatWord factor; factor.u=0x4222f983;
    float x=value*factor.f;
    bool negative=x<0.0f;
    x=fabsf(x);
    while ((int)bits(x)>=0x47800000) x-=65536.0f;
    unsigned index=(unsigned short)(unsigned)x;
    float fraction=x-(float)index;
    const float* entry=dat_003A48F4[index&255];
    s=entry[0]+fraction*entry[2];
    c=entry[1]+fraction*entry[3];
    if (negative) s=-s;
}
__forceinline void bindPose(Transform& t, const BindPose& p) {
    t.scale[0]=p.scale[0]; t.scale[1]=p.scale[1]; t.scale[2]=p.scale[2];
    float sx,cx,sy,cy,sz,cz;
    sincosIndex(p.rotation[0],sx,cx);
    sincosIndex(p.rotation[1],sy,cy);
    sincosIndex(p.rotation[2],sz,cz);
    t.matrix.m[0][0]=cz*cy;
    t.matrix.m[1][0]=sz*cy;
    t.matrix.m[2][1]=sx*cy;
    t.matrix.m[2][2]=cx*cy;
    float a=sx*cz, b=cx*sz;
    t.matrix.m[0][1]=a*sy-b;
    t.matrix.m[1][2]=b*sy-a;
    a=sx*sz; b=cx*cz;
    t.matrix.m[0][2]=a+b*sy;
    t.matrix.m[1][1]=b+a*sy;
    t.matrix.m[2][0]=-sy;
    t.matrix.m[0][3]=p.translation[0];
    t.matrix.m[1][3]=p.translation[1];
    t.matrix.m[2][3]=p.translation[2];
    t.flags|=0x800;
    if (!(t.flags&8)) {
        t.flags&=~0x600;
        if (t.scale[0]==t.scale[1] && t.scale[0]==t.scale[2]) {
            t.flags|=0x400;
            if (bits(t.scale[0])==0x3f800000) t.flags|=0x200;
        }
    }
    fn_002A55D4(&t);
    ((nw::gfx::CalculatedTransform*)&t)->UpdateTranslateFlags();
    ((nw::gfx::CalculatedTransform*)&t)->UpdateCompositeFlags();
}
__forceinline unsigned char* relative(unsigned char* field) {
    int offset=*(int*)field;
    return offset ? field+offset : 0;
}
}

extern "C" Transform* fn_0033BA3C(Root* self, Transform* output, int index) {
    unsigned char* dictionary=relative(self->model->resource+0x14);
    unsigned char* record=dictionary ? relative(dictionary+index*16+0x28) : 0;
    BlendPolicy* policy=self->model->policies[*(unsigned*)(record+0x0c)];
    if (self->componentMode) {
        float sums[3]={0.0f,0.0f,0.0f};
        for (int i=0;i<count(self);++i) {
            Evaluator* child=self->children[i];
            if (!child) continue;
            float weight=self->weights[i];
            if (near(weight)) continue;
            if (!child->vtable->hasIndex(child,index)) continue;
            Evaluator* masked=special(child);
            if (!masked || !masked->disabled[0]) sums[0]+=weight;
            if (!masked || !masked->disabled[1]) sums[1]+=weight;
            if (!masked || !masked->disabled[2]) sums[2]+=weight;
        }
        if (fn_00215E20(sums[0]) && fn_00215E20(sums[1]) && fn_00215E20(sums[2])) return 0;
        float factors[3]={reciprocal(sums[0]),reciprocal(sums[1]),reciprocal(sums[2])};
        unsigned oldFlags=output->flags;
        bool preserved=(oldFlags&0x40000000)!=0;
        output->flags|=0x4000001c;
        Transform temp; identity(temp);
        Matrix34 firstMatrix;
        bool noMatrix=true, any=false;
        unsigned flags=0x7e0;
        for (int i=count(self)-1;i>=0;--i) {
            Evaluator* child=self->children[i];
            if (!child) continue;
            float weight=self->weights[i];
            float weights[3]={factors[0]*weight,factors[1]*weight,factors[2]*weight};
            Evaluator* masked=special(child);
            if (masked) {
                if (masked->disabled[0]) weights[0]=dat_003B6694;
                if (masked->disabled[1]) weights[1]=dat_003B6694;
                if (masked->disabled[2]) weights[2]=dat_003B6694;
            }
            if (zeroVector(weights)) continue;
            if (preserved) temp.flags|=0x40000000; else temp.flags&=~0x40000000;
            Transform* result=child->vtable->evaluate(child,&temp,index);
            if (!result) continue;
            any=true;
            if (special(child) && noMatrix && !(temp.flags&0x10)) {
                firstMatrix=temp.matrix; noMatrix=false;
            }
            if (!policy->vtable->blend(policy,output,0,result,weights)) break;
            flags&=temp.flags;
        }
        if (!preserved) output->flags&=~0x40000000;
        if (!any) {
            output->flags=(output->flags&~0x1c)|(oldFlags&0x1c);
            return 0;
        }
        if (!preserved && policy->needsFinish && !policy->vtable->finish(policy,output,0))
            fn_002A52EC(output,!noMatrix,&firstMatrix);
        output->flags=(output->flags&~0x7e0)|(flags&0x7e0);
    } else {
        bool any=false;
        for (int i=0;i<count(self);++i) {
            Evaluator* child=self->children[i];
            if (child && child->vtable->hasIndex(child,index)) { any=true; break; }
        }
        if (!any) return 0;
        if (self->dirty && self->normalize) {
            float sum=0.0f;
            for (int i=0;i<self->weightsEnd-self->weights;++i) sum=self->weights[i]+sum;
            float factor=reciprocal(sum);
            for (int i=0;i<self->weightsEnd-self->weights;++i)
                self->cachedWeights[i]=self->weights[i]*factor;
            self->dirty=0;
        }
        bool preserved=(output->flags&0x40000000)!=0;
        output->flags|=0x4000001c;
        Transform temp; identity(temp);
        Matrix34 firstMatrix;
        bool noMatrix=true;
        unsigned flags=0x7e0;
        for (int i=count(self)-1;i>=0;--i) {
            float weight=self->cachedWeights[i];
            Evaluator* child=self->children[i];
            float weights[3]={weight,weight,weight};
            if (zeroVector(weights)) continue;
            if (preserved) temp.flags|=0x40000000; else temp.flags&=~0x40000000;
            Transform* result=child ? child->vtable->evaluate(child,&temp,index) : 0;
            if (!result) { bindPose(temp,*self->model->poses[index]); result=&temp; }
            if (special(child) && noMatrix && !(temp.flags&0x10)) {
                firstMatrix=temp.matrix; noMatrix=false;
            }
            if (!policy->vtable->blend(policy,output,0,result,weights)) break;
            flags&=temp.flags;
        }
        if (!preserved) {
            output->flags&=~0x40000000;
            if (policy->needsFinish && !policy->vtable->finish(policy,output,0))
                fn_002A52EC(output,!noMatrix,&firstMatrix);
        }
        output->flags=(output->flags&~0x7e0)|(flags&0x7e0);
    }
    return output;
}
#endif
