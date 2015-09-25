#include "skiplist.h"

struct SkipList *skipList_new() {
    struct SkipList *list = (struct SkipList *) malloc(sizeof (struct SkipList));
    list->height = 1;
    list->count = 0;
    list->top = skipListSpecialNode_new(NODE_MIN);
    list->head = list->top;
    list->dataFile = fopen(SKIPLIST_DATA_FILE, "ab+");
    list->StoreNode = fopen(SKIPLIST_NODES_FILE, "ab+");
    fseek(list->dataFile, 0, SEEK_END);
    fseek(list->StoreNode, 0, SEEK_SET);
    return list;
}

struct SkipListNode *skipListNode_new(struct SkipList *list, unsigned char *key, void *data) {
    struct SkipListNode *node = (struct SkipListNode *) malloc(sizeof (struct SkipListNode));
    node->key = key;
    int len = 0;
    node->data = -1;
    if (data != NULL) {
        len = strlen(data);
        fseek(list->dataFile, 0, SEEK_END);
        node->data = ftell(list->dataFile);
        fwrite(&len, sizeof (int), 1, list->dataFile);
        fwrite(data, sizeof (char), strlen(data), list->dataFile);
    }
    node->down = NULL;
    node->next = NULL;
    node->deleted = 0;
    node->type = NODE_NORMAL;
    return node;
}

struct SkipListNode *skipListSpecialNode_new(enum SkipListNodeType type) {
    struct SkipListNode *node = (struct SkipListNode *) malloc(sizeof (struct SkipListNode));
    node->key = NULL;
    node->data = 0;
    node->down = NULL;
    node->next = NULL;
    node->type = type;
    node->deleted = 0;
    return node;
}

