#include <stdlib.h> 
#include <stdio.h> 
#include <time.h>
#include <limits.h>

#ifndef __SKIPLIST_H 
#define __SKIPLIST_H 

#define SKIPLIST_MAXLEVEL 8    

typedef struct skiplistNode { 
    double score; 
    struct skiplistLevel { 
        struct skiplistNode *forward; 
    }level[]; 
}skiplistNode; 

typedef struct skiplist { 
    struct skiplistNode *header; 
    int level; 
}skiplist; 

#endif

skiplistNode *slCreateNode(int level, double score) { 
    skiplistNode * sn = malloc(sizeof(*sn) + level*sizeof(struct skiplistLevel)); 
    sn->score = score; 
    return sn; 
} 

skiplist *slCreate(void) { 
    int j; 
    skiplist *sl; 

    sl = malloc(sizeof(*sl)); 
    sl->level = 1; 
    sl->header = slCreateNode(SKIPLIST_MAXLEVEL, 0); 
    for(j = 0; j < SKIPLIST_MAXLEVEL; j++) { 
        sl->header->level[j].forward = NULL; 
    } 
    return sl; 
} 

void slFreeNode(skiplistNode *sn) { 
    free(sn); 
} 

void slFree(skiplist *sl) { 
    skiplistNode *node = sl->header->level[0].forward, *next; 

    free(sl->header); 
    while(node) { 
        next = node->level[0].forward; 
        slFreeNode(node); 
        node = next; 
    } 
    free(sl); 
} 

int slRandomLevel(void) { 
    int level = 1; 
    while(rand()%2)
        level += 1; 
    return (level < SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL; 
} 

int slSearch(skiplist *sl, double score) { 
    skiplistNode *node; 
    int i; 

    node = sl->header; 
    for (i = sl->level-1; i >= 0 ;i--) { 
        while(node->level[i].forward && node->level[i].forward->score < score) { 
            node = node->level[i].forward; 
        } 
    } 
    node = node->level[0].forward; 
    if (node && score == node->score) { 
        printf("Found %d\n",(int)node->score); 
        return 1; 
    } else { 
        printf("Not found %d\n", (int)score); 
        return 0; 
    } 
} 

skiplistNode *slInsert(skiplist *sl, double score) { 
    //if(slSearch(sl,score))
    //{
    //    printf("duplicate score\n");
    //    return NULL;
    //}
    skiplistNode *update[SKIPLIST_MAXLEVEL]; 
    skiplistNode *node; 

    node = sl->header; 
    int i, level; 
    for ( i = sl->level-1; i >= 0; i--) { 
        while(node->level[i].forward && node->level[i].forward->score < score) { 
            node = node->level[i].forward; 
        } 
        if (node->level[i].forward && node->level[i].forward->score == score) {
            printf("duplicate\n");
            return NULL;
        }
        update[i] = node; 
    } 
    level = slRandomLevel(); 
    if (level > sl->level) { 
        for (i = sl->level; i< level ;i++) { 
            update[i] = sl->header; 
        } 
        sl->level = level; 
    } 
    node = slCreateNode(level, score); 
    for (i = 0; i < level; i++) { 
        node->level[i].forward = update[i]->level[i].forward; 
        update[i]->level[i].forward = node; 
    } 

    return node; 
} 

void slDeleteNode(skiplist *sl, skiplistNode *x, skiplistNode **update){ 
    int i; 
    for (i = 0; i < sl->level; i++) { 
        if (update[i]->level[i].forward == x) { 
            update[i]->level[i].forward = x->level[i].forward; 
        } 
    } 
    while (sl->level > 1 && sl->header->level[sl->level-1].forward == NULL)    
        sl->level--; 
} 

int slDelete(skiplist *sl, double score) { 
    skiplistNode *update[SKIPLIST_MAXLEVEL], *node; 
    int i; 

    node = sl->header; 
    for(i = sl->level-1; i >= 0; i--) { 
        while (node->level[i].forward && node->level[i].forward->score < score) { 
            node = node->level[i].forward; 
        } 
        update[i] = node; 
    } 
    node = node->level[0].forward; 
    if (node && score == node->score) { 
        slDeleteNode(sl, node, update); 
        slFreeNode(node); 
        return 1; 
    } else { 
        return 0; 
    } 
    return 0; 
} 

void slPrint(skiplist *sl) { 
    skiplistNode *node; 
    int i; 
    for (i = 0; i < SKIPLIST_MAXLEVEL; i++) { 
        printf("LEVEL[%d]: ", i); 
        node = sl->header->level[i].forward; 
        while(node) { 
            printf("%d -> ", (int)(node->score)); 
            node = node->level[i].forward; 
        } 
        printf("NULL\n"); 
    } 
} 

