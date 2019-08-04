
static void rbtree_print_tab(int level, FILE* output) {
    int i = 0;
    for(; i < level; i++) {
        fputs("...|", output);
    }
}

static void rbtree_print_node(RBTNodeType node, int level, int deepBlack, FILE* output) {
    if(!node) {
        rbtree_print_tab(level, output);
        puts(" NIL");
        return;
    }
    //Right
    rbtree_print_node(node->right, level + 1, deepBlack + (node->color==RBT_BLACK?1:0), output);

    //Current
    rbtree_print_tab(level, output);
    if(NULL==node->left && NULL==node->right) {
        if( RBT_BLACK == node->color ) {
            fprintf(output, " B(%05Id)=======================%d\n", ((node->key.i)), deepBlack);
        } else {
            fprintf(output, " R(%05Id)=======================%d\n", ((node->key.i)), deepBlack - 1);
        }
    } else {
        if( RBT_BLACK == node->color ) {
            fprintf(output, " B(%05Id)=%d\n", ((node->key.i)), deepBlack);
        } else {
            fprintf(output, " R(%05Id)\n", ((node->key.i)));
        }
    }

    //Left
    rbtree_print_node(node->left, level + 1, deepBlack + (node->color==RBT_BLACK?1:0), output);
}

static void rbtree_print(RBTreeType tree, FILE* output) {
    fprintf(output, "\n\n");
    rbtree_print_node(tree->root, 0, 0, output);
    fprintf(output, "\n\n");
}
