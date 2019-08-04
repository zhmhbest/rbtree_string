//!@file 这是 detach 的一个子过程。

static void inline rbtree_detachBlackLeafSub(RBTreeType tree, RBTNodeType node);
static void rbtree_detachBlackLeaf(RBTreeType tree, RBTNodeType node) {
    //再包裹一次是为了防止无限递归
    if(node->parent) {
        rbtree_detachBlackLeafSub(tree, node);
    }
}

//! {Case:5} 红兄
   /*.........................→.........................→.........................*
    *...........[B]...........→...........[B]...........→...........[B]...........*
    *...........(p)...........→............s............→............s............*
    *.........../.\...........→.........../.\...........→.........../.\...........*
    *........../...\..........→........../...\..........→........../...\..........*
    *........./.....\.........→........./.....\.........→........./.....\.........*
    *.....[B]n.......s[R].....→.....[R]p.......e[B].....→.....[R]p.......e[B].....*
    *.............../.\.......→......./.\...............→.........\...............*
    *...........[B]E...e[B]...→...[B]n...E[B]...........→..........E[B]...........*
    *.........................→.........................→.........................*

    *.........................→.........................→.........................*
    *...........[B]...........→...........[B]...........→...........[B]...........*
    *...........(p)...........→............s............→............s............*
    *.........../.\...........→.........../.\...........→.........../.\...........*
    *........../...\..........→........../...\..........→........../...\..........*
    *........./.....\.........→........./.....\.........→........./.....\.........*
    *.....[R]s.......n[B].....→.....[B]e.......p[R].....→.....[B]e.......p[R].....*
    *......./.\...............→.............../.\.......→.............../.........*
    *...[B]e...E[B]...........→...........[B]E...n[B]...→...........[B]E..........*
    *.........................→.........................→.........................*/
#define RBTM_DetachCaseRedSibling(tree, parent, sibling, rotateFun) \
    RBTM_SetColor(parent,  RBT_RED);    \
    RBTM_SetColor(sibling, RBT_BLACK);  \
    rotateFun(tree, parent);

//! {Case:6} 无红侄，红父
   /*.........................→.........................→.........................*
    *...........[R]...........→...........[B]...........→...........[B]...........*
    *............p............→............p............→............p............*
    *.........../.\...........→.........../.\...........→.............\...........*
    *........../...\..........→........../...\..........→..............\..........*
    *........./.....\.........→........./.....\.........→...............\.........*
    *.....[B]n.......s[B].....→.....[B]n.......s[R].....→................s[R].....*
    *.............../.\.......→.............../.\.......→.............../.\.......*
    *...........[B]?...?[B]...→...........[B]?...?[B]...→...........[B]?...?[B]...*
    *.........................→.........................→.........................*/
//! {Case:7} 无红侄，黑父，传递调整
   /*.........................→.........................→.........................*
    *...........[B]...........→...........[B]...........→...........[B]...........*
    *............p............→............p............→............p............*
    *.........../.\...........→.........../.\...........→.............\...........*
    *........../...\..........→........../...\..........→..............\..........*
    *........./.....\.........→........./.....\.........→...............\.........*
    *.....[B]n.......s[B].....→.....[B]n.......s[R].....→................s[R].....*
    *.............../.\.......→.............../.\.......→.............../.\.......*
    *...........[B]?...?[B]...→...........[B]?...?[B]...→...........[B]?...?[B]...*
    *.........................→.........................→.........................*/
#define RBTM_DetachCaseDoubleBlackNephew(tree, parent, sibling) \
    if( RBTM_IsRed(parent) ) {                  \
        RBTM_SetColor(parent, RBT_BLACK);       \
        RBTM_SetColor(sibling, RBT_RED);        \
    } else {                                    \
        RBTM_SetColor(sibling, RBT_RED);        \
        rbtree_detachBlackLeaf(tree, parent);   \
    }

