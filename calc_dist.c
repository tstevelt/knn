/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"knn.h"

double calc_dist ( double *From, double *To )
{
	double	rv;
	rv = 0.0;
	for ( int c = 0; c < ColumnCount; c++ )
	{
		rv = rv + pow(To[c] - From[c], 2 );
	}
	rv = sqrt ( rv );
	return ( rv );
}
