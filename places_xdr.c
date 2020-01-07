/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "places.h"

bool_t
xdr_codetype (XDR *xdrs, codetype objp)
{
	register int32_t *buf;

	 if (!xdr_vector (xdrs, (char *)objp, MAX_CODE,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nametype (XDR *xdrs, nametype objp)
{
	register int32_t *buf;

	 if (!xdr_vector (xdrs, (char *)objp, MAX_NAME,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_statetype (XDR *xdrs, statetype objp)
{
	register int32_t *buf;

	 if (!xdr_vector (xdrs, (char *)objp, MAX_STATE,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airportList (XDR *xdrs, airportList *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct airport), (xdrproc_t) xdr_airport))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_place (XDR *xdrs, place *objp)
{
	register int32_t *buf;

	 if (!xdr_nametype (xdrs, objp->name))
		 return FALSE;
	 if (!xdr_statetype (xdrs, objp->state))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->lat))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->lon))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_city_state (XDR *xdrs, city_state *objp)
{
	register int32_t *buf;

	 if (!xdr_nametype (xdrs, objp->name))
		 return FALSE;
	 if (!xdr_statetype (xdrs, objp->state))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airport (XDR *xdrs, airport *objp)
{
	register int32_t *buf;

	 if (!xdr_codetype (xdrs, objp->code))
		 return FALSE;
	 if (!xdr_nametype (xdrs, objp->name))
		 return FALSE;
	 if (!xdr_statetype (xdrs, objp->state))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->lat))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->lon))
		 return FALSE;
	 if (!xdr_double (xdrs, &objp->distance))
		 return FALSE;
	 if (!xdr_airportList (xdrs, &objp->next))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_place_airports (XDR *xdrs, place_airports *objp)
{
	register int32_t *buf;

	 if (!xdr_place (xdrs, &objp->city))
		 return FALSE;
	 if (!xdr_airportList (xdrs, &objp->airports))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_places_reply (XDR *xdrs, places_reply *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->error))
		 return FALSE;
	switch (objp->error) {
	case 0:
		 if (!xdr_place_airports (xdrs, &objp->places_reply_u.list))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}