#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kfifo.h"

struct KfifoNode {
    unsigned int num;
    char *string;
};

char *array[] = {
    "abcdefg",
    "gfedcba",
    "aaaaa",
    "bbbb",
    "ccc",
    "dd",
    "e",
    "12345",
    "1234",
    "123",
    "12",
    "1",
    "1111",
};

#define TAB_SIZE(array) (sizeof(array)/sizeof(array[0]))

static struct kfifo *pkfifoSpace;
static spinlock_t fifoLock; 

static void kfifo_check(char* str, int line, struct kfifo* pkfifo) 
{
    if(pkfifo != NULL) {
        printf("[%s-%d]: pkfifo->size = %-4d pkfifo->in = %-4d pkfifo->out = %-4d ", str, line, pkfifo->size, pkfifo->in, pkfifo->out);
    }
}


#ifndef GFP_KERNEL
#define GFP_KERNEL  1
#endif

int main(int argc, char* argv[])
{
    int i; 
    struct KfifoNode *pstNode;

    pkfifoSpace = kfifo_alloc((sizeof(struct KfifoNode) << 4), GFP_KERNEL, &fifoLock);
    if (pkfifoSpace == NULL) {
        printf("kfifo_alloc failed !\n");
        return -EFAULT;
    }
    spin_lock_init(&fifoLock); //Initial fifo spinlock 
    //pstNode = kzalloc(sizeof(struct KfifoNode), GFP_KERNEL);
    pstNode = malloc(sizeof(struct KfifoNode));
    printf("******insert*********************************************************************\n");
    kfifo_check((char *)__func__, __LINE__, pkfifoSpace);
    printf("\n");
    for(i = 0; i < TAB_SIZE(array); i++) {
        pstNode->num = i;
        pstNode->string = (char *)array[i];
        kfifo_put(pkfifoSpace, (unsigned char *)pstNode, sizeof(struct KfifoNode)); //将数据写入缓冲区
        kfifo_check((char *)__func__, __LINE__, pkfifoSpace);
        printf("Num: %-3d, Message: %s\n", pstNode->num, pstNode->string);
    }
    if(!kfifo_len(pkfifoSpace)) {
        printf("[%s-%d]: kfifo_len return 0, test failed !!! \n", __func__, __LINE__);
        kfifo_reset(pkfifoSpace);
        kfifo_free(pkfifoSpace);

        return -1;
    }

    printf("------get-and-display------------------------------------------------------------\n");
    //for(i = 0; i < TAB_SIZE(array); i++) {
    while(kfifo_len(pkfifoSpace)) {
        kfifo_get(pkfifoSpace, (unsigned char *)pstNode, sizeof(struct KfifoNode));
        kfifo_check((char *)__func__, __LINE__, pkfifoSpace);
        printf("Num is: %-3d, fifoMessage is: %s\n", pstNode->num, pstNode->string);
    }
    printf("------get-all-over----------------------------------------------------------------\n");
    kfifo_check((char *)__func__, __LINE__, pkfifoSpace);
    printf("\n--------------------------------------------------------------------------------\n");
    free(pstNode);
    kfifo_reset(pkfifoSpace);
    kfifo_free(pkfifoSpace);

    return  0;
}