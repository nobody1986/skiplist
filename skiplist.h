#define NODES_IN_LEVEL 20
#define NODES_PER_LEVEL_INIT 100
#define NODES_PER_LEVEL_STEP 100

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
    int next;
    int down;
    char type;
    int key;
    int data;
};

struct SkipList {
    int height;
    int count;
    void *head;
    void *top;
};


struct SkipList *skipList_new();
struct SkipListNode *skipListNode_new(unsigned char *, void *);
struct SkipListNode *skipListSpecialNode_new(enum SkipListNodeType);
void skipListNode_insert(struct SkipList *, unsigned char *, void *);
void *skipListNode_find(struct SkipList *, unsigned char *);
void skipList_free(struct SkipList *);
void SkipListNode_free(struct SkipListNode *);
int SkipList_level(struct SkipList *);
void SkipList_print(struct SkipList *);
