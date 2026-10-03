namespace {
struct CourseList;
}

namespace rp {
extern CourseList* getCourseList();
}

extern "C" void* fn_002527F8(CourseList*, int, int);
extern "C" void* fn_0026AFA0(CourseList*, int, int);

extern "C" void* fn_002527D8(void*, int arg1, int arg2) {
    return fn_002527F8(rp::getCourseList(), arg1, arg2);
}

extern "C" void* fn_0026AF80(void*, int arg1, int arg2) {
    return fn_0026AFA0(rp::getCourseList(), arg1, arg2);
}
