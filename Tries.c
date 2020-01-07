/*
 * Tries.c
 *
 *  Created on: Nov 4, 2019
 *      Author: neald
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "Linkedlist.h"
#include "Tries.h"
#define ALPHABET_SIZE (256)
#define CHAR_TO_INDEX(c) ((int)c)
#define CITYLENGTH 64   //length of city name
#define LINELENGTH 255 //length of line
#define FILENAME "places2k.txt" //file name
#define LATLENGTH 10 //length of lat
#define LONLENGTH 11  //length of lon
#define STATELENGTH 2   //length of state

/* private helper functions */

//collect all trienode with the same prefix
static void collect(struct TrieNode* x, char prefix[], struct Node* head);

//private insert function called recursively
static struct TrieNode* put(struct TrieNode* x, char key[], double lat, double lon, int d, char state[], char city[]);

//private get function called recursively
static struct TrieNode* get(struct TrieNode* x, char key[], int d);


//wipe all space at the tail of a string
static char *ltrim(const char *s);


// returns new trie node
struct TrieNode* getNode(void)
{
	struct TrieNode* pNode = NULL;

	pNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
	pNode->isWordEnd = false;
	pNode->lat = 0;
	pNode->lon = 0;
	pNode->state[0] = '\0';
	pNode->city[0] = '\0';

	for (int i = 0; i < ALPHABET_SIZE; i++)
		pNode->children[i] = NULL;

	return pNode;
}


// If not present, inserts key into trie. If the
// key is prefix of trie node, just marks leaf node
void insert(struct TrieNode* root, char key[], double lat, double lon, int d, char state[], char city[])
{
	struct TrieNode* pCrawl = root;
	pCrawl = (struct TrieNode*)put(pCrawl, key, lat, lon, 0, state, city);
	//printf("Inside insert:\ncity is %s, state is %s, lat is %f, lon is %f\n", city, state, lat, lon);
}


//search for the key
struct TrieNode* search(struct TrieNode* root, char key[])
{
	struct TrieNode* temp = get(root, key, 0);
	if (!temp) {
		return false;
	}
	//return true;
	return (struct TrieNode*)temp;

}

//find TrieNode of which the keys have the same prefix
struct Node* keys_with_prefix(struct TrieNode* root, char prefix[])
{
	struct Node* head = NULL;
	head = malloc(sizeof(node_t));
	head->val = "start";
	head->next = NULL;
	collect(get(root, prefix, 0), prefix, head);
	return head;
}


int buildTrie(struct TrieNode* root)
{
	FILE* file;
	double lat, lon;
	char line[LINELENGTH], state[STATELENGTH + 1], name[CITYLENGTH + 1], lats[LATLENGTH + 1], lons[LONLENGTH + 1];
	char* key;
	char* city;
	//open file
	if((file = fopen(FILENAME, "r")) == NULL)
	{
		printf("Failed to open the file");
		return -1;
	}
	//for debugging
	int count = 0;

	//read file
	while(fgets(line, 255, file) != NULL)
	{
		//Check for dos and unix EOL format
		if((line[0] != '\n') & (strlen(line) > STATELENGTH))
		{
			//read state
			strncpy(state, strdup(line), STATELENGTH);
			state[STATELENGTH] = '\0';

			//read name
			strncpy(name, strdup(line + 9), CITYLENGTH);
//			char *nametemp;
//			nametemp = strtok(name, '\t');
			name[CITYLENGTH] = '\0';
			city = ltrim(name);


			//concat state and name as a key, lower case
			key = concatAndLowercase(state, city);

			//read lat
			strncpy(lats, strdup(line + 143), LATLENGTH);
			lats[LATLENGTH] = '\0';
			lat = atof(lats);

			//read lon
			strncpy(lons, strdup(line + 153), LONLENGTH);
			lons[LONLENGTH] = '\0';
			lon = atof(lons);
			count++;

			//add to the trie
			insert(root, key, lat, lon, 0, state, city);

//			//for debugging
//			if(count == 5)
//			{
//				printf("city is %s, state is %s, lat is %f, lon is %f\n", name, state, lat, lon);
//			}

		}
		else
		{
		    continue;
		}
	}
	fclose(file);
	return 0;
}


