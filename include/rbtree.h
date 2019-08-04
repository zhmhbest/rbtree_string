
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

#ifndef ZHMH_PRIVATE_RBTREE_H_INCLUDED
#define ZHMH_PRIVATE_RBTREE_H_INCLUDED

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

/**
 * @name RBTColor
 */
typedef enum {
    RBT_BLACK,
    RBT_RED
} RBTColor;

/**
 * @name RBTKey
 */
typedef struct {
    union {
        void*       p;
        int64_t     i;
        uint64_t    u;
    };
} RBTKey;

/**
 * @name RBTData
 */
typedef struct {
    void*   first; //指向第一个链表
    size_t  length; //链表长度
} RBTData;

/**
 * @name RBTNode
 */
//typedef struct RBTNode RBTNode;
struct RBTNode {
    RBTColor    color;
    RBTKey      key;
    RBTData     data;
    struct RBTNode *parent, *left, *right;
};

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

#define RBTKeyType  int64_t
#define RBTreeType  struct RBTree*
#define RBTreeSize  sizeof(struct RBTree)
#define RBTNodeType struct RBTNode*
#define RBTNodeSize sizeof(struct RBTNode)

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

/*!
 * @name RBTFoKC = Function of key compare
 */
typedef int RBTFoKC (RBTKeyType, RBTKeyType);

/*!
 * @name RBTree
 */
//typedef struct RBTree RBTree;
struct RBTree {
    struct RBTNode* root;
    RBTFoKC*        fokc;
};

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

typedef struct {
    void (*init)(RBTreeType tree);

    RBTNodeType (*min)(RBTreeType tree);
    RBTNodeType (*max)(RBTreeType tree);
    RBTNodeType (*prev)(RBTNodeType node);
    RBTNodeType (*next)(RBTNodeType node);

    void (*LL)(RBTreeType tree, RBTNodeType node);
    void (*RR)(RBTreeType tree, RBTNodeType node);

    RBTNodeType (*lookupKey) (RBTreeType tree, RBTKeyType key, RBTNodeType* pparent);
    RBTNodeType (*lookupNode)(RBTreeType tree, RBTNodeType node, RBTNodeType* pparent);

    RBTNodeType (*insertKey) (RBTreeType tree, RBTKeyType key);
    RBTNodeType (*insertNode)(RBTreeType tree, RBTNodeType node);

    RBTNodeType (*detachKey) (RBTreeType tree, RBTKeyType key);
    RBTNodeType (*detachNode)(RBTreeType tree, RBTNodeType node);

    void (*print)(RBTreeType tree, FILE* output);
} rbtree_interface_zhmh;

extern rbtree_interface_zhmh* rbtree_zhmh;

#endif // ZHMH_PRIVATE_RBTREE_H_INCLUDED

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■












/*.........................................→.........................................*
 *....................A....................→....................A....................*
 *.................../.\...................→.................../.\...................*
 *................../...\..................→................../...\..................*
 *................./.....\.................→................./.....\.................*
 *................/.......\................→................/.......\................*
 *.............../.........\...............→.............../.........\...............*
 *............../...........\..............→............../...........\..............*
 *............./.............\.............→............./.............\.............*
 *............B...............C............→............B...............C............*
 *.........../.\............./.\...........→.........../.\............./.\...........*
 *........../...\.........../...\..........→........../...\.........../...\..........*
 *........./.....\........./.....\.........→........./.....\........./.....\.........*
 *........D.......E.......F.......G........→........D.......E.......F.......G........*
 *......./.\...../.\...../.\...../.\.......→......./.\...../.\...../.\...../.\.......*
 *......H...I...J...K...L...M...N...O......→......H...I...J...K...L...M...N...O......*
 *.........................................→.........................................*/

/*.........................→.........................*
 *..........f...f..........→..........f...f..........*
 *...........\./...........→...........\./...........*
 *............?............→............?............*
 *.........../.\...........→.........../.\...........*
 *........../...\..........→........../...\..........*
 *........./.....\.........→........./.....\.........*
 *........?.......?........→........?.......?........*
 *......./.\...../.\.......→......./.\...../.\.......*
 *......?...?...?...?......→......?...?...?...?......*
 *.........................→.........................*/
