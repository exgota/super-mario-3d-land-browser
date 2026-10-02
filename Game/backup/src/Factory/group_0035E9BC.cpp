typedef unsigned int Word;
typedef void* Ptr;
extern "C" Word fn_00160AA0(Ptr);
extern "C" Word fn_00160C1C(Ptr);
extern "C" Word fn_00160D3C(Ptr);
extern "C" Word fn_00161128(Ptr);
extern "C" Word fn_001537F8(Ptr);
extern "C" Word fn_00213350(Ptr);
extern "C" Word fn_002132B0(Ptr);
extern "C" Word fn_002130F8(Ptr);
extern "C" Word fn_0017ED04(Ptr);
extern "C" Word fn_00212DE4(Ptr);
extern "C" Word fn_0018AD7C(Ptr);
namespace al {
struct IUseNerve {};
bool isFirstStep(const IUseNerve*);
void updateNerveState(IUseNerve*);
}
extern "C" Word fn_0035E9BC(Ptr, Ptr* p) { return al::isFirstStep((const al::IUseNerve*)*p); }
extern "C" Word fn_003602B8(Ptr, Ptr* p) { return fn_00160AA0(*p); }
extern "C" Word fn_003602C0(Ptr, Ptr* p) { return fn_00160C1C(*p); }
extern "C" Word fn_003602C8(Ptr, Ptr* p) { return fn_00160D3C(*p); }
extern "C" Word fn_003602D0(Ptr, Ptr* p) { return fn_00161128(*p); }
extern "C" Word fn_00360698(Ptr, Ptr* p) { return fn_001537F8(*p); }
extern "C" Word fn_003607F0(Ptr, Ptr* p) { return fn_001537F8(*p); }
extern "C" Word fn_00360930(Ptr, Ptr* p) { return fn_00213350(*p); }
extern "C" Word fn_0036097C(Ptr, Ptr* p) { return fn_00213350(*p); }
extern "C" Word fn_00360984(Ptr, Ptr* p) { return fn_00213350(*p); }
extern "C" Word fn_00360AC0(Ptr, Ptr* p) { return fn_001537F8(*p); }
extern "C" Word fn_00361454(Ptr, Ptr* p) { return fn_002132B0(*p); }
extern "C" Word fn_0036145C(Ptr, Ptr* p) { return fn_002132B0(*p); }
extern "C" Word fn_00362A9C(Ptr, Ptr* p) { return fn_002130F8(*p); }
extern "C" Word fn_00362AA4(Ptr, Ptr* p) { return fn_002130F8(*p); }
extern "C" Word fn_003646D0(Ptr, Ptr* p) { al::updateNerveState((al::IUseNerve*)*p); }
extern "C" Word fn_003647F4(Ptr, Ptr* p) { al::updateNerveState((al::IUseNerve*)*p); }
extern "C" Word fn_0036549C(Ptr, Ptr* p) { return al::isFirstStep((const al::IUseNerve*)*p); }
extern "C" Word fn_003654A4(Ptr, Ptr* p) { return al::isFirstStep((const al::IUseNerve*)*p); }
extern "C" Word fn_003660D4(Ptr, Ptr* p) { return fn_0017ED04(*p); }
extern "C" Word fn_0036711C(Ptr, Ptr* p) { return fn_00212DE4(*p); }
extern "C" Word fn_00367124(Ptr, Ptr* p) { return fn_00212DE4(*p); }
extern "C" Word fn_003671E4(Ptr, Ptr* p) { return fn_00212DE4(*p); }
extern "C" Word fn_003671EC(Ptr, Ptr* p) { return fn_00212DE4(*p); }
extern "C" Word fn_00368FBC(Ptr, Ptr* p) { return fn_0018AD7C(*p); }
