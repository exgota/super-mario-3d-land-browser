namespace {
struct Stream;
struct StreamVtable {
    void* slots0[9];
    unsigned (*read)(Stream*, void*, unsigned);
    void* slots1[6];
    void (*seek)(Stream*, int, int);
};
struct Stream {
    StreamVtable* vtable;
};
struct Header {
    unsigned words[2];
};
struct Reader {
    Stream* stream;
    Header header;
};
struct Scratch {
    Header header;
    unsigned char storage[96];
};
}

extern "C" void fn_002C1600(Header*);
extern "C" bool fn_0033FD44(Header*, const void*);
extern "C" unsigned fn_0033ED60(const void*);
extern "C" unsigned fn_0033ECE8(const void*);
extern "C" void fn_002C15AC(Header*, const void*);

extern "C" bool fn_002C13D0(Reader* self, void* destination, unsigned capacity) {
    Scratch scratch;
    void* aligned;
    unsigned size;
    self->stream->vtable->seek(self->stream, 0, 0);
    if (self->stream->vtable->read(self->stream,
        reinterpret_cast<void*>(reinterpret_cast<unsigned>(scratch.storage + 31) & ~31u),
        64) != 64)
        goto failure;
    aligned = reinterpret_cast<void*>(reinterpret_cast<unsigned>(scratch.storage + 31) & ~31u);
    fn_002C1600(&scratch.header);
    if (!fn_0033FD44(&scratch.header, aligned))
        goto failure;
    size = fn_0033ED60(aligned) + fn_0033ECE8(aligned);
    if (size > capacity)
        goto failure;
    self->stream->vtable->seek(self->stream, 0, 0);
    if (self->stream->vtable->read(self->stream, destination, size) == size)
        goto success;
failure:
    return false;
success:
    fn_002C15AC(&self->header, destination);
    return true;
}
