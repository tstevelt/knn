/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"knn.h"

void AppendTrain ( char *tokens[], int tokcnt, int HasLabel )
{
	if ( Train == NULL )
	{
		if (( Train = malloc ( sizeof(RECORD) * TrainLines )) == NULL )
		{
			printf ( "malloc Train failed\n" );
			exit ( 1 );
		}
	}

	if ( TrainCount >= TrainLines )
	{
		return;
	}

	if (( Train[TrainCount].Row = malloc ( sizeof(DATA) )) == NULL )
	{
		printf ( "malloc Train.Row failed\n" );
		exit ( 1 );
	}

	if (( Train[TrainCount].Row->Columns = malloc ( sizeof(double) * ColumnCount )) == NULL )
	{
		printf ( "malloc Train.Row->Columns failed\n" );
		exit ( 1 );
	}

	if (( Train[TrainCount].Neighbors = malloc ( sizeof(NEIGHBOR) * K )) == NULL )
	{
		printf ( "malloc Train.Neighbors failed\n" );
		exit ( 1 );
	}
	for ( int k = 0; k < K; k++ )
	{
		 Train[TrainCount].Neighbors[k].Distance = (double)LONG_MAX;
		 Train[TrainCount].Neighbors[k].Index    = INT_MAX;
	}

#ifdef DEBUG
	printf ( "%d Train", TrainCount );
	for ( xt = 0; xt < ColumnCount; xt++ )
	{
		printf ( " %s", tokens[xt] );
	}
	printf ( " %s\n", tokens[xt] );
#endif

	snprintf ( Train[TrainCount].ID, MAXLABEL, "%s",  tokens[0] );
	for ( xt = 1, xc = 0; xt <= ColumnCount; xt++, xc++ )
	{
		Train[TrainCount].Row->Columns[xc] = atof(tokens[xt]);
	}

	if ( HasLabel )
	{
		if ( KnnType == RESULTS_CLASSIFICATION )
		{
			snprintf ( Train[TrainCount].LabelC, MAXLABEL, "%s",  tokens[xt] );
		}
		else if ( KnnType == RESULTS_REGRESSION )
		{
			Train[TrainCount].LabelR = atof ( tokens[xt] );
		}
	}

	TrainCount++;
}
