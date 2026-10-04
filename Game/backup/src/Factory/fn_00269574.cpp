#include <Util/alStringUtil.h>

struct StringSource {
    unsigned char _pad[0x10];
    const char* value;
};

struct StringSourceOwner {
    unsigned char _pad[0x28];
    StringSource* source;
};

extern "C" bool fn_00269574(const StringSourceOwner* self, const char* value) {
    return al::isEqualString(self->source->value, value);
}
