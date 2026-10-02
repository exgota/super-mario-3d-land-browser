extern "C" int *__rt_errno_adddr();

extern "C" int __read_errno() {
    return *__rt_errno_adddr();
}
