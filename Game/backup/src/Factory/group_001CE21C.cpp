namespace {
struct Target {
    unsigned char pad[0x28];
    void* field_28;
};
struct Link { Target* target; };
struct Node { unsigned char pad[0x28]; Link* field_28; };
}

extern "C" int fn_001CE22C(void*);
extern "C" int fn_0024947C(void*);
extern "C" int fn_0024EB8C(void*);
extern "C" int fn_00262430(void*);
extern "C" int fn_002624AC(void*);
extern "C" int fn_00262568(void*);

extern "C" int fn_001CE21C(Node* self) { return fn_001CE22C(self->field_28->target->field_28); }
extern "C" int fn_0024946C(Node* self) { return fn_0024947C(self->field_28->target->field_28); }
extern "C" int fn_0024EB7C(Node* self) { return fn_0024EB8C(self->field_28->target->field_28); }
extern "C" int fn_00262420(Node* self) { return fn_00262430(self->field_28->target->field_28); }
extern "C" int fn_0026249C(Node* self) { return fn_002624AC(self->field_28->target->field_28); }
extern "C" int fn_00262558(Node* self) { return fn_00262568(self->field_28->target->field_28); }
