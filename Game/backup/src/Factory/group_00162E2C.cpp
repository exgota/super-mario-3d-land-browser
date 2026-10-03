extern "C" void fn_00162E34(void *, int);
extern "C" void ena_enableStatus(void *, int);

extern "C" void fn_00162E2C(void *arg) {
    fn_00162E34(arg, 1);
}

extern "C" void glEnable(void *arg) {
    ena_enableStatus(arg, 1);
}
