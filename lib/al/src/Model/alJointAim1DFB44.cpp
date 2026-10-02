// NonMatching reconstruction of the complete EU 001DFB44..001E04DC root.
// Includes its executable tail after the interior literal pool.  No new data
// identity or original class name is asserted by the descriptive record names.
#include <Model/alJointAim1DFB44.h>
#include <stddef.h>

using sead::Vector3f;
using sead::Quatf;
using sead::Matrix34f;
using al::JointAimEntry1DF860;
using al::JointAimController1E04DC;

// Native link identities already present in the EU map.  These declarations
// only bind calls to original entry points; no alternate math definitions are
// supplied.  Their observed records have the same 12/16/48-byte float layouts.
extern "C" {
void _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(Vector3f*,const Vector3f*,const Vector3f*);
void _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(Vector3f*,const Vector3f*,const Vector3f*);
void _ZN4sead14Vector3CalcCtrIfE6rotateERN2nn4math4VEC3ERKNS3_5MTX34ERKS4_(Vector3f*,const Matrix34f*,const Vector3f*);
void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(Vector3f*,const Vector3f*,float);
void _ZN4sead15Matrix34CalcCtrIfE12makeIdentityERN2nn4math5MTX34E(Matrix34f*);
void _ZN4sead15Matrix34CalcCtrIfE5makeQERN2nn4math5MTX34ERKNS3_4QUATE(Matrix34f*,const Quatf*);
void _ZN4sead15Matrix34CalcCtrIfE8multiplyERN2nn4math5MTX34ERKS4_S7_(Matrix34f*,const Matrix34f*,const Matrix34f*);
bool fn_0027D5C4(Vector3f*);
void fn_0027306C(Vector3f*,const Vector3f*,const Vector3f*);
float fn_0026E82C(float);
void fn_00270844(Quatf*,const Vector3f*,float);
void fn_0026A958(Quatf*,const Quatf*,const Quatf*);
void fn_0027A488(Vector3f*,const Quatf*);
void fn_0027D4D8(Quatf*,const Vector3f*,const Vector3f*);
float fn_0026F7F8(float);
float fn_00287908(float);
void fn_0024AE40(Quatf*,const Matrix34f*);
}

namespace
{
inline float dot(const Vector3f& a,const Vector3f& b)
{
        return a.x*b.x + a.y*b.y + a.z*b.z;
}
inline Vector3f column(const Matrix34f& m,int index)
{
        return Vector3f(m.m[0][index],m.m[1][index],m.m[2][index]);
}
inline void rotate(Vector3f& v,const Matrix34f& m)
{
        _ZN4sead14Vector3CalcCtrIfE6rotateERN2nn4math4VEC3ERKNS3_5MTX34ERKS4_(&v,&m,&v);
}
inline float clamp(float value,float low,float high)
{
        if(value<low) return low;
        if(value>high) return high;
        return value;
}
// Integer comparisons of float representations are visible in the original
// angle wrapping, quaternion clamp, small-sine test and inverse threshold.
union FloatBits { float f; int s; unsigned int u; };
inline int signedBits(float value) { FloatBits bits;bits.f=value;return bits.s; }
inline unsigned int unsignedBits(float value) { FloatBits bits;bits.f=value;return bits.u; }
inline float signedAngle(float value)
{
        if(signedBits(value)>signedBits(3.1415927410125732421875f))
                value-=6.283185482025146484375f;
        return value;
}
inline void rotate(Vector3f& v,const Quatf& q)
{
        const float x=q.y*v.z-q.z*v.y+q.w*v.x;
        const float y=q.z*v.x-q.x*v.z+q.w*v.y;
        const float w=-q.x*v.x-q.y*v.y-q.z*v.z;
        const float z=q.x*v.y-q.y*v.x+q.w*v.z;
        v.x=x*q.w-y*q.z+z*q.y-w*q.x;
        v.y=y*q.w+x*q.z-z*q.x-w*q.y;
        v.z=y*q.x-x*q.y+z*q.w-w*q.z;
}
inline void interpolate(Quatf& current,const Quatf& target,float amount)
{
        float cosine=current.x*target.x+current.y*target.y+current.z*target.z+current.w*target.w;
        if(signedBits(cosine)>signedBits(1.0f)) cosine=1.0f;
        else if(unsignedBits(cosine)>unsignedBits(-1.0f)) cosine=-1.0f;
        const bool reverse=cosine<0.0f;
        if(reverse) cosine=-cosine;
        const float angle=fn_0026F7F8(cosine);
        const float sine=fn_00287908(angle);
        const float magnitude=sine>0.0f?sine:-sine;
        float fromWeight;
        float toWeight=amount;
        if(signedBits(magnitude)<0x34000000) fromWeight=1.0f-amount;
        else {
                const float fraction=amount*angle;
                const float reciprocal=1.0f/sine;
                fromWeight=fn_00287908(angle-fraction)*reciprocal;
                toWeight=fn_00287908(fraction)*reciprocal;
        }
        if(reverse) toWeight=-toWeight;
        current.x=current.x*fromWeight+toWeight*target.x;
        current.y=current.y*fromWeight+toWeight*target.y;
        current.z=current.z*fromWeight+toWeight*target.z;
        current.w=current.w*fromWeight+toWeight*target.w;
}
inline Quatf inverse(const Quatf& q)
{
        const float norm=q.w*q.w+q.x*q.x+q.y*q.y+q.z*q.z;
        if(signedBits(norm)>0x34000000) {
                const float reciprocal=1.0f/norm;
                return Quatf(-q.x*reciprocal,-q.y*reciprocal,-q.z*reciprocal,q.w*reciprocal);
        }
        return Quatf(-q.x,-q.y,-q.z,q.w);
}
}

