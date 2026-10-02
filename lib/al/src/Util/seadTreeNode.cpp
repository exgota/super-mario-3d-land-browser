namespace sead { class MethodTreeNode; }
extern "C" void fn_00222934(sead::MethodTreeNode*, void*);
extern "C" void fn_0028CD24(void*);
extern "C" void fn_0028CDB4(void*);

namespace sead
{

class TreeNode
{
public:
        TreeNode();
        void detachSubTree();
        void pushFrontChild( TreeNode* node );
        void pushBackChild( TreeNode* node );
        void detachAll();

private:
        friend class MethodTreeNode;
        friend void ::fn_00222934(sead::MethodTreeNode*, void*);
        void clearChildLinksRecursively_();
        TreeNode* mParent;
        TreeNode* mFirstChild;
        TreeNode* mNextSibling;
        TreeNode* mPreviousSibling;
};

TreeNode::TreeNode()
{
        mParent = mFirstChild = mNextSibling = mPreviousSibling = 0;
}

__attribute__((noinline)) void TreeNode::clearChildLinksRecursively_()
{
        TreeNode* child = mFirstChild;
        while ( child )
        {
                TreeNode* current = child;
                child = child->mNextSibling;
                current->clearChildLinksRecursively_();
                current->mParent = current->mFirstChild = current->mNextSibling = current->mPreviousSibling = 0;
        }
}

// Preserve the out-of-line call observed in pushFrontChild.
__attribute__((noinline)) void TreeNode::detachSubTree()
{
        if ( mPreviousSibling )
        {
                mPreviousSibling->mNextSibling = mNextSibling;
                if ( mNextSibling )
                {
                        mNextSibling->mPreviousSibling = mPreviousSibling;
                        mNextSibling = 0;
                }
                mPreviousSibling = 0;
                mParent = 0;
        }
        else
        {
                if ( mParent )
                {
                        mParent->mFirstChild = mNextSibling;
                        mParent = 0;
                }
                if ( mNextSibling )
                {
                        mNextSibling->mPreviousSibling = mPreviousSibling;
                        mNextSibling = 0;
                }
        }
}

void TreeNode::pushFrontChild( TreeNode* node )
{
        node->detachSubTree();
        if ( !mFirstChild )
        {
                mFirstChild = node;
                node->mParent = this;
        }
        else
        {
                node->mNextSibling = mFirstChild;
                mFirstChild->mPreviousSibling = node;
                mFirstChild = node;
                node->mParent = this;
        }
}

__attribute__((noinline)) void TreeNode::pushBackChild( TreeNode* node )
{
        node->detachSubTree();
        TreeNode* current = mFirstChild;
        if ( !current )
        {
                mFirstChild = node;
                node->mParent = this;
        }
        else
        {
                node->detachSubTree();
                while ( current->mNextSibling )
                        current = current->mNextSibling;
                current->mNextSibling = node;
                node->mPreviousSibling = current;
                node->mParent = current->mParent;
        }
}

__attribute__((noinline)) void TreeNode::detachAll()
{
        detachSubTree();
        TreeNode* child = mFirstChild;
        while ( child )
        {
                TreeNode* current = child;
                child = child->mNextSibling;
                current->clearChildLinksRecursively_();
                current->mParent = current->mFirstChild = current->mNextSibling = current->mPreviousSibling = 0;
        }
        mParent = mFirstChild = mNextSibling = mPreviousSibling = 0;
}

} // namespace sead

// Observed MethodTreeNode prefix. No inherited/public class or table is rebuilt.
namespace sead {
class MethodTreeNode;
struct MethodTreeLink : TreeNode { MethodTreeNode* owner; };
struct MethodCallbackRecord;
struct MethodCallbackDispatch { void (*invoke)(MethodCallbackRecord*); };
struct MethodCallbackRecord {
    const MethodCallbackDispatch* dispatch;
    unsigned char unknown04[12];
};
class MethodTreeNode {
public:
    void call();
    void callRec_();
    void pushBackChild(MethodTreeNode* node);
    void unlock_();
    void lock_();

    unsigned char unknown00[16];
    MethodTreeLink tree;
    unsigned char unknown24[8];
    MethodCallbackRecord callback;
    void* criticalSection;
    unsigned int unknown40;
    unsigned int pause;
    unsigned char unknown48[8];
};

__attribute__((noinline)) void MethodTreeNode::callRec_() {
    if (!(pause & 1))
        callback.dispatch->invoke(&callback);
    TreeNode* child = tree.mFirstChild;
    if (child && !((pause >> 1) & 1)) {
        do {
            static_cast<MethodTreeLink*>(child)->owner->callRec_();
            child = static_cast<MethodTreeLink*>(child)->owner->tree.mNextSibling;
        } while (child);
    }
}
void MethodTreeNode::call() {
    if (criticalSection)
        fn_0028CDB4(criticalSection);
    if (!(pause & 1))
        callback.dispatch->invoke(&callback);
    TreeNode* child = tree.mFirstChild;
    if (child && !((pause >> 1) & 1)) {
        do {
            static_cast<MethodTreeLink*>(child)->owner->callRec_();
            child = static_cast<MethodTreeLink*>(child)->owner->tree.mNextSibling;
        } while (child);
    }
    if (criticalSection)
        fn_0028CD24(criticalSection);
}
void MethodTreeNode::unlock_() {
    if (criticalSection)
        fn_0028CD24(criticalSection);
}
void MethodTreeNode::lock_() {
    if (criticalSection)
        fn_0028CDB4(criticalSection);
}
void MethodTreeNode::pushBackChild(MethodTreeNode* node) {
    if (criticalSection)
        fn_0028CDB4(criticalSection);
    node->tree.detachSubTree();
    node->criticalSection = criticalSection;
    MethodTreeNode* child = node->tree.mFirstChild ? static_cast<MethodTreeLink*>(node->tree.mFirstChild)->owner : 0;
    if (child) {
        void* section = criticalSection;
        child->criticalSection = section;
        MethodTreeNode* first = child->tree.mFirstChild ? static_cast<MethodTreeLink*>(child->tree.mFirstChild)->owner : 0;
        if (first)
            fn_00222934(first, section);
        MethodTreeNode* next = child->tree.mNextSibling ? static_cast<MethodTreeLink*>(child->tree.mNextSibling)->owner : 0;
        if (next)
            fn_00222934(next, section);
    }
    TreeNode& parent = tree;
    TreeNode* incoming = node ? &node->tree : 0;
    parent.pushBackChild(incoming);
    if (criticalSection)
        fn_0028CD24(criticalSection);
}
}
extern "C" __attribute__((noinline)) void fn_00222934(sead::MethodTreeNode* node, void* section) {
    node->criticalSection = section;
    sead::MethodTreeNode* child = node->tree.mFirstChild ? static_cast<sead::MethodTreeLink*>(node->tree.mFirstChild)->owner : 0;
    if (child)
        fn_00222934(child, section);
    sead::MethodTreeNode* sibling = node->tree.mNextSibling ? static_cast<sead::MethodTreeLink*>(node->tree.mNextSibling)->owner : 0;
    if (sibling)
        fn_00222934(sibling, section);
}
