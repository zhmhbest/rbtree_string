
/*!
 * @name 默认比较函数
 */
static int rbtree_fokc_default(RBTKeyType l, RBTKeyType r) {
    return l - r;
}

static void rbtree_init(RBTreeType tree) {
    tree->root = NULL;
    tree->fokc = rbtree_fokc_default;
}

static rbtree_interface_zhmh rbtree_methods = {
    rbtree_init,

    rbtree_min,
    rbtree_max,
    rbtree_prev,
    rbtree_next,

    rbtree_LL,
    rbtree_RR,

    rbtree_lookupKey,
    rbtree_lookupNode,

    rbtree_insertKey,
    rbtree_insertNode,

    rbtree_detachKey,
    rbtree_detachNode,

    rbtree_print
};

rbtree_interface_zhmh* rbtree_zhmh = &rbtree_methods;
