#include "rbtree_string.h"
#include "rbtree.h"
#include <string.h>

//#define rbtree_string_PRINTSTEP

typedef struct string_list_zhmh string_list_zhmh;
struct string_list_zhmh {
    string_zhmh*        skey;
    void*               data;
    string_list_zhmh*   next;
};

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

#ifdef rbtree_string_PRINTSTEP
void string_zhmh_printfirst(RBTNodeType node) {
    string_list_zhmh* list = (string_list_zhmh*)(node->data.first);
    printf("printfirst: %Id\n", node->data.length);
    intptr_t temp;
    while(list) {
        temp = (intptr_t)(list->data);
        printf("    list: %Id\n", temp);
        list = list->next;
    }
}
#endif // rbtree_string_PRINTSTEP

static void string_zhmh_hash(string_zhmh* s) {
    static const int seed = 31;
    RBTKeyType hash = 0;
    if (s->length > 0) {
        const unsigned char* e = s->data.b + s->length;
        const unsigned char* v = s->data.b;
        while(v < e) {
            hash = seed * hash + *v;
            v++;
        }
        s->hash = hash;
        //s->hash = 0;
    }
    #ifdef rbtree_string_PRINTSTEP
        printf("hash=%Id\n", hash);
    #endif // rbtree_string_PRINTSTEP
}

static int string_zhmh_compare(string_zhmh* s1, string_zhmh* s2) {
    if( s1->data.p == s2->data.p ) return 0;
    if( s1->length != s2->length ) return -100;
    if( s1->hash   != s2->hash   ) return -200;
    return memcmp(s1->data.p, s2->data.p, s1->length);
}

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

static string_zhmh* rbts_getstring(const void* dat, size_t len) {
    string_zhmh* string = (string_zhmh*)malloc(sizeof(string_zhmh));
    if(NULL==string) return NULL;

    char* data = (char*)malloc(len + 1);
    if(NULL==data) {
        free(string);
        return NULL;
    }

    memcpy(data, dat, len);
    data[len] = 0;

    string->data.s = data;
    string->length = len;
    string_zhmh_hash(string);

    return string;
}

static string_zhmh* rbts_getstring2(const char* dat) {
    return rbts_getstring(dat, strlen(dat));
}

static string_zhmh* rbts_getstring3(string_zhmh* str) {
    string_zhmh* string = (string_zhmh*)malloc(sizeof(string_zhmh));
    if(NULL!=string) {
        memcpy(string, str, sizeof(string_zhmh));
        string->data.p = malloc(str->length + 1);
        if(NULL==string->data.p) {
            free(string);
            return NULL;
        }
        memcpy(string->data.p, str->data.p, str->length + 1);//include final 0
    }
    return string;
}

static void rbts_freestring(string_zhmh* str) {
    if(str) {
        if(str->data.p) {
            free(str->data.p);
        }
        free(str);
    }
}

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

static void* rbts_getstorage(void) {
    RBTreeType storage = (RBTreeType)malloc(sizeof(RBTreeSize));
    if(NULL==storage) return NULL;

    rbtree_zhmh->init(storage);
    return storage;
}

static void rbts_freestorage_sub(RBTNodeType node, rbtree_datafree_callback* callback) {
    if(!node) return;
    rbts_freestorage_sub(node->left , callback);
    rbts_freestorage_sub(node->right, callback);
    //free current
    string_list_zhmh* list = (string_list_zhmh*)node->data.first;
    string_list_zhmh* temp;
    while(list) {
        rbts_freestring(list->skey);
        if(NULL!=callback)  {
            callback(list->data);
        }
        temp = list;
        list = list->next;
        free(temp);
    }
    free(node);
}

static void rbts_freestorage(void* storage_ptr, rbtree_datafree_callback* callback) {
    RBTreeType storage = (RBTreeType)storage_ptr;
    #ifdef rbtree_string_PRINTSTEP
        rbtree_zhmh->print(storage, stdout);
    #endif // rbtree_string_PRINTSTEP
    rbts_freestorage_sub(storage->root, callback);
    free(storage);
}

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

