namespace sead { template <class T> struct Vector3; }
namespace {
struct Slot { unsigned char pad[4]; void* value; };
struct Owner { unsigned char pad[0x40]; Slot* slot; };
}
namespace al {
class RailRider {
public:
    void moveToRailStart();
    void moveToNearestRail(const sead::Vector3<float>&);
};
}
extern "C" void fn_003376A4(void*, void*);
extern "C" void fn_003376EC(void*, void*);

extern "C" void fn_001EC0E8(Owner* self) {
    self = reinterpret_cast<Owner*>(self->slot->value);
    reinterpret_cast<al::RailRider*>(self)->moveToRailStart();
}
extern "C" void fn_00242A38(Owner* self, const sead::Vector3<float>& position) {
    self = reinterpret_cast<Owner*>(self->slot->value);
    reinterpret_cast<al::RailRider*>(self)->moveToNearestRail(position);
}
extern "C" void fn_00337698(Owner* self, void* arg) {
    fn_003376A4(self->slot->value, arg);
}
extern "C" void fn_003376E0(Owner* self, void* arg) {
    fn_003376EC(self->slot->value, arg);
}
