/*

    1 {Case:1} if(NULL == parent) //插入的为空树
        node->color = Black;
        tree->root = node;

    2  Insert(node);

        2.1 {Case:2} if(Black == parent->color)
            node->color = Red;
            parent->left or parent->right = node;

        2.2 if(Red == parent->color)
            //Because : root->color == black && parent->color == red.
            //So      : Parent can not be root of tree. Grandpa must exist and be black.

            2.2.1 {Case:3} if(Red == uncle->color)
            *...........................→...........................*
            *............[B]............→............[R]............*
            *.............G.............→.............G.............*
            *............/.\............→............/.\............*
            *.........../...\...........→.........../...\...........*
            *........../.....\..........→........../.....\..........*
            *......[R]P.......U[R]......→......[B]P.......U[B]......*
            *......./.\.................→......./.\.................*
            *..[R]N...S[B]..............→..[R]N...S[B]..............*
            *...........................→...........................*
            SetColor(grandpa, Red);
            SetColor(parent, Black);
            SetColor(uncle, Black);
            node = grandpa; //G的父节点也可能是红色的，可能变成一个向上传递的双红问题。

            2.2.2 if(Black == uncle->color)

                2.2.2.1 {Case:4-A}
                    *...........................→...........................*
                    *............[B]............→............[B]............*
                    *.............G.............→.............P.............*
                    *............/.\............→............/.\............*
                    *.........../...\...........→.........../...\...........*
                    *........../.....\..........→........../.....\..........*
                    *......[R]P.......U[B]......→......[R]N.......G[R]......*
                    *......./.\.................→................./.\.......*
                    *..[R]N...S[B]..............→..............[B]S...U[B]..*
                    *...........................→...........................*
                    LL(tree, grandpa);
                    SetColor(grandpa, Red);
                    SetColor(parent, Black);

                    *...........................→...........................*
                    *............[B]............→............[B]............*
                    *.............G.............→.............P.............*
                    *............/.\............→............/.\............*
                    *.........../...\...........→.........../...\...........*
                    *........../.....\..........→........../.....\..........*
                    *......[B]U.......P[R]......→......[R]G.......N[R]......*
                    *................./.\.......→......./.\.................*
                    *..............[B]S...N[R]..→..[B]U...S[B]..............*
                    *...........................→...........................*
                    RR(tree, grandpa);
                    SetColor(grandpa, Red);
                    SetColor(parent, Black);

                2.2.2.2 {Case:4-B}
                    *...........................→...........................→...........................*
                    *............[B]............→............[B]............→............[B]............*
                    *.............G.............→.............G.............→.............N.............*
                    *............/.\............→............/.\............→............/.\............*
                    *.........../...\...........→.........../...\...........→.........../...\...........*
                    *........../.....\..........→........../.....\..........→........../.....\..........*
                    *......[R]P.......U[B]......→......[R]N.......U[B]......→......[R]P.......G[R]......*
                    *.........\.................→......./...................→...................\.......*
                    *.........N[R]..............→..[R]P.....................→.....................U[B]..*
                    *...........................→...........................→...........................*
                    RR(tree, parent);
                    LL(tree, grandpa);
                    SetColor(grandpa, Red);
                    SetColor(node, Black);

                    *...........................→...........................→...........................*
                    *............[B]............→............[B]............→............[B]............*
                    *.............G.............→.............G.............→.............N.............*
                    *............/.\............→............/.\............→............/.\............*
                    *.........../...\...........→.........../...\...........→.........../...\...........*
                    *........../.....\..........→........../.....\..........→........../.....\..........*
                    *......[B]U.......P[R]......→......[B]U.......N[R]......→......[R]G.......P[R]......*
                    *................./.........→...................\.......→......./...................*
                    *..............[R]N.........→.....................P[R]..→..[B]U.....................*
                    *...........................→...........................→...........................*
                    LL(tree, parent);
                    RR(tree, grandpa);
                    SetColor(grandpa, Red);
                    SetColor(node, Black);
*/

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

//{Case:3} 该情况可能导致冲突向上传递。
#define RBTM_InsertCaseRedUncle(_node, _parent, _grandpa, _uncle) \
    RBTM_SetColor((_grandpa), RBT_RED);  \
    RBTM_SetColor((_parent), RBT_BLACK); \
    RBTM_SetColor((_uncle), RBT_BLACK);  \
    (_node) = (_grandpa);

