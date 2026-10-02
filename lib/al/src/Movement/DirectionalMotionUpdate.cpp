// Original EU 0x001A3F08. Clean-room directional velocity update.
// NON_MATCHING: semantic reconstruction awaiting canonical exactness.
#include <stddef.h>
#include <math.h>
namespace nn { namespace math { struct VEC3 { float x,y,z; }; }}
namespace sead { template<class T> struct Vector3CalcCtr {
    static void cross(nn::math::VEC3&,const nn::math::VEC3&,const nn::math::VEC3&);
    static void multScalar(nn::math::VEC3&,const nn::math::VEC3&,float);
    static void add(nn::math::VEC3&,const nn::math::VEC3&,const nn::math::VEC3&);
}; }
namespace motion1a3f {
typedef unsigned Word;
typedef nn::math::VEC3 Vec3;
typedef sead::Vector3CalcCtr<float> Calc;
struct Virtual { void** table; };
struct Motion { unsigned char unknown0[12]; Vec3 forward; unsigned char unknown18[12]; Vec3 velocity; };
struct Controller { unsigned char unknown0[16]; Motion* motion; Virtual* input; Virtual* trigger; Word unknown1c; Virtual* listener; Word unknown24; Virtual* blend; Word unknown2c[3]; Word frames; Word unknown3c; Word fastFrames; };
struct Quaternion { float x,y,z,w; };
static_assert_(offsetof(Controller,motion)==0x10);
static_assert_(offsetof(Controller,fastFrames)==0x40);
static_assert_(offsetof(Motion,velocity)==0x24);
inline Word callWord(Virtual* p,unsigned offset) { return reinterpret_cast<Word(*)(Virtual*)>(p->table[offset/4])(p); }
inline float callFloat(Virtual* p,unsigned offset) { return reinterpret_cast<float(*)(Virtual*)>(p->table[offset/4])(p); }
inline const Vec3* callVector(Virtual* p,unsigned offset) { return reinterpret_cast<const Vec3*(*)(Virtual*)>(p->table[offset/4])(p); }
inline void notify(Virtual* p,unsigned offset,bool value) { reinterpret_cast<void(*)(Virtual*,bool)>(p->table[offset/4])(p,value); }
inline float dot(const Vec3& a,const Vec3& b) { return (a.x*b.x+a.y*b.y)+a.z*b.z; }
inline Word bits(float value) { union { float f;Word u; } cast;cast.f=value;return cast.u; }
}
extern "C" {
motion1a3f::Virtual* fn_0026E1DC(motion1a3f::Controller*);
void fn_0027305C(motion1a3f::Motion*);
void fn_00279ABC(motion1a3f::Vec3*);
float fn_00252864(unsigned,float);
float fn_001735B0(float,float);
void fn_0027306C(motion1a3f::Vec3*,const motion1a3f::Vec3*,const motion1a3f::Vec3*);
float fn_00173580(float,float,float);
void fn_00267738(motion1a3f::Quaternion*,const motion1a3f::Vec3*,const motion1a3f::Vec3*,float);
void fn_001A3A58(motion1a3f::Controller*,bool,float);
void fn_001A3938(motion1a3f::Controller*,bool,float);
}
#ifdef NON_MATCHING
extern "C" void fn_001A3F08(motion1a3f::Controller* self) {
    using namespace motion1a3f;
    Virtual* config=fn_0026E1DC(self);
    const Vec3* input=callVector(self->input,0xc);
    if(callWord(self->trigger,0x24))fn_0027305C(self->motion);
    Vec3 forward=self->motion->forward;
    fn_00279ABC(&forward);
    Vec3 up;up.x=0.0f;up.y=1.0f;up.z=0.0f;
    Vec3 right;
    Calc::cross(right,up,forward);
    fn_00279ABC(&right);
    Calc::cross(up,forward,right);
    fn_00279ABC(&up);
    float forwardSpeed=dot(forward,self->motion->velocity);
    float verticalSpeed=dot(up,self->motion->velocity);
    float sideSpeed=dot(right,self->motion->velocity);
    unsigned friction=callWord(self->input,0x30)?callWord(config,0x40):callWord(config,0x3c);
    float threshold=callFloat(config,0x34)*0.95f;
    if(forwardSpeed>threshold)++self->fastFrames;
    else if(self->fastFrames<10)self->fastFrames=0;
    float lateral=fn_00252864(friction,sideSpeed);
    bool stopping=false;
    if(bits(forwardSpeed)>0xbdcccccd) {
        forwardSpeed=fn_00252864(friction,forwardSpeed);
        stopping=true;
    } else if(callWord(self->input,8)) {
        Vec3 originalInput=*input;
        Vec3 inputUnit=originalInput;
        fn_00279ABC(&inputUnit);
        if(bits(dot(inputUnit,forward))>0xbe31d0d4) {
            forwardSpeed=fn_001735B0(fn_00252864(friction,forwardSpeed),0.1f);
            stopping=true;
        } else {
            Vec3 desired=originalInput;
            fn_0027306C(&desired,&up,&desired);
            fn_00279ABC(&desired);
            float target;
            if(callFloat(self->blend,0)>0.0f) {
                float blend=callFloat(self->blend,0);
                float first=callFloat(config,0x2dc)*blend;
                target=first+callFloat(config,0x38)*(1.0f-blend);
            } else target=callWord(self->input,0x30)?callFloat(config,0x38):callFloat(config,0x34);
            float strength=callFloat(config,0xc4);
            float remainder=1.0f-callFloat(config,0xc4);
            float factor=strength+remainder*sqrtf(dot(originalInput,originalInput));
            float desiredSpeed=factor*target;
            if(desiredSpeed>forwardSpeed) {
                float acceleration;
                if(callFloat(config,0x34)>forwardSpeed) {
                    float base=callFloat(config,0x34);
                    unsigned duration=callWord(config,0x48);
                    acceleration=base/static_cast<float>(duration);
                } else {
                    float base=callFloat(config,0x38);
                    unsigned duration=callWord(config,0x4c);
                    acceleration=base/static_cast<float>(duration);
                }
                forwardSpeed=fn_00173580(forwardSpeed,target,acceleration);
            }
            if(forwardSpeed<0.0f)forwardSpeed=0.0f;
            else if(forwardSpeed>desiredSpeed) {
                if(callWord(self->input,0x30))forwardSpeed=fn_00252864(friction,forwardSpeed);
                else forwardSpeed=fn_00252864(callWord(config,0x44),forwardSpeed);
                if(forwardSpeed<desiredSpeed)forwardSpeed=desiredSpeed;
            }
            float numerator=forwardSpeed-callFloat(config,0x34);
            float high=callFloat(config,0x38);
            float denominator=high-callFloat(config,0x34);
            float ratio=numerator/denominator;
            if(ratio<0.0f)ratio=0.0f;
            else if(ratio>1.0f)ratio=1.0f;
            float weighted=callFloat(config,0x54)*ratio;
            float angle=weighted+callFloat(config,0x50)*(1.0f-ratio);
            Quaternion q;
            fn_00267738(&q,&forward,&desired,angle*0.01745329238474369f);
            float ty=(q.z*forward.x-q.x*forward.z)+q.w*forward.y;
            float tx=(q.y*forward.z-q.z*forward.y)+q.w*forward.x;
            float tw=(-(q.x*forward.x)-q.y*forward.y)-q.z*forward.z;
            float tz=(q.x*forward.y-q.y*forward.x)+q.w*forward.z;
            float z=((ty*q.x-tx*q.y)+tz*q.w)-tw*q.z;
            float y=((ty*q.w+tx*q.z)-tz*q.x)-tw*q.y;
            float x=((tx*q.w-ty*q.z)+tz*q.y)-tw*q.x;
            forward.x=x;forward.y=y;forward.z=z;
            fn_00279ABC(&forward);
            self->motion->forward=forward;
        }
    } else {
        forwardSpeed=fn_00252864(self->fastFrames>=10?friction:1,forwardSpeed);
        stopping=true;
    }
    verticalSpeed-=callFloat(config,0x10)*1.0f;
    if(-callFloat(config,0x90)>verticalSpeed)verticalSpeed=-callFloat(config,0x90);
    Motion* motion=self->motion;
    Vec3 a,b,c,d,e;
    Calc::multScalar(a,forward,forwardSpeed);
    Calc::multScalar(b,up,verticalSpeed);
    Calc::add(c,a,b);
    Calc::multScalar(d,right,lateral);
    Calc::add(e,c,d);
    motion->velocity=e;
    Vec3 planar=self->motion->velocity;
    fn_0027306C(&planar,&up,&planar);
    float speed=sqrtf(dot(planar,planar));
    if(self->listener)notify(self->listener,8,speed!=0.0f);
    fn_001A3A58(self,stopping,speed);
    fn_001A3938(self,stopping,speed);
    ++self->frames;
}
#endif
