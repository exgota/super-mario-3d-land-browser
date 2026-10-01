#include <Collision/SweptSphere269A60.h>
#include <math.h>
using namespace dot269a60;

extern "C" {
// Existing map identities. The static filter is independently delimited by
// initializer 00387DEC and callback 00361114, but has no canonical data row.
extern Filter dat_00424B14;
void _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(Vec*, const Vec*, const Vec*);
void _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(Vec*, const Vec*, const Vec*);
void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(Vec*, const Vec*, float);
void _ZN4sead14Vector3CalcCtrIfE5crossERN2nn4math4VEC3ERKS4_S7_(Vec*, const Vec*, const Vec*);
void fn_0027306C(Vec*, const Vec*, const Vec*);
void fn_0025C824(Sweep*, const Vec*, const Vec*, float, float, float);
void fn_0025C7F8(Sweep*);
void fn_0025C790(const Sweep*, Vec*, float*, Vec*);
void* _ZN2al15getLiveActorKitEv();
unsigned fn_0025CA4C(Director*, const Vec*, int, float);
Contact* fn_001D43DC(Director*, unsigned);
bool fn_0025C780(const Contact*);
const Vec* fn_0025C760(const Contact*, int);
const Vec* fn_0025C770(const Contact*, int);
int fn_0026B000(int, int);
bool fn_001676B0(const Hit*, const Hit*);
void fn_0039EC9C(Hit*, Hit*, int, Less);
void fn_0039E180(Hit*, Hit*, Less);
Surface* fn_0025C8E4(Surface*, const Surface*);
}

