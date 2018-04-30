#include <stdlib.h>
#include <stdio.h>

int hash(int key, int size) {
    long int ind = (((179443142 * (long int)key + 3160623869)>>4 % 65309) + 
                ((179443142 * (long int)key + 3160623869)<<3 % 65309)) % size;
    // printf("%ld\n", ind);
    if (ind < 0) {
        return -ind;
    }
    return ind;
}