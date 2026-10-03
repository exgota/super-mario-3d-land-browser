namespace {
struct Pool;

struct Node {
    Node* next;
    unsigned int unknown[6];
    Pool* pool;
};

struct Pool {
    unsigned int unknown0;
    Node* freeHead;
    unsigned int unknown8;
    unsigned int activeCount;
};
}

extern "C" void fn_00399580(Node* node) {
    Pool* pool = node->pool;
    node->next = pool->freeHead;
    pool->freeHead = node;
    --pool->activeCount;
}

extern "C" void fn_003995A0(Node* node) {
    Pool* pool = node->pool;
    node->next = pool->freeHead;
    pool->freeHead = node;
    --pool->activeCount;
}

extern "C" void fn_003995C0(Node* node) {
    Pool* pool = node->pool;
    node->next = pool->freeHead;
    pool->freeHead = node;
    --pool->activeCount;
}

extern "C" void fn_003995E0(Node* node) {
    Pool* pool = node->pool;
    node->next = pool->freeHead;
    pool->freeHead = node;
    --pool->activeCount;
}
