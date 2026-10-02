namespace {
typedef void* Arg;
}
namespace nn { namespace gxlow { namespace CTR {
struct InterruptReceiver { void WaitAnyHandlerDone(); };
void YieldThread() { reinterpret_cast<InterruptReceiver*>(0x0041D1B0)->WaitAnyHandlerDone(); }
}}}
