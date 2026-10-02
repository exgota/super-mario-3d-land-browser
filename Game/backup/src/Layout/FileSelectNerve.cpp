namespace al { class Nerve; }
extern "C" const al::Nerve dat_003F1180;
extern "C" const al::Nerve dat_003F1184;
extern "C" const al::Nerve dat_003E2E0C;

#include <Layout/FileSelectOperation.h>
#include <Nerve/alNerveFunction.h>

extern "C" void fn_0014AED4(const al::Nerve*, al::NerveKeeper* keeper) {
    fn_0014AEDC(reinterpret_cast<FileSelectOperationState*>(keeper->getHost()));
}

extern "C" bool fn_00264930(const al::IUseNerve* state) {
    return al::isNerve(state, &dat_003F1180);
}

extern "C" bool fn_00264950(const al::IUseNerve* state) {
    return al::isNerve(state, &dat_003F1184);
}

extern "C" void fn_002744FC(al::IUseNerve* state) {
    al::setNerve(state, &dat_003E2E0C);
}
