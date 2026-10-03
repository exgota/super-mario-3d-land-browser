namespace nn {
namespace fs {
namespace detail {
class FileSystemBase;
}
}
}

extern "C" {
extern nn::fs::detail::FileSystemBase* dat_003EF67C;
extern void* dat_003EF834;
}

namespace nn {
namespace fs {
namespace detail {
void RegisterGlobalFileSystemBase(FileSystemBase& fileSystem) {
    dat_003EF67C = &fileSystem;
}
}
}
}

extern "C" void fn_002B8370(void* value) {
    dat_003EF834 = value;
}
