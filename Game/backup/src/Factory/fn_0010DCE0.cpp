namespace {
struct ReceivedPacket;
}

extern "C" int CFLi_SetReceivedPacketAsync(ReceivedPacket*, const void*, unsigned int, int);
extern "C" int fn_00287778(int);

extern "C" int CFL_SetReceivedPacket(ReceivedPacket* packet, const void* data, unsigned int size)
{
    int result = CFLi_SetReceivedPacketAsync(packet, data, size, 1);
    if (result == 10)
        return fn_00287778(result);
    return result;
}
