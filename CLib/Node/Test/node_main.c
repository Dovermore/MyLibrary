/*****************************************************************************
 *Author   : Calvin Zhuoqun Huang
 *Licence  : None
 *File Name: node_main.c
 
 *Brief:
    Testing node module
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../node.h"

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
#define INVALID -1
/* </MacroDefs> */


/* <TypeDefs> */
/* typedef */
/* </TypeDefs> */


/* <FunctionPrototypes> */
/* –––––––––––––––––––– */
/* </FunctionPrototypes> */



int
main(int argc, char *argv[]) {
    Node *node1 = new_node(1, sizeof(float), sizeof(int), 1),
         *node2 = new_node(2, sizeof(float), 10*sizeof(char), 2),
         *node3 = new_node(1, sizeof(int*), sizeof(float), 2);
    char ary1[10] = "123456789\0",
         ary2[10] = "987654321\0";
    int *i = (int[5]){1,2,3,5,9},
        tmp = 10,
        **pi = &i;
    float tmp2 = 1.55555;
    printf("pointer i's address is %p\n", i);
    printf("1\n");
    ai(node1, &tmp2, &tmp, 0);
    printf("2\n");
    ai(node2, &tmp2, ary1, 0);
    printf("3\n");
    ai(node2, &tmp2, ary2, 1);
    printf("4\n");
    ai(node3, pi, &tmp2, 0);
    printf("5\n");
    printf("data in node 1 %d\n", *(int*)cd(node1, 0));
    printf("key in node 1 %f\n", *(float*)ck(node1, 0));
    printf("data in node 2 %s\n", (char*)cd(node2, 0));
    printf("data in node 2 %s\n", (char*)cd(node2, 1));
    int *c = *(int**)ck(node3, 0);
    printf("pointer in key address is %p\n", c);
    for (int j=0; j<5; j++) {
        printf("data pointer in node 3 %p\n", c);
        printf("data in node 3 %d\n", ((int*)c)[j]);
        printf("data in i %d\n", i[j]);
    }
    printf("Check the linking functionality\n");
    ac(node1, node3, 0);
    ac(node2, node3, 0);
    ac(node2, node3, 1);
    ac(node3, node3, 0);
    ac(node3, node1, 1); 
    printf("data in node 1's first child is %f\n", *(float*)cd(cc(node1, 0), 0));
    return 0;
}