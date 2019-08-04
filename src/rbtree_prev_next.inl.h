
/*!
 * @name GetMin
 * @param _io_node : input node , output MinNode
 */
#define RBTM_GetMin(_io_node)       while(_io_node->left) {_io_node =  _io_node->left;}

/*!
 * @name GetMax
 * @param _io_node : input node , output MaxNode
 */
#define RBTM_GetMax(_io_node)       while(_io_node->right) {_io_node =  _io_node->right;}

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

static RBTNodeType rbtree_min(RBTreeType tree) {
    if(tree && tree->root) {
        RBTNodeType node = tree->root;
        RBTM_GetMin(node);
        return node;
    }
    return NULL;
}

static RBTNodeType rbtree_max(RBTreeType tree) {
    if(tree && tree->root) {
        RBTNodeType node = tree->root;
        RBTM_GetMax(node);
        return node;
    }
    return NULL;
}

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

/*!
 * @note 最大前驱
 */
static RBTNodeType rbtree_prev(RBTNodeType node) {
    if(!node) return NULL;
    if(node->left) {
        node = node->left;
        RBTM_GetMax(node);
        return node;
    } else {
        //Loop : while ( exist parent && is left child )
        while( RBTM_ExistParent(node) && RBTM_IsLeftChild(node) ) {
            node =  RBTM_GetParent(node);
        }
        return RBTM_GetParent(node); //此处也可能是NULL
    }
}

/*!
 * @note 最小后继
 */
static RBTNodeType rbtree_next(RBTNodeType node) {
    if(!node) return NULL;
    if(node->right) {
        node = node->right;
        RBTM_GetMin(node);
        return node;
    } else {
        //Loop : while ( exist parent && is right child )
        while( RBTM_ExistParent(node) && RBTM_IsRightChild(node) ) {
            node =  RBTM_GetParent(node);
        }
        return RBTM_GetParent(node); //此处也可能是NULL
    }
}
