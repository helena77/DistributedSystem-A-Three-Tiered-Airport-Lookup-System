const MAX_STATE = 3;
const MAX_CODE = 4;
const MAX_NAME = 65;
typedef char codetypeA[MAX_CODE];
typedef char nametypeA[MAX_NAME];
typedef char statetypeA[MAX_STATE];
typedef struct airportA* airportListA;

/* the latitude and longitude of the city */
struct lat_lon {
        double latitude;
        double longitude;
};

/* the airport */
struct airportA {
        codetypeA code;
        nametypeA name;
        statetypeA state;
        lat_lon location;
        double distance;
        airportListA next;
};

/* union of airports */
union airports_reply switch(int error) {
        case 0:
                airportListA list;
        default:
                void;
};

/* airports IDL program */
program AIRPORTS_SERVER_PROG {
        version AIRPORTS_SERVER_VERS {
                airports_reply AIRPORTS_QUERY(lat_lon) = 1;
        } = 1;
} = 0x20807023;