static void collect(struct TrieNode* x, char prefix[], struct Node* head)
{
	if (x == NULL)
	{
		return;
	}
	//int size = strlen(prefix);
	int index;
	if (x->state != NULL)
	{

		if(x->isWordEnd){
			//printf("prefix is %s\n", prefix);
			push(head, prefix, x->state, x->city, x->lat, x->lon);
//			printf("city is %s, state is %s, lat is %f, lon is %f\n", x->city, x->state, x->lat, x->lon);
		}
	}
	for (index = 0; index < ALPHABET_SIZE; index++) {
		//append current character to the prefix
		char* temp = NULL;
		char tempchar = index;
		int length = strlen(prefix);
		temp = malloc(length + 1 + 1);
		strcpy(temp, prefix);
		strncat(temp, &tempchar, 1);
		collect(x->children[index], temp, head);
	}
}


//private insert function called recursively
static struct TrieNode* put(struct TrieNode* x, char key[], double lat, double lon, int d, char state[], char city[])
{

if (!x) {
		x = (struct TrieNode*)getNode();
	}
	if (d == strlen(key)) {
		x->lat = lat;
		x->lon = lon;
		x->isWordEnd = true;
//		printf("Inside put before copy:\ncity is %s, state is %s, lat is %f, lon is %f\n", city, state, lat, lon);
		//need to test if this works
//		strcpy(x->state, state);
//		strcpy(x->city, city);
		strncpy(x->state, strdup(state), STATELENGTH);
		strcpy(x->city, city);
		//for debugging
//		printf("Inside put:\ncity is %s, state is %s, lat is %f, lon is %f\n", x->city, x->state, x->lat, x->lon);
		return (struct TrieNode*)x;
	}
	//go to the next node
	char c = (char)key[d];
	int index = CHAR_TO_INDEX(c);
	if(index < 0)
	{
		index = index + 255;
	}
	x->children[index] = (struct TrieNode*)put(x->children[index], key, lat, lon, d + 1, state, city);
	return (struct TrieNode*)x;
}

//private get function called recursively
static struct TrieNode* get(struct TrieNode* x, char key[], int d)
{
	if (!x) {
		return NULL;
	}
	if (d == strlen(key)) {
		return (struct TrieNode*)x;
	}

	//go to the next node
	char c = (char)key[d];
	int index = CHAR_TO_INDEX(c);
	return (struct TrieNode*)get(x->children[index], key, d + 1);
}

//returns all lowercase string
char* lower(char* c)
{
  char* temp = strdup(c);
  int count = 0;

  while(temp[count] != '\0'){
	temp[count] = tolower(temp[count]);
	count++;
  }

  return temp;
}

// Function to remove all spaces from a given string
static void removeSpaces(char *str)
{
    // To keep track of non-space character count
    int count = 0;

    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i]; // here count is
                                   // incremented
    str[count] = '\0';
}

//concat name and state as a key, lower case
char* concatAndLowercase(char* x, char* y)
{
	//concat name and state as a key, lower case
	char temp[STATELENGTH + CITYLENGTH];
	char* key;
	strcpy(temp, x);
	strcat(temp, y);
	removeSpaces(temp);
	key = lower(temp);
	return key;
}

static char *ltrim(const char *s)
{
  char *r = strdup(s);
  if (r != NULL)
  {
    char *fr = r + strlen(s) - 1;
    while( (isspace(*fr) || !isprint(*fr) || *fr == 0) && fr >= r) --fr;
    *++fr = 0;
  }
  return r;
}

static int sizeOfResult(struct Node* result)
{
	int size = 0;
	struct Node* tempnode = NULL;
	tempnode = (struct Node*)malloc(sizeof(struct Node));
	tempnode = result;
	while(tempnode->next !=NULL )
	{
		size++;
		tempnode = tempnode->next;
	}
	return size;
}

