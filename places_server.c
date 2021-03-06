/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "places.h"
#include "airports.h"
#include "Tries.h"
#include "Linkedlist.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

struct TrieNode* root;
int begin = 0;

places_reply *
places_query_1_svc(city_state *argp, struct svc_req *rqstp)
{
	static places_reply  result;
	city_state city;
	char* userinput;
	double lat;
	double lon;
	struct Node* prefixList = NULL;
	struct Node* prefixListDup = NULL;
	int size = 0;

	char *host = "localhost";
        CLIENT *clnt;
        airports_reply  *result_1;
        lat_lon  airports_query_1_arg;
        airportList list;
        airportList* listP;
        airportListA listA;

	/* 
         * build a node and  read file  
         */
        if (begin == 0) {
                root = getNode();
                buildTrie(root);
                begin = 1;
        }

	/* 
 	 * free previous result
 	 */
	xdr_free((xdrproc_t)xdr_places_reply, (char *)&result);
	
	/*
 	 * copy name to argument
 	 */
	strcpy(city.name, argp->name);
        strcpy(city.state, argp->state);
	userinput = concatAndLowercase(city.state, city.name);	
	
	prefixList = (struct Node* ) malloc(sizeof(struct Node));
	prefixList->val = '\0';
	prefixList->next = NULL;
	prefixList->state = '\0';
	prefixList->city = '\0';
	prefixList->lat = 0;
	prefixList->lon = 0;
	prefixList = keys_with_prefix(root, userinput);

	prefixListDup = (struct Node* ) malloc(sizeof(struct Node));
	prefixListDup = prefixList;
	while (prefixListDup->next != NULL) {
		size++;
		prefixListDup = prefixListDup->next;
	}

        if (size == 1) {	
		/*
                 * append name and state to result
                 */		
                strcpy(result.places_reply_u.list.city.name, prefixList->next->city); 		
                strcpy(result.places_reply_u.list.city.state, lower(city.state));
                result.places_reply_u.list.city.lat = prefixList->next->lat;
                result.places_reply_u.list.city.lon = prefixList->next->lon;
 
		/* 
	         * getting  airports list from airports server 
                 */
	        #ifndef DEBUG
        	clnt = clnt_create (host, AIRPORTS_SERVER_PROG, AIRPORTS_SERVER_VERS, "udp");
        	if (clnt == NULL) {
                	clnt_pcreateerror (host);
        	}
		#endif  /* DEBUG */

                airports_query_1_arg.latitude = result.places_reply_u.list.city.lat;
                airports_query_1_arg.longitude = result.places_reply_u.list.city.lon;
                result_1 = airports_query_1(&airports_query_1_arg, clnt);
                if (result_1 == (airports_reply *) NULL) {
                        clnt_perror (clnt, "call failed");
			exit(1);
                }
		if (result_1->error > 0) {
			perror("wrong latitude and longitude");
			clnt_freeres (clnt,(xdrproc_t)xdr_airports_reply,(char *)result_1);
			return &result;
		}
		if (result_1->error == -1) {
			perror("no airports found!");
			return &result;
		}
                listP = &result.places_reply_u.list.airports;
                for (listA = result_1->airports_reply_u.list; listA != NULL; listA = listA->next) {
                        list = (airport *) malloc(sizeof(airport));
                        strcpy(list->code, listA->code);
                        strcpy(list->name, listA->name);
                        strcpy(list->state, listA->state);
                        list->distance = listA->distance;
                        list->next = NULL;
                        *listP = list;
                        listP = &list->next;
                }
		clnt_freeres (clnt,(xdrproc_t)xdr_airports_reply,(char *)&result_1);
        	#ifndef DEBUG
        	clnt_destroy (clnt);
        	#endif   /* DEBUG */
                result.error = 0;
        } else if (size == 0) {
                result.error = -1;
        } else if (size > 1) {
		printf("\nMany cities found as below:\n");
		print_list(prefixList);
                result.error = 1;
        }

	return &result;
}
