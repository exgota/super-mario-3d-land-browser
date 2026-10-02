namespace al {
void* createSceneObj(int);
}

namespace rp {
void* createCoinRotater() {
    return al::createSceneObj(7);
}
}