//{Case:4-B}
#define RBTM_InsertCaseBlackUncleB(_tree, _node, _parent, _which, _Rotate) \
if( _which ) {                          \
    _Rotate(_tree, _parent);            \
    _node   = _parent;                  \
    _parent = RBTM_GetParent(_node);    \
}

//{Case:4-A}
#define RBTM_InsertCaseBlackUncleA(_tree, _parent, _grandpa, _Rotate) \
    _Rotate(_tree, _grandpa);           \
    RBTM_SetColor(_grandpa, RBT_RED);   \
    RBTM_SetColor(_parent, RBT_BLACK);

static inline void __rbtree_InsertWhileRedParent(thisRBTree tree, thisRBTNode node) {
    thisRBTNode grandpa;
        thisRBTNode parent;
            //thisRBTNode node;
        //thisRBTNode uncle;
    for(;;) {
        parent  = RBTM_GetParent(node);
            if( RBTM_IsBlackOrNull(parent) ) break;
        grandpa = RBTM_GetParent(parent); //grandpa一定存在，不必判NULL。
        #ifdef __debug
            //printf("InsertWhileRedParent n:%d p:%d g:%d\n",node->key_int,parent->key_int,grandpa->key_int);
        #endif // __debug
        //==========================================================
        if( RBTM_IsLeftChild(parent) ) {
            //uncle = grandpa->right;
            if( RBTM_IsRed(grandpa->right) ) {
                RBTM_InsertCaseRedUncle(node, parent, grandpa, grandpa->right);
            } else {
                RBTM_InsertCaseBlackUncleB(tree, node, parent, RBTM_IsRightChild(node), rbtree_RR);
                RBTM_InsertCaseBlackUncleA(tree, parent, grandpa, rbtree_LL);
            }
        } else {
            //uncle = grandpa->left;
            if( RBTM_IsRed(grandpa->left) ) {
                RBTM_InsertCaseRedUncle(node, parent, grandpa, grandpa->left);
            } else {
                RBTM_InsertCaseBlackUncleB(tree, node, parent, RBTM_IsLeftChild(node), rbtree_LL);
                RBTM_InsertCaseBlackUncleA(tree, parent, grandpa, rbtree_RR);
            }
        }
        //==========================================================
    }
    RBTM_SetColor(tree->root, RBT_BLACK);
}

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

/*!
 * @name Insert Node
 *  @param tree
 *  @param wins
        1. NodeKey(wins)必须不在树中。
        2. wins->Parent 必须为待插入结点的父节点
 */
thisRBTNode rbtree_InsertNode(thisRBTree tree, thisRBTNode wins) {
    #define _parent (wins->parent)
    if(!(tree && wins)) {
        return NULL;
    }

    wins->left  = NULL;
    wins->right = NULL;

    if(!_parent) {
        //空树
        RBTM_SetRoot(tree, wins);
        RBTM_SetColor(wins, RBT_BLACK);
        return wins;
    }

    RBTM_InsertSetChild(tree, _parent, wins);
    RBTM_SetColor(wins, RBT_RED); //尽量把插入节点置成红色，不违反规则5。

    if( RBTM_IsRed(_parent) ) {
        __rbtree_InsertWhileRedParent(tree, wins);
    }

    return wins;
    #undef _parent
}

/*!
 * @name Insert Key
 *  @param tree
 *  @param key
 * @return 返回node在树中的位置。
 * @note node->data需要在外面赋值。
 */
thisRBTNode rbtree_InsertKey(thisRBTree tree, intptr_t key) {
    if(!tree) {
        return NULL;
    }

    thisRBTNode node;
    thisRBTNode parent;

    //已存在
    node = rbtree_LookupKey(tree, key, &parent);
    if(node) {
        return node;
    }

    //新建
    node = RBTM_NewNode;
    if(!node) {
        return NULL;
    } else {
        NodeKey(node) = key;
        NodeDat(node).first = NULL;
        NodeDat(node).length = 0;
        node->parent  = parent;
    }

    return rbtree_InsertNode(tree, node);
}
