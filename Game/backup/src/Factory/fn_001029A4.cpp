namespace nn { namespace os {
class Thread {
public:
    static void NoParameterFunc(void (*func)());
};
void Thread::NoParameterFunc(void (*func)()) {
    func();
}
} }
