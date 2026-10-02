namespace {
struct Node;
struct TreeNode {
    TreeNode* parent;
    TreeNode* child;
    TreeNode* next;
    TreeNode* prev;
    Node* node;
};
struct Node {
    char pad0[0x10];
    TreeNode tree;
    char pad1[0x18];
    void* heap;
};
}

extern "C" void fn_0028CDB4(void*);
extern "C" void fn_0028CD24(void*);
extern "C" void fn_00222934(Node*, void*);
extern "C" void _ZN4sead8TreeNode13detachSubTreeEv(TreeNode*);
extern "C" void _ZN4sead8TreeNode14pushFrontChildEPS0_(TreeNode*, TreeNode*);

extern "C" void fn_00222C44(Node* parent, Node* child) {
    if (parent->heap)
        fn_0028CDB4(parent->heap);
    _ZN4sead8TreeNode13detachSubTreeEv(&child->tree);
    child->heap = parent->heap;
    if (child->tree.child && child->tree.child->node) {
        Node* node = child->tree.child->node;
        void* heap = parent->heap;
        node->heap = heap;
        if (node->tree.child && node->tree.child->node)
            fn_00222934(node->tree.child->node, heap);
        if (node->tree.next && node->tree.next->node)
            fn_00222934(node->tree.next->node, heap);
    }
    TreeNode* parentTree = &parent->tree;
    TreeNode* childTree = child ? &child->tree : 0;
    _ZN4sead8TreeNode14pushFrontChildEPS0_(parentTree, childTree);
    if (parent->heap)
        fn_0028CD24(parent->heap);
}
