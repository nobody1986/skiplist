#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>


#define NODES_IN_LEVEL 20
#define NODES_PER_LEVEL_INIT 100
#define NODES_PER_LEVEL_STEP 100
#define SKIPLIST_DATA_FILE "data.dat"
#define SKIPLIST_NODES_FILE "nodes.node"

enum SkipListNodeType {
    NODE_MIN,
    NODE_MAX,
    NODE_NORMAL
};

enum SkipListNodeDataType {
    TypeString,
    TypeInt,
};

struct SkipListNode {
    enum SkipListNodeType type;
    struct SkipListNode *next;
    struct SkipListNode *down;
    unsigned char *key;
    long data;
    short deleted;
};

struct SkipListNodeStore {
    long data;
    short deleted;
    short len;
};

struct SkipList {
    int height;
    int count;
    struct SkipListNode *head;
    struct SkipListNode *top;
    FILE *dataFile;
    FILE *StoreNode;
};




struct SkipList *skipList_new();
struct SkipListNode *skipListNode_new(struct SkipList *, unsigned char *, void *);
struct SkipListNode *skipListSpecialNode_new(enum SkipListNodeType);
void skipListNode_insert(struct SkipList *, unsigned char *, void *);
void *skipListNode_find(struct SkipList *, unsigned char *);
void skipList_free(struct SkipList *);
void SkipListNode_free(struct SkipListNode *);
int SkipList_level(struct SkipList *);
void SkipList_print(struct SkipList *);

long skipListStoreNode_new(struct SkipList *, unsigned char *, long);
long writeNodeToFile(FILE*, struct SkipListNodeStore *, char *);
struct SkipListNodeStore *readNodeFromFile(FILE *, long *, char **);
char *readNodeData(FILE *, long);

void saveNodesToFile(struct SkipList *);
struct SkipListNode *skipListNodeReverse_new(struct SkipList *, unsigned char *, long );
void skipListNodeReverse_insert(struct SkipList *, unsigned char *, long);
void reverseNodesFromFile(struct SkipList *);