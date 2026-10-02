namespace {
typedef unsigned int u32;
struct Inner { u32 pad[13]; u32 value; };
struct Link { Inner *next; };
struct Owner { char pad[0x28]; Link *link; };
}

extern "C" u32 fn_0024EB8C(u32);
extern "C" u32 fn_0024947C(u32);
extern "C" u32 fn_002624AC(u32);
extern "C" u32 fn_00243A00(u32);
extern "C" u32 fn_0024AD3C(u32);
extern "C" u32 fn_0024E9A8(u32);

extern "C" u32 fn_001C2F0C(Owner *self) { return fn_0024EB8C(self->link->next->value); }
extern "C" u32 fn_001D2CD8(Owner *self) { return fn_0024947C(self->link->next->value); }
extern "C" u32 fn_001DB54C(Owner *self) { return fn_002624AC(self->link->next->value); }
extern "C" u32 fn_00253A3C(Owner *self) { return fn_00243A00(self->link->next->value); }
extern "C" u32 fn_00253A4C(Owner *self) { return fn_0024AD3C(self->link->next->value); }
extern "C" u32 fn_00260510(Owner *self) { return fn_0024E9A8(self->link->next->value); }