//! {Case:8, 9} 红侄
   /*.........................→.........................→.........................*
    *...........[?]...........→...........[?]...........→...........[?]...........*
    *...........(p)...........→............s............→............s............*
    *.........../.\...........→.........../.\...........→.........../.\...........*
    *........../...\..........→........../...\..........→........../...\..........*
    *........./.....\.........→........./.....\.........→........./.....\.........*
    *.....[B]n.......s[B].....→.....[B]p.......e[B].....→.....[B]p.......e[B].....*
    *.............../.\.......→......./.\...............→.........\...............*
    *...........[?]E...e[R]...→...[B]n...E[?]...........→..........E[?]...........*
    *.........................→.........................→.........................*

    *.........................→.........................→.........................*
    *...........[?]...........→...........[?]...........→...........[?]...........*
    *...........(p)...........→............s............→............s............*
    *.........../.\...........→.........../.\...........→.........../.\...........*
    *........../...\..........→........../...\..........→........../...\..........*
    *........./.....\.........→........./.....\.........→........./.....\.........*
    *.....[B]n.......s[B].....→.....[B]p.......e[B].....→.....[B]p.......e[B].....*
    *.............../.\.......→......./.\...............→.........\...............*
    *...........[?]E...e[R]...→...[B]n...E[?]...........→..........E[?]...........*
    *.........................→.........................→.........................*/
#define RBTM_DetachCaseRedNephew(tree, parent, sibling, whichchild, nephew_near, nephew_far, rotateFun1, rotateFun2) \
    RBTColor color_temp;                                            \
    if( RBTM_IsRed(nephew_near) ) {                                 \
        /* near ==> far */                                          \
        RBTM_SwapColorByTemp(sibling, nephew_near, color_temp);     \
        rotateFun1(tree, sibling);                                  \
        sibling = parent->whichchild;                               \
    }                                                               \
    /* far */                                                       \
    RBTM_SwapColorByTemp(parent, sibling, color_temp);              \
    RBTM_SetColor(nephew_far, RBT_BLACK);                           \
    rotateFun2(tree, parent);

static void inline rbtree_detachBlackLeafSub(RBTreeType tree, RBTNodeType node) {
    RBTNodeType sibling;
    if( RBTM_IsLeftChild(node) ) {
//■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■
//■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■
        sibling = RBTM_GetParent(node)->right;

        if( RBTM_IsRed(sibling) ) {
            #ifdef rbtree_detach_PRINTSTEP
                printf("Detach: Case:5L\n");
            #endif // rbtree_detach_PRINTSTEP
            RBTM_DetachCaseRedSibling(tree, RBTM_GetParent(node), sibling, rbtree_RR);
        } else {
            if( RBTM_IsBlackOrNull(sibling->left) && RBTM_IsBlackOrNull(sibling->right) ) {
                #ifdef rbtree_detach_PRINTSTEP
                    printf("Detach: Case:6,7L\n");
                #endif // rbtree_detach_PRINTSTEP
                RBTM_DetachCaseDoubleBlackNephew(tree, RBTM_GetParent(node), sibling);
            } else {
                #ifdef rbtree_detach_PRINTSTEP
                    printf("Detach: Case:8,9L\n");
                #endif // rbtree_detach_PRINTSTEP
                RBTM_DetachCaseRedNephew(tree, RBTM_GetParent(node), sibling, right, sibling->left, sibling->right, rbtree_LL, rbtree_RR);
            }
        }
        //parent->left = NULL;

//■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■
    } else {
//■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■
        sibling = RBTM_GetParent(node)->left;

        if( RBTM_IsRed(sibling) ) {
            #ifdef rbtree_detach_PRINTSTEP
                printf("Detach: Case:5R\n");
            #endif // rbtree_detach_PRINTSTEP
            RBTM_DetachCaseRedSibling(tree, RBTM_GetParent(node), sibling, rbtree_LL);
        } else {
            if( RBTM_IsBlackOrNull(sibling->left) && RBTM_IsBlackOrNull(sibling->right) ) {
                #ifdef rbtree_detach_PRINTSTEP
                    printf("Detach: Case:6,7R\n");
                #endif // rbtree_detach_PRINTSTEP
                RBTM_DetachCaseDoubleBlackNephew(tree, RBTM_GetParent(node), sibling);
            } else {
                #ifdef rbtree_detach_PRINTSTEP
                    printf("Detach: Case:8,9R\n");
                #endif // rbtree_detach_PRINTSTEP
                RBTM_DetachCaseRedNephew(tree, RBTM_GetParent(node), sibling, left, sibling->right, sibling->left, rbtree_RR, rbtree_LL);
            }
        }
        //parent->right = NULL;

//■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■
//■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■ ■■
    } //end if
}