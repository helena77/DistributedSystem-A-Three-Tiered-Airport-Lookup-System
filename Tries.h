/*
 * Tries.h
 *
 * Created on: Nov 7, 2019
 * Author: helenaw
 */

#ifndef TRIES_H_
#define TRIES_H_
#include <stdbool.h>
#define ALPHABET_SIZE (256)
#define CITYLENGTH 64   //length of city name
#define STATELENGTH 2   //length of state

// returns new trie node
struct TrieNode* getNode(void);

// return all keys with prefix
struct Node* keys_with_prefix(struct TrieNode* root, char prefix[]);

// inserts key into trie.
void insert(struct TrieNode* root, char key[], double lat, double lon, int d, char state[], char city[]);

// search for the key, return last node of the key
struct TrieNode* search(struct TrieNode* root, char key[]);

// lower case
char* concatAndLowercase(char* x, char* y);
char* lower(char* c);

// build trie
int buildTrie(struct TrieNode* root);

// trieNode
struct TrieNode
{
	struct TrieNode* children[ALPHABET_SIZE];
	double lat;
	double lon;
	// isWordEnd is true if the node represents
        // end of a word
        bool isWordEnd;
        char state[STATELENGTH];
	char city[CITYLENGTH];
};


#endif /* TRIES_H_ */


