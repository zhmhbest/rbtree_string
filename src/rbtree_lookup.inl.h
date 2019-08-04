
/*!
 * @name lookup key
 */
static RBTNodeType rbtree_lookupKey(RBTreeType tree, RBTKeyType key, RBTNodeType* pparent) {
    if(pparent) *pparent = NULL;
    if(!tree) return NULL;
    {
        int ret;
        RBTNodeType current = tree->root;
        while(current) {
            ret = tree->fokc(current->key.i, key);
            if(0==ret) {
                return  current;
            }
            if(pparent) {
                *pparent = current;
            }
            current = (ret>0) ? current->left : current->right ;
        }
    }
    return NULL;
}

/*!
 * @name lookup node
 * @note (outputNode == inputNode)判断是否绝对相等
 * @note 方法仅判断了Key。
 */
static RBTNodeType rbtree_lookupNode(RBTreeType tree, RBTNodeType node, RBTNodeType* pparent) {
    return rbtree_lookupKey(tree, node->key.i, pparent);
}
