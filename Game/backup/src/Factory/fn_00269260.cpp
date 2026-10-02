namespace {
struct PositionSource {
    unsigned char padding[16];
    float x;
    float y;
    float z;
};
}

namespace rp {
extern void* getPlayerPos();
}

extern "C" void fn_00269290(void*, void*, void*, float, float, float);

extern "C" void fn_00269260(void* first, void* second, PositionSource* source) {
    void* player = rp::getPlayerPos();
    return fn_00269290(first, player, second, source->x, source->y, source->z);
}