//! @return 0 正确
static int rbts_setvalue(void* storage_ptr, string_zhmh* str, void* data) {
    RBTreeType storage = (RBTreeType)storage_ptr;
    RBTNodeType node = rbtree_zhmh->insertKey(storage, str->hash);
    if(0 == node->data.length) {
        //新插入list
        #ifdef rbtree_string_PRINTSTEP
            printf("setvalue: new list!\n");
        #endif // rbtree_string_PRINTSTEP
        string_list_zhmh* list = (string_list_zhmh*)malloc(sizeof(string_list_zhmh));
        if(NULL==list) return -100;
        list->skey = rbts_getstring3(str);//str;
        list->data = data;
        list->next = NULL;
        node->data.first = list;
        node->data.length = 1;
    } else {
        string_list_zhmh* list = (string_list_zhmh*)(node->data.first);
        for(;;) {
            if(0==string_zhmh_compare(list->skey, str)) {
                //已存在
                #ifdef rbtree_string_PRINTSTEP
                    printf("setvalue: exist!\n");
                #endif // rbtree_string_PRINTSTEP
                list->data = data;
                break;
            } else if(NULL==list->next) {
                //新插入
                #ifdef rbtree_string_PRINTSTEP
                    printf("setvalue: new value!\n");
                #endif // rbtree_string_PRINTSTEP
                string_list_zhmh* temp = (string_list_zhmh*)malloc(sizeof(string_list_zhmh));
                if(NULL==temp) return -200;
                temp->skey = rbts_getstring3(str); //str;
                temp->data = data;
                temp->next = NULL;
                node->data.length = node->data.length + 1;
                list->next = temp;
                break;
            }
            list = list->next;
        }
    }
    return 0;
}

static void* rbts_getvalue(void* storage_ptr, string_zhmh* str) {
    RBTreeType storage = (RBTreeType)storage_ptr;
    RBTNodeType node = rbtree_zhmh->lookupKey(storage, str->hash, NULL);
    if(NULL==node || 0==node->data.length) return NULL;
    string_list_zhmh* list = (string_list_zhmh*)(node->data.first);
    while(list) {
        if(0==string_zhmh_compare(list->skey, str)) {
            return list->data;
        }
        list = list->next;
    }
    return NULL;
}

static int rbts_delvalue(void* storage_ptr, string_zhmh* str, void** pdata) {
    RBTreeType storage = (RBTreeType)storage_ptr;
    RBTNodeType node = rbtree_zhmh->lookupKey(storage, str->hash, NULL);
    if(NULL==node) return 0;

    string_list_zhmh* current = (string_list_zhmh*)(node->data.first);
    if(0==string_zhmh_compare(current->skey, str)) {
        #ifdef rbtree_string_PRINTSTEP
            printf("delvalue: first!\n");
        #endif // rbtree_string_PRINTSTEP
        //修正
        node->data.first = current->next;
        node->data.length = node->data.length - 1;
        //释放
        if(NULL!=pdata) *pdata = current->data;
        rbts_freestring(current->skey);
        free(current);
    } else {
        string_list_zhmh* prev;
        for(;;) {
            prev = current;
            current = current->next;
            if(NULL==current) break;
            if(0==string_zhmh_compare(current->skey, str)) {
                #ifdef rbtree_string_PRINTSTEP
                    printf("delvalue: body!\n");
                #endif // rbtree_string_PRINTSTEP
                //修正
                prev->next = current->next;
                node->data.length = node->data.length - 1;
                //释放
                if(NULL!=pdata) *pdata = current->data;
                rbts_freestring(current->skey);
                free(current);
            }
        }
    }

    if(0==node->data.length) {
        #ifdef rbtree_string_PRINTSTEP
            printf("delvalue: detach!\n");
        #endif // rbtree_string_PRINTSTEP
        rbtree_zhmh->detachNode(storage, node);
        free(node);
    }
    return 0;
}

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

static int rbts_set(void* storage_ptr, const char* chs, void* data) {
    string_zhmh* str = rbts_getstring2(chs);
    int ret = rbts_setvalue(storage_ptr, str, data);
    rbts_freestring(str);
    return ret;
}
static void* rbts_get(void* storage_ptr, const char* chs) {
    string_zhmh* str = rbts_getstring2(chs);
    void* ret = rbts_getvalue(storage_ptr, str);
    rbts_freestring(str);
    return ret;
}
static int rbts_del(void* storage_ptr, const char* chs, void** pdata) {
    string_zhmh* str = rbts_getstring2(chs);
    int ret = rbts_delvalue(storage_ptr, str, pdata);
    rbts_freestring(str);
    return ret;
}

//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■
//■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■ ■■■■■■■■

static rbtree_string_interface_zhmh rbtree_string_methods = {
    rbts_getstring,
    rbts_getstring2,
    rbts_getstring3,
    rbts_freestring,

    rbts_getstorage,
    rbts_freestorage,

    rbts_setvalue,
    rbts_getvalue,
    rbts_delvalue,

    rbts_set,
    rbts_get,
    rbts_del
};
rbtree_string_interface_zhmh* rbtree_string_zhmh = &rbtree_string_methods;
