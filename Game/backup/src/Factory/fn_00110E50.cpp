namespace {
struct Message {
    unsigned char unknown[0xC];
    void* allocation;
};
}

extern "C" void LMSi_Free(void* allocation);

extern "C" void LMS_CloseMessage(Message* message) {
    if (message->allocation)
        LMSi_Free(message->allocation);
    return LMSi_Free(message);
}
