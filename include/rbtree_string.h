#ifndef ZHMH_RBTREE_H_INCLUDED
#define ZHMH_RBTREE_H_INCLUDED
#include <stdio.h>
#include <stdint.h>

typedef struct {
    union {
        void* p;
        char* s;
        unsigned char* b;
    } data;
    size_t length;
    int64_t hash;
} string_zhmh;

typedef void rbtree_datafree_callback(void* data);

typedef struct {
    string_zhmh* (*getStringByBuf)(const void* data, size_t length);
    string_zhmh* (*getStringByChs)(const char* chs);
    string_zhmh* (*getStringByStr)(string_zhmh* str);
    void (*freeString)(string_zhmh* str);

    void* (*getStorage)(void);
    void  (*freeStorage)(void*, rbtree_datafree_callback* callback);

    int   (*setValue)(void*, string_zhmh* str, void* data);
    void* (*getValue)(void*, string_zhmh* str);
    int   (*delValue)(void*, string_zhmh* str, void** pdata);

    int   (*set)(void*, const char* chs, void* data);
    void* (*get)(void*, const char* chs);
    int   (*del)(void*, const char* chs, void** pdata);

} rbtree_string_interface_zhmh;

extern rbtree_string_interface_zhmh* rbtree_string_zhmh;

#endif // ZHMH_RBTREE_H_INCLUDED
