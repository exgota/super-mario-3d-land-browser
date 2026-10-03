namespace {
struct Slot4 {
    unsigned int reserved;
    void* value;
};
}

extern "C" void fn_001EAF2C(Slot4* self, void* value) {
    self->value = value;
}

namespace nw {
namespace snd {
class SoundArchive;
namespace internal {
class SoundArchiveLoader {
public:
    void SetSoundArchive(const SoundArchive* archive);
private:
    unsigned int mReserved;
    const SoundArchive* mArchive;
};
void SoundArchiveLoader::SetSoundArchive(const SoundArchive* archive) {
    mArchive = archive;
}
}
}
}

extern "C" void fn_002DAB04(Slot4* self, void* value) {
    self->value = value;
}

extern "C" void fn_002E1674(Slot4* self, void* value) {
    self->value = value;
}
