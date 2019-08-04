/*
1. node->color == Red Or Black。
2. root->color == Black。
3. leaf->color == Black。
4.
	1. RedNode的Child必定为Black。
	2. BlackNode的Child既可以是Red也可以是Black。
5. 从任意每个 node 到其每个 leaf 的所有 SimplePath 上 BlackNode 的数量是相同的。
*/

#if defined(__clang__) || (defined(__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
    #define GCC_DIAGNOSTIC_ENABLE
#endif
#ifdef GCC_DIAGNOSTIC_ENABLE
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-function"
#endif // GCC_DIAGNOSTIC_ENABLE

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

#include "rbtree.h"

#define RBTM_NewTree (RBTreeType)malloc(RBTreeSize)
#define RBTM_NewNode (RBTNodeType)malloc(RBTNodeSize)

#define RBTM_SetRoot(_tree, _node)      ( _tree->root = _node )

#define RBTM_GetColor(_node)            ( _node->color )
#define RBTM_SetColor(_node, _color)    ( _node->color = _color )

#define RBTM_GetParent(_node)           ( _node->parent )
#define RBTM_SetParent(_node, _parent)  ( _node->parent = _parent )

#define RBTM_IsRoot(_node)              ( _node && NULL      == RBTM_GetParent(_node) )
#define RBTM_IsBlack(_node)             ( _node && RBT_BLACK == RBTM_GetColor(_node) )
#define RBTM_IsRed(_node)               ( _node && RBT_RED   == RBTM_GetColor(_node) )
#define RBTM_IsBlackOrNull(_node)       ( !_node || RBT_BLACK == RBTM_GetColor(_node) )

#define RBTM_ExistParent(_node)         ( _node && RBTM_GetParent(_node) )
    #define RBTM_IsLeftChild(_node)         ( _node == RBTM_GetParent(_node)->left )
    #define RBTM_IsRightChild(_node)        ( _node == RBTM_GetParent(_node)->right )

//#define RBTM_BindRoot(_tree, _node)     _tree->root = _node;   if(_node)   _node->parent = NULL
//#define RBTM_BindLeft(_node, _Lnode)    _node->left  = _Lnode; if(_Lnode) _Lnode->parent = _node
//#define RBTM_BindRight(_node, _Rnode)   _node->right = _Rnode; if(_Rnode) _Rnode->parent = _node

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

static RBTNodeType rbtree_min(RBTreeType tree);
static RBTNodeType rbtree_max(RBTreeType tree);
static RBTNodeType rbtree_prev(RBTNodeType node);
static RBTNodeType rbtree_next(RBTNodeType node);

static void rbtree_rotateLeft(RBTreeType tree, RBTNodeType node);
static void rbtree_rotateRight(RBTreeType tree, RBTNodeType node);

static RBTNodeType rbtree_lookupKey(RBTreeType tree, RBTKeyType key, RBTNodeType* pparent);
static RBTNodeType rbtree_lookupNode(RBTreeType tree, RBTNodeType node, RBTNodeType* pparent);

static RBTNodeType rbtree_insertKey(RBTreeType tree, RBTKeyType key);
static RBTNodeType rbtree_insertNode(RBTreeType tree, RBTNodeType wins);

static RBTNodeType rbtree_detachKey(RBTreeType tree, RBTKeyType key);
static RBTNodeType rbtree_detachNode(RBTreeType tree, RBTNodeType node);

static void        rbtree_print(RBTreeType tree, FILE* output);

#include "rbtree_prev_next.inl.h"
#include "rbtree_rotate.inl.h"
#include "rbtree_lookup.inl.h"
#include "rbtree_insert.inl.h"
#include "rbtree_detach.inl.h"
#include "rbtree_print.inl.h"
#include "rbtree_final.h"

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

#ifdef GCC_DIAGNOSTIC_ENABLE
    #pragma GCC diagnostic pop
#endif // GCC_DIAGNOSTIC_ENABLE
