/* 
 * File:   main.c
 * Author: snow
 *
 * Created on 2015年9月19日, 下午8:22
 */

#include <stdio.h>
#include <stdlib.h>
#include "skiplist.h"

/*
 * 1000000
real	0m0.414s
user	0m0.358s
sys	0m0.056s
 * 10000000
real	0m4.302s
user	0m3.641s
sys	0m0.591s
 * 
 * 
 * ====skip=====
 * 
 * 10000000
real	0m5.278s
user	0m4.494s
sys	0m0.720s
 * 1000000
real	0m0.507s
user	0m0.431s
sys	0m0.064s

 * 
 * write 100 read 10000
 * 
real	0m0.426s
user	0m0.408s
sys	0m0.012s
 * 
write 1000 read 10000
 * 
real	1m13.470s
user	1m12.358s
sys	0m0.836s
 * 
 * 
write 100000 read 1000 * 100000
HEIGHT:10
COUNT:149770

real	1m2.052s
user	0m55.911s
sys	0m0.921s
 */
int main(int argc, char** argv) {
    struct SkipList *list = skipList_new();
    char *key = NULL;
    char *data = NULL;
    for (int i = 0; i < 100000; ++i) {
        key = (char *) malloc(sizeof (char) * 16);
        data = (char *) malloc(sizeof (char) * 16);
        snprintf(key, 15, "key%010d", i);
        snprintf(data, 15, "data%010d", i);
        skipListNode_insert(list, key, data);
//        char *ret = (char *) skipListNode_find(list, key);
        //                        printf("get => %s\n",ret);
        //    SkipList_print(list);
    }
    for (int n = 0; n < 100; ++n) {
        for (int i = 0; i < 100000; ++i) {
            key = (char *) malloc(sizeof (char) * 16);
            //            data = (char *) malloc(sizeof (char) * 16);
            snprintf(key, 15, "key%010d", i);
            //            sprintf(data, "data%d", i);
            //            skipListNode_insert(list, key, data);
            char *ret = (char *) skipListNode_find(list, key);
//            printf("get => %s\n", ret);
        }
    }
    SkipList_print(list);
    return (EXIT_SUCCESS);
}

