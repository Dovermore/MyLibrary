/* Header guard */
#ifndef _INTHASH_H
#define _INTHASH_H


typedef struct hash_table_s HashTable;

struct hash_table_s {
    int size_table,
        *size_chain,
        *nele_chain;
    Item **table;
};

int hash(int key, int size);

HashTable *new_hash_table(int size);

void hash_insert(HashTable *h, int key, int data);

int hash_check_key(HashTable *h, int key);

int hash_return_data(HashTable *h, int key);

int hash_remove_key(HashTable *h, int key);

int hash_update_data(HashTable *h, int key, int data);

void free_hash_table(HashTable *h);

void print_hash_table(HashTable *h, int show);


#endif