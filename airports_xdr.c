/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "airports.h"

bool_t
xdr_codetypeA (XDR *xdrs, codetypeA objp)
{
	register int32_t *buf;

	 if (!xdr_vector (xdrs, (char *)objp, MAX_CODE,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nametypeA (XDR *xdrs, nametypeA objp)
{
	register int32_t *buf;

	 if (!xdr_vector (xdrs, (char *)objp, MAX_NAME,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_statetypeA (XDR *xdrs, statetypeA objp)
{
	register int32_t *buf;

	 if (!xdr_vector (xdrs, (char *)objp, MAX_STATE,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airportListA (XDR *xdrs, airportListA *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct airportA), (xdrproc_t) xdr_airportA))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_lat_lon (XDR *xdrs, lat_lon *objp)
{
	register int32_t *buf;

	 if (!xdr_double (xdrs, &objp->latitude))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->longitude))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airportA (XDR *xdrs, airportA *objp)
{
	register int32_t *buf;

	 if (!xdr_codetypeA (xdrs, objp->code))
		 return FALSE;
	 if (!xdr_nametypeA (xdrs, objp->name))
		 return FALSE;
	 if (!xdr_statetypeA (xdrs, objp->state))
		 return FALSE;
	 if (!xdr_lat_lon (xdrs, &objp->location))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->distance))
		 return FALSE;
	 if (!xdr_airportListA (xdrs, &objp->next))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airports_reply (XDR *xdrs, airports_reply *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->error))
		 return FALSE;
	switch (objp->error) {
	case 0:
		 if (!xdr_airportListA (xdrs, &objp->airports_reply_u.list))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}
