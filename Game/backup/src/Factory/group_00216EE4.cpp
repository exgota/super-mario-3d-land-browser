#include <cstdint>

namespace {
struct CourseList;

struct ReceiverFields {
    std::uint32_t first;
    std::uint32_t second;
};

struct Receiver {
    std::uint32_t unknown;
    ReceiverFields fields;
};
}

namespace rp {
CourseList* getCourseList();
}

extern "C" std::uintptr_t fn_00216F00(std::uintptr_t, std::uint32_t, std::uint32_t);
extern "C" std::uintptr_t fn_0026AF34(std::uintptr_t, std::uint32_t, std::uint32_t);

extern "C" std::uintptr_t fn_00216EE4(Receiver* self) {
    return fn_00216F00(reinterpret_cast<std::uintptr_t>(rp::getCourseList()),
                        self->fields.first, self->fields.second);
}

extern "C" std::uintptr_t fn_0026AF18(Receiver* self) {
    return fn_0026AF34(reinterpret_cast<std::uintptr_t>(rp::getCourseList()),
                        self->fields.first, self->fields.second);
}