void skipListNode_insert(struct SkipList *list, unsigned char *key, void *data) {
    struct SkipListNode *tmp = NULL;
    struct SkipListNode *newNode = skipListNode_new(list, key, data);
    int height = list->height;
    int level = SkipList_level(list);
    struct SkipListNode **everyLevel = NULL;
    everyLevel = (struct SkipListNode **) malloc(sizeof (struct SkipListNode *) * (height + 1));
    if (list->head->next == NULL) {
        list->head->next = newNode;
        list->count++;
    } else {
        tmp = list->top;

        while (height >= 1) {
            //                printf("%s == %s == %d\n", key,tmp->next->key,strcmp(key, tmp->next->key));
            while (tmp->next != NULL && (strcmp(key, tmp->next->key) > 0)) {
                tmp = tmp->next;
            }
            *(everyLevel + height - 1) = tmp;
            if (tmp->down != NULL) {
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
            newNode = skipListNode_new(list, key, NULL);
            newNode->down = tmp->next;
            tmp = *(everyLevel + i);
            newNode->next = tmp->next;
            tmp->next = newNode;
            list->count++;
        }
        if (level == list->height) {
            newNode = skipListNode_new(list, key, NULL);
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

//char *readNodeData(struct SkipList *list, struct SkipListNode *node) {
//    char *data = NULL;
//    int len = 0;
//    fseek(list->dataFile, node->data, SEEK_SET);
//    fread(&len, sizeof (int), 1, list->dataFile);
//    data = (char *) malloc(sizeof (char) * (len + 1));
//    memset(data, 0, sizeof (char) * (len + 1));
//    fread(data, sizeof (char), len, list->dataFile);
//    return data;
//}

void *skipListNode_find(struct SkipList *list, unsigned char *key) {
    SkipListNode *tmp = skipListNode_findNode(list, key, 0);
    if (tmp == NULL) {
        return tmp;
    }
    return readNodeData(list->dataFile, tmp->data);
}

SkipListNode *skipListNode_findNode(struct SkipList *list, unsigned char *key, short isRange) {
    struct SkipListNode *tmp = NULL;
    int height = list->height;
    int r = 1;
    if (list->head->next == NULL) {
        return NULL;
    } else {
        tmp = list->top;
        while (height > 1) {
            while (tmp->next != NULL) {
                if ((strcmp(key, tmp->next->key) < 0) ||
                        (tmp->key != NULL && strcmp(key, tmp->key) == 0)) {
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
                (tmp->key == NULL || (r = strcmp(key, tmp->key)) > 0) &&
                tmp->next != NULL
                ) {
            tmp = tmp->next;
        }
        if (r == 0) {
            return tmp;
        }
        if (isRange == 1 && r < 0) {
            return tmp;
        }
    }
    return NULL;
}

int SkipList_level(struct SkipList *list) {
    //    return 0;
    unsigned int ran = 0; // rand();
    int height = list->height;
    int level = 0;
    struct timeval t_val;
    //    while(level <= height){
    //        if(ran % (unsigned int)pow(2,level)!=0){
    //            break;
    //        }
    //        ++level;
    //    }

    while (1) {
        gettimeofday(&t_val, NULL);
        srand((unsigned) t_val.tv_usec);
        ran = rand();
        //        printf("rand:%d\n",ran);
        if (ran % 3 > 1) {
            ++level;
        } else {
            break;
        }
    }
    if (level > height) {
        level = height;
    }
    return level;
}

void skipList_free(struct SkipList *list) {
    struct SkipListNode *tmp = list->top;
    struct SkipListNode *tmp1 = NULL;
    struct SkipListNode *tmp2 = NULL;
    int height = list->height;
    fclose(list->dataFile);
    fclose(list->StoreNode);
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

void SkipList_print(struct SkipList *list) {
    struct SkipListNode *tmp = list->top;
    struct SkipListNode *tmp1 = NULL;
    struct SkipListNode *tmp2 = NULL;
    int height = list->height;
    printf("HEIGHT:%d\n", height);
    printf("COUNT:%d\n", list->count);
    printf("TOP:%p\n", list->top);
    printf("HEAD:%p\n", list->head);
    while (tmp != NULL) {
        tmp1 = tmp->down;
        printf("LEVEL%d[%p]:", height - 1, tmp);
        while (tmp != NULL) {
            tmp2 = tmp->next;
            if (tmp->type == NODE_MIN) {
                printf("MIN\t");
            } else {
                printf("%s\t", tmp->key);
            }
            tmp = tmp2;
        }
        printf("\n");
        tmp = tmp1;
        --height;
    }
}

long skipListStoreNode_new(struct SkipList *list, unsigned char *key, long data) {
    struct SkipListNodeStore *node = (struct SkipListNodeStore *) malloc(sizeof (struct SkipListNodeStore));
    node->len = strlen(key);
    int len = 0;
    node->data = data;
    node->deleted = 0;
    long pos = writeNodeToFile(list->StoreNode, node, key);
    free(node);
    return pos;
}

long writeNodeToFile(FILE *fp, struct SkipListNodeStore *node, char *key) {
    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp);
    fwrite(node, sizeof (struct SkipListNodeStore), 1, fp);
    fwrite(key, sizeof (char), node->len, fp);
    return pos;
}

struct SkipListNodeStore *readNodeFromFile(FILE *fp, long *pos, char **key) {
    struct SkipListNodeStore *node = (struct SkipListNodeStore *) malloc(sizeof (struct SkipListNodeStore));
    fseek(fp, *pos, SEEK_SET);
    int ret = 0;
    ret = fread(node, sizeof (struct SkipListNodeStore), 1, fp);
    if (ret == 0) {
        free(node);
        return NULL;
    }
    *key = (char *) malloc(sizeof (char) * (node->len + 1));
    memset(*key, 0, sizeof (char) * (node->len + 1));
    ret = fread(*key, sizeof (char), node->len, fp);
    if (ret < node->len) {
        free(node);
        free(*key);
        return NULL;
    }
    *pos = ftell(fp);
    return node;
}

char *readNodeData(FILE *fp, long pos) {
    char *data = NULL;
    int len = 0;
    fseek(fp, pos, SEEK_SET);
    fread(&len, sizeof (int), 1, fp);
    data = (char *) malloc(sizeof (char) * (len + 1));
    memset(data, 0, sizeof (char) * (len + 1));
    fread(data, sizeof (char), len, fp);
    return data;
}

void saveNodesToFile(struct SkipList *list) {
    struct SkipListNode *tmp = list->head;
    while (tmp != NULL) {
        if (tmp->type == NODE_NORMAL && tmp->deleted == 0) {
            printf("stored => %s\n", tmp->key);
            skipListStoreNode_new(list, tmp->key, tmp->data);
        }
        tmp = tmp->next;
    }
}

struct SkipListNode *skipListNodeReverse_new(struct SkipList *list, unsigned char *key, long data) {
    struct SkipListNode *node = (struct SkipListNode *) malloc(sizeof (struct SkipListNode));
    node->key = key;
    node->data = data;
    node->down = NULL;
    node->next = NULL;
    node->deleted = 0;
    node->type = NODE_NORMAL;
    return node;
}

void skipListNodeReverse_insert(struct SkipList *list, unsigned char *key, long data) {
    struct SkipListNode *tmp = NULL;
    struct SkipListNode *newNode = skipListNodeReverse_new(list, key, data);
    int height = list->height;
    int level = SkipList_level(list);
    struct SkipListNode **everyLevel = NULL;
    everyLevel = (struct SkipListNode **) malloc(sizeof (struct SkipListNode *) * (height + 1));
    if (list->head->next == NULL) {
        list->head->next = newNode;
        list->count++;
    } else {
        tmp = list->top;

        while (height >= 1) {
            while (tmp->next != NULL && (strcmp(key, tmp->next->key) > 0)) {
                tmp = tmp->next;
            }
            *(everyLevel + height - 1) = tmp;
            if (tmp->down != NULL) {
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
            newNode = skipListNode_new(list, key, NULL);
            newNode->down = tmp->next;
            tmp = *(everyLevel + i);
            newNode->next = tmp->next;
            tmp->next = newNode;
            list->count++;
        }
        if (level == list->height) {
            newNode = skipListNode_new(list, key, NULL);
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

void reverseNodesFromFile(struct SkipList *list) {
    char *key = NULL;
    long pos = 0;
    struct SkipListNodeStore *node = NULL;
    while ((node = readNodeFromFile(list->StoreNode, &pos, &key)) != NULL) {
        printf("key => %s\n", key);
        skipListNodeReverse_insert(list, key, node->data);
        key = NULL;
        //        free(key);
    }
    fclose(list->StoreNode);
    list->StoreNode = fopen(SKIPLIST_NODES_FILE, "wb+");
    fseek(list->StoreNode, 0, SEEK_SET);
}

int skipList_range(struct SkipList *list, unsigned char *start, unsigned char *end, skipList_callback callback, void *userdata) {
    SkipListNode *tmp = skipListNode_findNode(list, start, 1);
    int len = 0;
    char *data = NULL;
    if (tmp == NULL) {
        return tmp;
    }
    while (tmp != NULL && strcmp(end, tmp->key) >= 0) {
        data = readNodeData(list->dataFile, tmp->data);
        if (data != NULL && callback != NULL) {
            callback(tmp->key, data, userdata);
            ++len;
        }
        tmp = tmp->next;
    }
    return len;
}

int skipList_findn(struct SkipList *list, unsigned char *key, int number, skipList_callback callback, void *userdata) {
    SkipListNode *tmp = skipListNode_findNode(list, key, 1);
    int len = 0;
    int n = 0;
    char *data = NULL;
    if (tmp == NULL) {
        return tmp;
    }
    while (tmp != NULL && n < number) {
        data = readNodeData(list->dataFile, tmp->data);
        if (data != NULL && callback != NULL) {
            callback(tmp->key, data, userdata);
            ++len;
        }
        ++n;
        tmp = tmp->next;
    }
    return len;
}