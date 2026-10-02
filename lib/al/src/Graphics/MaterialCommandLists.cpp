// Original EU 0x001141CC. Clean-room material command-list construction.
// NON_MATCHING: only the canonical project checker may establish exactness.
#include <stddef.h>
namespace material1141cc {
typedef unsigned Word;
struct Material { Word words[0x778 / 4]; };
struct Owner { unsigned char unknown[0x60]; unsigned selected; Word unknown64[2]; Material* material; void* lists[1]; };
struct CommandBuffer { Word unknown; Word handle; Word base; int resource; unsigned char ready; unsigned char padding[3]; Word length; Word capacity; };
struct Packet4 { Word words[4]; };
struct Packet6 { Word words[6]; };
static_assert_(offsetof(Owner, material) == 0x6c);
static_assert_(offsetof(Owner, lists) == 0x70);
inline Word& word(Material* p, unsigned offset) { return p->words[offset / 4]; }
inline void* at(Material* p, unsigned offset) { return reinterpret_cast<char*>(p) + offset; }
inline Word* words(unsigned address) { return reinterpret_cast<Word*>(address); }
inline const void* data(unsigned address) { return reinterpret_cast<const void*>(address); }
}
extern "C" {
void fn_0028ED00(unsigned, void*);
void fn_00281A54(unsigned);
void fn_0028F864(unsigned, void*);
void fn_0028F6F0(unsigned);
void fn_0028F618(unsigned, unsigned);
void fn_002847E4(unsigned);
void fn_002819D0();
unsigned glGetError();
void fn_002819A8(void*);
void fn_00281978(unsigned);
void nngxAdd3DCommand(const void*, unsigned, unsigned);
unsigned fn_00281874(material1141cc::Owner*, void*, unsigned);
void fn_0028140C(void*, void*);
unsigned fn_002877FC();
void fn_00281360(unsigned*, void*, void*, void*);
void fn_00284894(unsigned);
}
namespace material1141cc {
inline unsigned query(unsigned parameter) {
    unsigned value;
    fn_0028ED00(parameter, &value);
    return value;
}
inline void append(Material* material, unsigned countOffset, unsigned entriesOffset, unsigned base) {
    unsigned offset;
    fn_0028ED00(0x202, &offset);
    unsigned count = word(material, countOffset);
    word(material, countOffset) = count + 1;
    reinterpret_cast<Word*>(at(material, entriesOffset))[count] = base + (offset & ~3u);
}
inline void mode(unsigned value) {
    Word command[2] = {value, words(0x003a7a68)[0x3c / 4]};
    nngxAdd3DCommand(command, sizeof(command), 1);
}
inline void createBuffer(CommandBuffer* buffer, unsigned size, unsigned* previous) {
    fn_0028ED00(0x207, previous);
    fn_0028F864(1, &buffer->handle);
    fn_0028F6F0(buffer->handle);
    fn_0028F618(size, 0x20);
    fn_0028ED00(0x206, &buffer->base);
    fn_0028F6F0(*previous);
    fn_0028ED00(0x207, words(0x003ef074));
    fn_0028F6F0(buffer->handle);
    if (buffer->resource > 0) {
        fn_002847E4(buffer->resource);
        buffer->resource = 0;
    }
    fn_002819D0();
    glGetError();
    fn_002819A8(words(0x003ef078));
    fn_00281978(0x801);
}
inline void finishBuffer(CommandBuffer* buffer, unsigned* previous) {
    buffer->ready = 1;
    fn_00281360(previous, &buffer->resource, &buffer->length, &buffer->capacity);
    glGetError();
    fn_0028F6F0(*words(0x003ef074));
    fn_00281978(*words(0x003ef078));
    glGetError();
}
}
#ifdef NON_MATCHING
extern "C" void fn_001141CC(material1141cc::Owner* owner) {
    using namespace material1141cc;
    Material* initial = owner->material;
    word(initial, 0x6c8) = 0;
    word(initial, 0x6cc) = 0;
    word(initial, 0x710) = 0;
    word(initial, 0x734) = 0;
    word(initial, 0x738) = 0;
    word(initial, 0x75c) = 0;
    word(initial, 0x774) = 0;
    unsigned saved;
    fn_0028ED00(0x201, &saved);
    if (saved) fn_00281A54(saved);
    unsigned previous;
    createBuffer(reinterpret_cast<CommandBuffer*>(at(initial, 0x690)), 0x1000, &previous);
    {
        Material* material = owner->material;
        Word* context = words(0x004244a8);
        unsigned sign = word(material, 0x68c) >> 31;
        unsigned base = query(0x206);
        nngxAdd3DCommand(data(0x003ef084), 0x38, 1);
        append(material, 0x6c8, 0x6d0, base);
        mode(2);
        {
        Packet4 command = *reinterpret_cast<const Packet4*>(data(0x003a84e4));
        unsigned identifier = context[0x50 / 4];
        command.words[0] = identifier | (identifier << 16) | 0x0ee00ee0;
        nngxAdd3DCommand(&command, sizeof(command), 1);
        }
        fn_00281874(owner, at(material, 0x414), base);
        fn_0028140C(at(material, 0xe8), context);
        {
            Material* refreshed = owner->material;
            {
            Packet4 begin = *reinterpret_cast<const Packet4*>(data(0x003a7e30));
            nngxAdd3DCommand(&begin, sizeof(begin), 1);
            }
            append(refreshed, 0x75c, 0x760, base);
            {
            Packet4 end = *reinterpret_cast<const Packet4*>(data(0x003a7e40));
            nngxAdd3DCommand(&end, sizeof(end), 1);
            }
        }
        {
        Packet6 state = *reinterpret_cast<const Packet6*>(data(0x003a84f4));
        state.words[4] = sign ? *words(0x003ef06c) : words(0x003a7d38)[word(material, 0x5dc)];
        nngxAdd3DCommand(&state, sizeof(state), 1);
        }
        fn_0028140C(material, context);
        int index = static_cast<int>(word(material, 0x564));
        if (index > 5) index = 5;
        unsigned color = words(0x003a822c)[index];
        if (!fn_002877FC()) {
            unsigned red = (color & 255) >> 3;
            unsigned green = ((color >> 8) & 255) >> 2;
            unsigned blue = ((color >> 16) & 255) >> 3;
            color = ((red << 3) | (red >> 2)) | (((green << 2) | (green >> 4)) << 8)
                | (((blue << 3) | (blue >> 2)) << 16) | 0xff000000;
        }
        {
        Word colorCommand[2] = {color, words(0x003a7a68)[0x44 / 4]};
        nngxAdd3DCommand(colorCommand, sizeof(colorCommand), 1);
        }
        fn_0028140C(at(material, 0x3a0), context);
        if (word(material, 0x578)) {
            append(material, 0x6cc, 0x6f0, base);
            mode(1);
        }
        fn_0028140C(at(material, 0x15c), context);
        {
        unsigned position;
        fn_0028ED00(0x202, &position);
        word(material, 0x774) = base + (position & ~3u);
        Word parameterCommand[2] = {word(material, 0x770), words(0x003a7a68)[0x4c / 4]};
        nngxAdd3DCommand(parameterCommand, sizeof(parameterCommand), 1);
        }
        fn_0028140C(at(material, 0x244), context);
        if (word(material, 0x448)) {
            int selected = static_cast<int>(word(material, 0x648));
            if (selected > 11) selected = 11;
            unsigned rgba = words(0x003a8284)[selected];
            Packet6 blend = *reinterpret_cast<const Packet6*>(data(0x003a850c));
            blend.words[0] = context[0x50 / 4] | 0x0eee0ee0;
            blend.words[4] = ((rgba & 255) >> 1) | ((((rgba >> 8) & 255) >> 1) << 8)
                | ((((rgba >> 16) & 255) >> 1) << 16) | (rgba & 0xff000000);
            nngxAdd3DCommand(&blend, sizeof(blend), 1);
            fn_00281874(owner, at(material, 0x448), base);
            fn_0028140C(at(material, 0x74), context);
        }
        if (word(material, 0x578)) {
            append(material, 0x6c8, 0x6d0, base);
            mode(2);
        }
        nngxAdd3DCommand(data(0x003ef0f4), 0x18, 1);
    }
    finishBuffer(reinterpret_cast<CommandBuffer*>(at(initial, 0x690)), &previous);
    createBuffer(reinterpret_cast<CommandBuffer*>(at(initial, 0x6ac)), 0x800, &previous);
    {
        Word* context = words(0x004244a8);
        Material* material = owner->material;
        unsigned base = query(0x206);
        nngxAdd3DCommand(data(0x003ef0bc), 0x38, 1);
        append(material, 0x6c8, 0x6d0, base);
        mode(2);
        {
        Packet4 command = *reinterpret_cast<const Packet4*>(data(0x003a8524));
        unsigned identifier = context[0x50 / 4];
        command.words[0] = identifier | (identifier << 16) | 0x0ee00ee0;
        nngxAdd3DCommand(&command, sizeof(command), 1);
        }
        void* selected = owner->lists[owner->selected];
        if (selected) {
            word(material, 0x734) = fn_00281874(owner, selected, base);
            fn_0028140C(at(material, 0x2b8), context);
        }
        fn_00281874(owner, at(material, 0x47c), base);
        fn_0028140C(at(material, 0x32c), context);
        if (word(material, 0x4b0)) {
            {
        Packet6 state = *reinterpret_cast<const Packet6*>(data(0x003a8534));
            state.words[4] = words(0x003a7d58)[word(material, 0x5ec)];
            nngxAdd3DCommand(&state, sizeof(state), 1);
        }
            fn_00281874(owner, at(material, 0x4b0), base);
            fn_0028140C(at(material, 0x1d0), context);
        }
        nngxAdd3DCommand(data(0x003ef0f4), 0x18, 1);
    }
    finishBuffer(reinterpret_cast<CommandBuffer*>(at(initial, 0x6ac)), &previous);
    if (saved) fn_00284894(saved);
}
#endif
