const MAX_STATE = 3;
const MAX_CODE = 4;
const MAX_NAME = 65;
typedef char codetype[MAX_CODE];
typedef char nametype[MAX_NAME];
typedef char statetype[MAX_STATE];
typedef struct airport* airportList;

/* the place */
struct place {
        nametype name;
        statetype state;
	double lat;
	double lon;
};

/* the query */
struct city_state {
        nametype name;
        statetype state;
};

/* the airport */
struct airport {
        codetype code;
        nametype name;
        statetype state;
        double lat;
	double lon;
        double distance;
        airportList next;
};

/* final result returned to the client from the place server*/
struct place_airports {
        place city;
        airportList airports;
};

/* union of places */
union places_reply switch(int error) {
        case 0:
                place_airports list;
        default:
                void;
};

/* places IDL program */
program PLACES_SERVER_PROG {
        version PLACES_SERVER_VERS {
                places_reply PLACES_QUERY(city_state) = 1;
        } = 1;
} = 0x20902687;