namespace {
inline Vec add(const Vec& a,const Vec& b) { Vec r; _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(&r,&a,&b); return r; }
inline Vec sub(const Vec& a,const Vec& b) { Vec r; _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(&r,&a,&b); return r; }
inline Vec scale(const Vec& a,float b) { Vec r; _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(&r,&a,b); return r; }
inline Vec cross(const Vec& a,const Vec& b) { Vec r; _ZN4sead14Vector3CalcCtrIfE5crossERN2nn4math4VEC3ERKS4_S7_(&r,&a,&b); return r; }
inline float dot(const Vec& a,const Vec& b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
inline Director* director() { return *reinterpret_cast<Director**>(static_cast<unsigned char*>(_ZN2al15getLiveActorKitEv())+0x28); }
inline void exclude(void* key) {
    Ring& q=dat_00424B14.ring;
    if(q.count<q.capacity) {
        int index=q.head+q.count++;
        if(index>=q.capacity) index-=q.capacity;
        q.slots[index]=key;
    }
}
// The configured fast FP mode folds unordered comparisons. Preserve the
// original root's explicit unordered path for degenerate edge intersections.
inline bool isNan(float value) {
    union { float value; unsigned bits; } representation;
    representation.value=value;
    return (representation.bits&0x7fffffffU)>0x7f800000U;
}
inline float solve(const Vec& delta,const Vec& direction,float radius) {
    float a=dot(direction,direction);
    float b=2.0f*dot(delta,direction);
    float c=dot(delta,delta)-radius*radius;
    float discriminant=b*b-(4.0f*a)*c;
    float result=0.0f;
    if(discriminant>=0.0f || isNan(discriminant)) {
        float root=sqrtf(discriminant);
        float q=(b>0.0f ? -b-root : root-b)*0.5f;
        result=q*(1.0f/a);
        if(result<0.0f) result=c/(a*result);
    }
    return result;
}
inline void store(Hit& hit,float fraction,const Vec& position,const Surface& surface) {
    hit.fraction=fraction;
    hit.position=position;
    hit.surface=surface;
}
inline void sort(Hit* begin,Hit* end) {
    Less less=&fn_001676B0;
    if(begin==end) return;
    fn_0039EC9C(begin,end,end-begin,less);
    if(end-begin>16) {
        fn_0039E180(begin,begin+16,less);
        for(Hit* next=begin+16;next!=end;++next) {
            Hit value=*next;
            Hit* hole=next;
            Hit* previous=hole-1;
            while(less(&value,previous)) { *hole=*previous; hole=previous; --previous; }
            hole->fraction=value.fraction;
            hole->position=value.position;
            fn_0025C8E4(&hole->surface,&value.surface);
        }
    } else fn_0039E180(begin,end,less);
}
}

// NonMatching: kept outside accepted ranks. Reuses original query/vector/sort
// boundaries; the static data identity requires separate canonical-map review.
extern "C" int fn_00269A60(Hit* output,int capacity,const Vec* start,
                          const Vec* direction,float radius,void* queryFilter,
                          void* delegatedFilter) {
    dat_00424B14.ring.head=0;
    dat_00424B14.ring.count=0;
    dat_00424B14.delegatedFilter=delegatedFilter;
    float sampleRadius=radius;
    if(35.0f<sampleRadius) sampleRadius=35.0f;
    Sweep sweep;
    Vec end=add(*start,*direction);
    fn_0025C824(&sweep,start,&end,radius,radius,sampleRadius);
    fn_0025C7F8(&sweep);
    int count=0;
    while(!(sweep.current==1.0f && sweep.previous==1.0f)) {
        Vec center(0,0,0);
        float sampledRadius=1.0f;
        fn_0025C790(&sweep,&center,&sampledRadius,0);
        Director* query=director();
        query->queryFilter=queryFilter;
        query->exclusionFilter=&dat_00424B14;
        unsigned contacts=fn_0025CA4C(query,&center,1,sampledRadius);
        for(unsigned i=0;i<contacts && count<capacity;++i) {
            Contact* contact=fn_001D43DC(director(),i);
            const Vec& normal=contact->surface.normal;
            if(dot(normal,*direction)>=0.0f) { exclude(contact->surface.key); continue; }
            if(fn_0025C780(contact)) {
                float denominator=dot(normal,*direction);
                if(denominator<=0.0f || isNan(denominator)) denominator=-denominator;
                float planeFraction=contact->distance/denominator;
                Vec backwards=scale(*direction,-planeFraction);
                Vec tangent;
                fn_0027306C(&tangent,&normal,&backwards);
                Vec point=add(contact->position,tangent);
                Vec delta0=sub(point,*fn_0025C770(contact,0));
                Vec delta1=sub(point,*fn_0025C770(contact,1));
                Vec delta2=sub(point,*fn_0025C770(contact,2));
                bool outside0=dot(*fn_0025C760(contact,0),delta0)>0.0f;
                bool outside1=dot(*fn_0025C760(contact,1),delta1)>0.0f;
                bool outside2=dot(*fn_0025C760(contact,2),delta2)>0.0f;
                if(outside0|outside1|outside2) {
                    Vec relative=sub(point,contact->position);
                    int edge=0;
                    if(dot(*fn_0025C760(contact,0),relative)>0.0f) {
                        edge=1;
                        if(dot(*fn_0025C760(contact,1),relative)>0.0f) edge=2;
                    }
                    int next=fn_0026B000(edge+4,3);
                    Vec vertexDelta=sub(*fn_0025C770(contact,next),contact->position);
                    Vec perpendicular=cross(vertexDelta,relative);
                    int side=fn_0026B000(next+(dot(normal,perpendicular)>0.0f?4:3),3);
                    const Vec& boundary=*fn_0025C760(contact,side);
                    float ratio=dot(boundary,vertexDelta)/dot(boundary,relative);
                    Vec displacement=scale(relative,ratio);
                    Vec intersection=add(contact->position,displacement);
                    float fraction=solve(sub(intersection,center),*direction,sampledRadius);
                    store(output[count++],sweep.current-fraction,intersection,contact->surface);
                } else store(output[count++],sweep.current-planeFraction,point,contact->surface);
            } else {
                float fraction=solve(sub(contact->position,center),*direction,sampledRadius);
                store(output[count++],sweep.current-fraction,contact->position,contact->surface);
            }
            exclude(contact->surface.key);
        }
        if(count>=capacity) break;
        fn_0025C7F8(&sweep);
    }
    sort(output,output+count);
    return count;
}
