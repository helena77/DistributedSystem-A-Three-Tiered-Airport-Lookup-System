/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "airports.h"
#include <math.h>
#include "kdtree.h"
#include <string.h>

#define DIM 2           	//dimension
#define CODELENGTH 3   		//length of city code
#define CITYLENGTH 42   	//length of city name and state
#define NAMELENGTH 40		//length of city name
#define STATELENGTH 2		//length of city state
#define LINELENGTH 255 		//length of line
#define LATLENGTH 5  		//length of lat
#define LONLENGTH 7  		//length of lon
#define NUMOFRESULT 5
#define FILENAME "airport-locations.txt" //file name

struct kdtree* tree;
int begin = 0;

int buildTree(struct kdtree *tree)
{
	FILE* file;
	double lat, lon;
	char line[LINELENGTH], code[CODELENGTH + 1], city[CITYLENGTH + 1], name[NAMELENGTH + 1], 
		state[STATELENGTH + 1], lats[LATLENGTH + 1], lons[LONLENGTH + 1];

	//open file
	if((file = fopen(FILENAME, "r")) == NULL)
	{
		printf("Failed to open the file");
		return -1;
	}
	//for debugging
	int count = 0;
	int countblank = 0;
	//read file
	while(fgets(line, 255, file) != NULL)
	{	
		//skip the first line, empty line and line without airport name
		if (strchr(line,',') == NULL)
		{
			countblank++;
			continue;
		}
		else
		{
			//read code
			strncpy(code, strdup(line + 1), CODELENGTH);
			code[CODELENGTH] = '\0';
			
			//read lat
			strncpy(lats, strdup(line + 6), LATLENGTH);
			lats[LATLENGTH] = '\0';
			lat = atof(lats);

			//read lon
			strncpy(lons, strdup(line + 12), LONLENGTH);
			lons[LONLENGTH] = '\0';
			lon = atof(lons);
			
			//read name and state
			strncpy(city, strdup(strchr(line,'\t') + 1), CITYLENGTH);
			char *p = strchr(city,'\n');
			//end of the string
			if (p != NULL)
				*p = '\0';
			
			int init_size = strlen(city);
			char delim[] = ",";
			char *ptr = strtok(city, delim);
			strncpy(name, ptr, NAMELENGTH);
			ptr = strtok(NULL, delim);
			strncpy(state, ptr, STATELENGTH);
			
				
			airportA* temp = malloc(sizeof(airportA));
			strcpy(temp->code, code);
			strcpy(temp->name, name);
			strcpy(temp->state, state);
			temp->location.latitude = lat;
			temp->location.longitude = lon;
			temp->distance = 1111111;
		
			double coordi[2];
			coordi[0] = lat;
			coordi[1] = lon;
			if((kd_insert(tree, coordi, temp)) != 0)
			{
				printf("fail to insert %s", code);
				return -1;
			}
			count++;
		}
	}
	fclose(file);
	return 0;
}


void nearest_5(struct kdtree* tree, airportListA* listP, double lat, double lon)
{
	airportListA pch;
	airportListA list;
	double dist;
	double pos[DIM] = {lat, lon};
	double resultpos[DIM];
	void* presults = kd_nearest_n(tree, pos, NUMOFRESULT);
	int count = 0;

	while (!kd_res_end(presults)) {
		/* get the data and position of the current result item */
		pch = (airportA*) kd_res_item(presults, resultpos);
		list = (airportA*) malloc(sizeof(airportA));			
		strcpy(list->code, pch->code);	
		strcpy(list->name, pch->name);
		strcpy(list->state, pch->state);
		list->location.latitude = pch->location.latitude;
		list->location.longitude = pch->location.longitude;
		/* compute distance */
		dist = distance(pos[0], pos[1], list->location.latitude, list->location.longitude, 'M');
		list->distance = dist;
		list->next = NULL;
		*listP = list;
		listP = &list->next;			
		/* go to the next entry */
		kd_res_next(presults);
	}
	/* free our tree, results set, and other allocated memory */
	kd_res_free(presults);
	kd_free(tree);
}

airports_reply *
airports_query_1_svc(lat_lon *argp, struct svc_req *rqstp)
{
	static airports_reply  result;
	airportListA list;
        airportListA* listP;
	double lat;
	double lon;

	/* 
 	 * read file
 	 */
	if (begin == 0) {
		tree = kd_create(DIM);
		buildTree(tree);
		begin = 1;
	}

	/* 
         * free previous result
         */
        xdr_free((xdrproc_t)xdr_airports_reply, (char *)&result);

	/* 
  	 * getting 5 nearest airports
  	 */
	lat = argp->latitude;
	lon = argp->longitude;
	listP = &result.airports_reply_u.list;	
	nearest_5(tree, listP, lat, lon);
	

	if (listP == NULL) {
		result.error = -1;
	} else { 
		result.error = 0;
	}
	
	return &result;
}
