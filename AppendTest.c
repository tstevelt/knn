/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"knn.h"

void AppendTest ( char *tokens[], int tokcnt, int HasLabel )
{
	if ( Test == NULL )
	{
		if (( Test = malloc ( sizeof(RECORD) * TestLines )) == NULL )
		{
			printf ( "malloc Test failed\n" );
			exit ( 1 );
		}
	}

	if ( TestCount >= TestLines )
	{
		return;
	}

	if (( Test[TestCount].Row = malloc ( sizeof(DATA) )) == NULL )
	{
		printf ( "malloc Test.Row failed\n" );
		exit ( 1 );
	}

	if (( Test[TestCount].Row->Columns = malloc ( sizeof(double) * ColumnCount )) == NULL )
	{
		printf ( "malloc Test.Row->Columns failed\n" );
		exit ( 1 );
	}

	if (( Test[TestCount].Neighbors = malloc ( sizeof(NEIGHBOR) * K )) == NULL )
	{
		printf ( "malloc Test.Neighbors failed\n" );
		exit ( 1 );
	}
	for ( int k = 0; k < K; k++ )
	{
		 Test[TestCount].Neighbors[k].Distance = (double)LONG_MAX;
		 Test[TestCount].Neighbors[k].Index    = INT_MAX;
	}

#ifdef DEBUG
	printf ( "%d Test", TestCount );
	for ( xt = 0; xt < tokcnt; xt++ )
	{
		printf ( " %s", tokens[xt] );
	}
	printf ( " %s\n", tokens[xt] );
#endif

	snprintf ( Test[TestCount].ID, MAXLABEL, "%s",  tokens[0] );
	for ( xt = 1, xc = 0; xt <= ColumnCount; xt++, xc++ )
	{
		Test[TestCount].Row->Columns[xc] = atof(tokens[xt]);
	}

	if ( HasLabel )
	{
		if ( KnnType == RESULTS_CLASSIFICATION )
		{
			snprintf ( Test[TestCount].LabelC, MAXLABEL, "%s",  tokens[xt] );
		}
		else if ( KnnType == RESULTS_REGRESSION )
		{
			Test[TestCount].LabelR = atof ( tokens[xt] );
		}
	}

	TestCount++;
}

void DeleteLastTest ()
{
	TestCount--;

	free ( Test[TestCount].Neighbors );
	free ( Test[TestCount].Row->Columns );
	free ( Test[TestCount].Row );
}