extern "C" bool fn_001DFB44(JointAimController1E04DC* controller,Matrix34f* jointMatrix,int jointIndex)
{
        for(int i=0;i<controller->count;++i) {
                JointAimEntry1DF860* entry=controller->entries;
                if((unsigned int)controller->count>(unsigned int)i) {
                        int index=controller->first+i;
                        if(index>=controller->capacity) index-=controller->capacity;
                        entry+=index;
                }
                if(entry->jointIndex!=jointIndex) continue;

                const Vector3f translation=column(*jointMatrix,3);
                Vector3f direction=controller->targetPosition;
                _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(&direction,&direction,&translation);
                fn_0027D5C4(&direction);
                const float threshold=entry->wasAiming?0.0f:0.1f;
                Vector3f front;
                if(entry->referenceMatrix) {
                        front=entry->referenceFront;
                        rotate(front,*entry->referenceMatrix);
                } else front=column(*controller->actorMatrix,2);

                Quatf target;
                if(!controller->enabled) {
                        entry->wasAiming=false;
                        target=Quatf::unit;
                } else if(dot(direction,front)>threshold) {
                        Vector3f side,up,forward;
                        const Matrix34f* reference;
                        if(entry->referenceMatrix) {
                                side=entry->referenceSide;
                                up=entry->referenceUp;
                                forward=entry->referenceFront;
                                rotate(side,*entry->referenceMatrix);
                                rotate(up,*entry->referenceMatrix);
                                rotate(forward,*entry->referenceMatrix);
                                reference=entry->referenceMatrix;
                        } else {
                                side=column(*controller->actorMatrix,0);
                                up=column(*controller->actorMatrix,1);
                                forward=column(*controller->actorMatrix,2);
                                reference=controller->actorMatrix;
                        }
                        const Vector3f localDirection(dot(side,direction),dot(up,direction),dot(forward,direction));
                        Vector3f jointFront=entry->jointFront;
                        Vector3f jointUp=entry->jointUp;
                        Vector3f jointSide=entry->jointSide;
                        rotate(jointFront,*jointMatrix);
                        rotate(jointUp,*jointMatrix);
                        rotate(jointSide,*jointMatrix);

                        Vector3f planar;
                        fn_0027306C(&planar,&Vector3f::ey,&localDirection);
                        fn_0027D5C4(&planar);
                        float horizontal=signedAngle(fn_0026E82C(clamp(dot(planar,Vector3f::ex),-1.0f,1.0f)));
                        Vector3f yawSide=Vector3f::ex;
                        Quatf yaw;
                        fn_00270844(&yaw,&Vector3f::ey,horizontal);
                        rotate(yawSide,yaw);
                        Vector3f verticalPlane;
                        fn_0027306C(&verticalPlane,&yawSide,&localDirection);
                        fn_0027D5C4(&verticalPlane);
                        float vertical=signedAngle(fn_0026E82C(clamp(dot(verticalPlane,Vector3f::ey),-1.0f,1.0f)));
                        horizontal=clamp(horizontal,entry->horizontalLimits.x,entry->horizontalLimits.y);
                        vertical=clamp(vertical,entry->verticalLimits.x,entry->verticalLimits.y);
                        entry->wasAiming=true;

                        Quatf horizontalRotation,verticalRotation,combined;
                        fn_00270844(&horizontalRotation,&Vector3f::ey,horizontal);
                        const Vector3f negativeSide(-Vector3f::ex.x,-Vector3f::ex.y,-Vector3f::ex.z);
                        fn_00270844(&verticalRotation,&negativeSide,vertical);
                        fn_0026A958(&combined,&horizontalRotation,&verticalRotation);
                        Vector3f rotatedFront;
                        fn_0027A488(&rotatedFront,&combined);
                        Vector3f referenceFront;
                        if(entry->referenceMatrix) {
                                Vector3f term;
                                _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(&term,&entry->referenceSide,rotatedFront.x);
                                referenceFront=term;
                                _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(&term,&entry->referenceUp,rotatedFront.y);
                                _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(&referenceFront,&referenceFront,&term);
                                _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(&term,&entry->referenceFront,rotatedFront.z);
                                _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(&referenceFront,&referenceFront,&term);
                        } else referenceFront=rotatedFront;
                        rotate(referenceFront,*reference);
                        Vector3f jointDirection(dot(jointSide,referenceFront),dot(jointUp,referenceFront),dot(jointFront,referenceFront));
                        fn_0027D5C4(&jointDirection);
                        const Vector3f upAxis(0.0f,1.0f,0.0f);
                        fn_0027D4D8(&target,&jointDirection,&upAxis);
                } else {
                        entry->wasAiming=false;
                        target=controller->retainPreviousTarget?entry->previousTarget:Quatf::unit;
                }
                entry->previousTarget=target;
                interpolate(entry->currentRotation,target,entry->interpolation);

                Matrix34f basis;
                _ZN4sead15Matrix34CalcCtrIfE12makeIdentityERN2nn4math5MTX34E(&basis);
                basis.m[0][0]=entry->jointSide.x;basis.m[1][0]=entry->jointSide.y;basis.m[2][0]=entry->jointSide.z;
                basis.m[0][1]=entry->jointUp.x;basis.m[1][1]=entry->jointUp.y;basis.m[2][1]=entry->jointUp.z;
                basis.m[0][2]=entry->jointFront.x;basis.m[1][2]=entry->jointFront.y;basis.m[2][2]=entry->jointFront.z;
                Quatf basisRotation;
                fn_0024AE40(&basisRotation,&basis);
                const Quatf inverseBasis=inverse(basisRotation);
                Quatf correction=basisRotation;
                fn_0026A958(&correction,&correction,&entry->currentRotation);
                fn_0026A958(&correction,&correction,&inverseBasis);
                Matrix34f correctionMatrix;
                _ZN4sead15Matrix34CalcCtrIfE5makeQERN2nn4math5MTX34ERKNS3_4QUATE(&correctionMatrix,&correction);
                _ZN4sead15Matrix34CalcCtrIfE8multiplyERN2nn4math5MTX34ERKS4_S7_(jointMatrix,jointMatrix,&correctionMatrix);
                return true;
        }
        return false;
}

static_assert(sizeof(JointAimEntry1DF860)==0x88,"entry stride");
static_assert(offsetof(JointAimEntry1DF860,currentRotation)==0x38,"current quaternion");
static_assert(offsetof(JointAimEntry1DF860,previousTarget)==0x48,"retained quaternion");
static_assert(offsetof(JointAimEntry1DF860,wasAiming)==0x58,"aim hysteresis");
static_assert(offsetof(JointAimEntry1DF860,referenceMatrix)==0x60,"reference matrix");
static_assert(sizeof(JointAimController1E04DC)==0x24,"controller allocation");
static_assert(offsetof(JointAimController1E04DC,count)==0x1c,"ring count");
