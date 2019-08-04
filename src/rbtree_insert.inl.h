

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

//#define rbtree_insert_PRINTSTEP

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

static inline void rbtree_insertWhileRedParent(RBTreeType tree, RBTNodeType node);
#include "rbtree_insert_RedParent.inl.h"

/*!
 * @name Insert Node
 * @warning 这是“Insert Key”的子进程！
 * @warning 这不是一个建议直接使用的方法！
 *  @param tree
 *  @param wins
 *    @warning wins->key    必须已被初始化。
 *    @warning wins->parent 必须已被初始化。
 *    @warning wins->parent 必须没有满堂。
 * @return node
 */

static RBTNodeType rbtree_insertNode(RBTreeType tree, RBTNodeType wins) {
    #define wins_key    (wins->key)
    #define wins_parent (wins->parent)
    if(!(tree && wins)) return NULL;

    //Initialize
    wins->left  = NULL;
    wins->right = NULL;

    if(!wins_parent) {
        #ifdef rbtree_insert_PRINTSTEP
            printf("Insert: {Case:1}\n");
        #endif // rbtree_insert_PRINTSTEP
        RBTM_SetRoot(tree, wins);
        RBTM_SetColor(wins, RBT_BLACK);
        return wins;
    }

    //SetChild
    {
        int ret = tree->fokc(wins_parent->key.i, wins_key.i);
        if(ret>0) {
            wins_parent->left  = wins;
        } else {
            wins_parent->right = wins;
        }
    }

    RBTM_SetColor(wins, RBT_RED); //尽量把插入节点置成红色，不违反规则5。
    if( RBTM_IsRed(wins_parent) ) {
        //{Case: 3, 4}
        rbtree_insertWhileRedParent(tree, wins);
    }
    #ifdef rbtree_insert_PRINTSTEP
    else {
        printf("Insert: {Case:2}\n");
    }
    #endif // rbtree_insert_PRINTSTEP

    return wins;
    #undef wins_key
    #undef wins_parent
}

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

/*!
 * @name Insert Key
 *  @param tree
 *  @param key
 * @return node
 * @warning node->data需要在外面赋值。
 */
static RBTNodeType rbtree_insertKey(RBTreeType tree, RBTKeyType key) {
    if(!tree) return NULL;

    RBTNodeType node;
    RBTNodeType parent;

    //已存在
    node = rbtree_lookupKey(tree, key, &parent);
    if(node) return node;

    //新建
    node = RBTM_NewNode;
    if(!node) {
        return NULL;
    } else {
        node->key.i = key;
        node->data.first  = NULL;
        node->data.length = 0;
        node->parent  = parent; //parent一定没有满堂！
        //node->left  = NULL;
        //node->right = NULL;
    }

    return rbtree_insertNode(tree, node);
}
