/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"knn.h"

void check_and_save ( int TestIndex, int TrainIndex, double Distance )
{
	NEIGHBOR	*p;

	p = Test[TestIndex].Neighbors;
	for ( int k = 0; k < K; k++ )
	{
		if ( Distance < p[k].Distance )
		{
			if ( k + 1 < K )
			{
				memmove ( &p[k+1], &p[k], sizeof(NEIGHBOR) * (K-k-1) );
			}
			p[k].Distance = Distance;
			p[k].Index = TrainIndex;
			return;
		}
	}
}
