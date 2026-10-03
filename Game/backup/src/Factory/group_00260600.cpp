namespace {
extern "C" int fn_00277CEC(void*);
extern "C" char dat_003B9534[];
extern "C" char dat_003B94C4[];
}

extern "C" const char* fn_00260600(void* object) {
    return fn_00277CEC(object) ? dat_003B9534 : dat_003B94C4;
}
