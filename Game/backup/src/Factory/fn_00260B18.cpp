namespace {
struct ByamlIter {
    const void* data;
    const void* node;
};
struct RailKeeper {
    void* rail;
    void* rider;
};
}

extern "C" {
int _ZNK2al9ByamlIter7getSizeEv(const ByamlIter*);
void _ZN2al9ByamlIterC1Ev(ByamlIter*);
bool _ZNK2al9ByamlIter17tryGetIterByIndexEPS0_i(const ByamlIter*, ByamlIter*, int);
bool _ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc(const ByamlIter*, int*, const char*);
void* _ZnwjRKSt9nothrow_t(unsigned int);
RailKeeper* _ZN2al10RailKeeperC1ERKNS_9ByamlIterE(RailKeeper*, const ByamlIter*);
bool _ZN2al10tryGetArg7EPbRKNS_9ByamlIterE(bool*, const ByamlIter*);
}

extern "C" void fn_00260B18(RailKeeper** keeper, float* arg, bool* flag,
                            const ByamlIter* rails, int id) {
    const int size = _ZNK2al9ByamlIter7getSizeEv(rails);
    for (int i = 0; i < size; ++i) {
        ByamlIter iter;
        _ZN2al9ByamlIterC1Ev(&iter);
        _ZNK2al9ByamlIter17tryGetIterByIndexEPS0_i(rails, &iter, i);
        int railId = -1;
        _ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc(&iter, &railId, "l_id");
        if (id == railId) {
            RailKeeper* result = static_cast<RailKeeper*>(_ZnwjRKSt9nothrow_t(sizeof(RailKeeper)));
            if (result)
                result = _ZN2al10RailKeeperC1ERKNS_9ByamlIterE(result, &iter);
            *keeper = result;
            int value = static_cast<int>(*arg);
            _ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc(&iter, &value, "Arg0");
            *arg = static_cast<float>(value);
            if (flag) {
                bool value = false;
                _ZN2al10tryGetArg7EPbRKNS_9ByamlIterE(&value, &iter);
                *flag = value;
            }
            return;
        }
    }
}
