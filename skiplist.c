#include "skiplist.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

struct SkipList *skipList_new() {
    struct SkipList *list = (struct SkipList *) malloc(sizeof (struct SkipList));
    list->height = 1;
    list->count = 0;
    list->top = skipListSpecialNode_new(NODE_MIN);
    list->head = list->top;
    list->dataFile = SKIPLIST_DATA_FILE;
    return list;
}

struct SkipListNode *skipListNode_new(unsigned char *key, void *data) {
    struct SkipListNode *node = (struct SkipListNode *) malloc(sizeof (struct SkipListNode));
    node->key = key;
    node->data = data;
    node->down = NULL;
    node->next = NULL;
    node->type = NODE_NORMAL;
    return node;
}

struct SkipListNode *skipListSpecialNode_new(enum SkipListNodeType type) {
    struct SkipListNode *node = (struct SkipListNode *) malloc(sizeof (struct SkipListNode));
    node->key = NULL;
    node->data = NULL;
    node->down = NULL;
    node->next = NULL;
    node->type = type;
    return node;
}

void skipListNode_insert(struct SkipList *list, unsigned char *key, void *data) {
    struct SkipListNode *tmp = NULL;
    struct SkipListNode *newNode = skipListNode_new(key, data);
    int height = list->height;
    int level = SkipList_level(list);
    struct SkipListNode **everyLevel = NULL;
    everyLevel = (struct SkipListNode **) malloc(sizeof (struct SkipListNode *) * (height + 1));
    if (list->head->next == NULL) {
        list->head->next = skipListNode_new(key, data);
        list->count++;
    } else {
        tmp = list->top;

        while (height >= 1) {
//                printf("%s == %s == %d\n", key,tmp->next->key,strcmp(key, tmp->next->key));
            while (    tmp->next != NULL && ( strcmp(key, tmp->next->key) > 0)) {
                tmp = tmp->next;
            }
            *(everyLevel + height - 1) = tmp;
            if(tmp->down!=NULL){
                tmp = tmp->down;
            }
            --height;
        }
        if (tmp->down != NULL) {
            tmp = tmp->down;
        }
        newNode->next = tmp->next;
        tmp->next = newNode;
        list->count++;
        int now = level;
        if (level == list->height) {
            now = level - 1;
        }
        for (int i = 1; i <= now; ++i) {
            newNode = skipListNode_new(key, data);
            newNode->down = tmp->next;
            tmp = *(everyLevel + i);
            newNode->next = tmp->next;
            tmp->next = newNode;
            list->count++;
        }
        if (level == list->height) {
            newNode = skipListNode_new(key, data);
            newNode->down = tmp->next;
            tmp = skipListSpecialNode_new(NODE_MIN);
            tmp->down = list->top;
            tmp->next = newNode;
            list->top = tmp;
            list->count++;
            list->height++;
        }
    }
    free(everyLevel);
}

void *skipListNode_find(struct SkipList *list, unsigned char *key) {
    struct SkipListNode *tmp = NULL;
    int height = list->height;
    int r = 1;
    if (list->head->next == NULL) {
        return NULL;
    } else {
        tmp = list->top;
        while (height > 1) {
            while (tmp->next != NULL) {
                if ((strcmp(key, tmp->next->key)< 0) ||
                        ( tmp->key!= NULL && strcmp(key, tmp->key) == 0)) {
                    break;
                }
                tmp = tmp->next;
            }
            tmp = tmp->down;
            --height;
        }
        if (tmp->down != NULL) {
            tmp = tmp->down;
        }
        while ( 
                ( tmp->key==NULL || (r = strcmp(key, tmp->key)) > 0) &&
                tmp->next != NULL
                ) {
            tmp = tmp->next;
        }
        if (r == 0) {
            return tmp->data;
        }
    }
    return NULL;
}

int SkipList_level(struct SkipList *list) {
//    return 0;
    unsigned int ran = 0;// rand();
    int height = list->height;
    int level = 0;
//    while(level <= height){
//        if(ran % (unsigned int)pow(2,level)!=0){
//            break;
//        }
//        ++level;
//    }
struct timeval t_val;

    while(1){
    	gettimeofday(&t_val, NULL);
        srand((unsigned)t_val.tv_usec);
        ran = rand();
//        printf("rand:%d\n",ran);
        if(ran % 3 > 1){
            ++level;
        }else{
            break;
        }
    }
    if(level > height){
        level = height;
    }
    return level;
}

void skipList_free(struct SkipList *list) {
    struct SkipListNode *tmp = list->top;
    struct SkipListNode *tmp1 = NULL;
    struct SkipListNode *tmp2 = NULL;
    int height = list->height;
    free(list);
    while (tmp != NULL) {
        tmp1 = tmp->down;
        while (tmp != NULL) {
            tmp2 = tmp->next;
            SkipListNode_free(tmp);
            tmp = tmp2;
        }
        tmp = tmp1;
    }
}

void SkipListNode_free(struct SkipListNode *node) {
    free(node);
}

void SkipList_print(struct SkipList *list){
    struct SkipListNode *tmp = list->top;
    struct SkipListNode *tmp1 = NULL;
    struct SkipListNode *tmp2 = NULL;
    int height = list->height;
    printf("HEIGHT:%d\n",height);
    printf("COUNT:%d\n",list->count);
//    while (tmp != NULL) {
//        tmp1 = tmp->down;
//        printf("LEVEL%d:",height-1);
//        while (tmp != NULL) {
//            tmp2 = tmp->next;
//            if(tmp->type == NODE_MIN){
//                printf("MIN\t");
//            }else{
//                printf("%s\t",tmp->key);
//            }
//            tmp = tmp2;
//        }
//        printf("\n");
//        tmp = tmp1;
//        --height;
//    }
}
