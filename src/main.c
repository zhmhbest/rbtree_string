#include "rbtree_string.h"

void freecallback(void* data) {
    printf("%Id\n", ((intptr_t)(data)) );
}

int main() {
    void* storage = rbtree_string_zhmh->getStorage();
    rbtree_string_zhmh->set(storage, "apple", (void*)4396);
    rbtree_string_zhmh->set(storage, "huawei", (void*)957);
    rbtree_string_zhmh->set(storage, "oppo", (void*)889);
    rbtree_string_zhmh->set(storage, "vivo", (void*)998);
    rbtree_string_zhmh->set(storage, "xiaomi", (void*)233);

    printf("apple=%Id\n", (intptr_t)(rbtree_string_zhmh->get(storage, "apple")) );
    printf("huawei=%Id\n", (intptr_t)(rbtree_string_zhmh->get(storage, "huawei")) );
    printf("oppo=%Id\n", (intptr_t)(rbtree_string_zhmh->get(storage, "oppo")) );
    printf("vivo=%Id\n", (intptr_t)(rbtree_string_zhmh->get(storage, "vivo")) );
    printf("xiaomi=%Id\n", (intptr_t)(rbtree_string_zhmh->get(storage, "xiaomi")) );

    rbtree_string_zhmh->set(storage, "apple", (void*)9999);
    printf("apple=%Id\n", (intptr_t)(rbtree_string_zhmh->get(storage, "apple")) );

    rbtree_string_zhmh->freeStorage(storage, freecallback);
}
