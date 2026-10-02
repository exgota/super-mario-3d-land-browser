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

void TreeNode::pushBackChild( TreeNode* node )
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

void TreeNode::detachAll()
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
