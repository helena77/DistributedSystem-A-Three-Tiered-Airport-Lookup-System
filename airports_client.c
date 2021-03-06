/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "airports.h"


void
airports_server_prog_1(char *host)
{
	CLIENT *clnt;
	airports_reply  *result_1;
	lat_lon  airports_query_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, AIRPORTS_SERVER_PROG, AIRPORTS_SERVER_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = airports_query_1(&airports_query_1_arg, clnt);
	if (result_1 == (airports_reply *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	airports_server_prog_1 (host);
exit (0);
}
