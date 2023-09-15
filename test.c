/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"knn.h"

void test ()
{
	FILE	*ifp, *ofp;
	double	TotalHit, TotalMiss, Accuracy;
	double	TotalHigh, TotalRange, TotalLow;

	if (( ifp = fopen ( ifile, "r" )) == (FILE *)0 )
	{
		printf ( "Cannot open input file %s\n", ifile );
		exit ( 1 );
	}

	if (( ofp = fopen ( ofile, "w" )) == (FILE *)0 )
	{
		printf ( "Cannot create output file %s\n", ofile );
		exit ( 1 );
	}

	lineno = count_lines ( ifp );

	TrainLines = lineno * 0.8;
	TestLines  = lineno - TrainLines;

	if ( Debug )
	{
		printf ( "lineno %d, Train %d Test %d\n", lineno, TrainLines, TestLines );
	}

	rewind ( ifp );

	lineno = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), ifp ) != (char *)0 )
	{
		lineno++;

		TrimRight ( xbuffer );

		tokcnt = GetTokensStd ( xbuffer, ",", tokens, MAXTOKS, 0 );
		if ( tokcnt < ColumnCount + 1 )
		{
			printf ( "Syntax error in line %d, tokcnt %d\n", lineno, tokcnt );
			continue;
		}

		if ( lineno <= TrainLines )
		{
			AppendTrain ( tokens, tokcnt, 1 );
		}
		else
		{
			AppendTest ( tokens, tokcnt, 1 );
		}
	}

	fclose ( ifp );

	/*----------------------------------------------------------
		find the distance from each test line to all train lines
		save K nearest neighbors
	----------------------------------------------------------*/
	TotalHit = TotalMiss = 0.0;
	TotalHigh = TotalRange = TotalLow = 0.0;
	for ( int e = 0; e < TestLines; e++ )
	{
		for ( int r = 0; r < TrainLines; r++ )
		{
			double d = calc_dist (  Test[e].Row->Columns, Train[r].Row->Columns );

			check_and_save ( e, r, d );

			if ( Debug )
			{
				for ( int c = 0; c < ColumnCount; c++ )
				{
					printf ( " %.3f",  Test[e].Row->Columns[c] );
				}
				if ( KnnType == RESULTS_CLASSIFICATION )
				{
					printf ( " %s ->", Test[e].LabelC );
				}
				else if ( KnnType == RESULTS_REGRESSION )
				{
					printf ( " %g ->", Test[e].LabelR );
				}


				for ( int c = 0; c < ColumnCount; c++ )
				{
					printf ( " %.3f",  Train[r].Row->Columns[c] );
				}

				if ( KnnType == RESULTS_CLASSIFICATION )
				{
					printf ( " %s = %.4f\n", Train[r].LabelC, d );
				}
				else if ( KnnType == RESULTS_REGRESSION )
				{
					printf ( " %g = %.4f\n", Train[r].LabelR, d );
				}

			}
		}

		if ( Debug )
		{
			for ( int k = 0; k < K; k++ )
			{
				printf ( "nearest %.4f %3d\n", Test[e].Neighbors[k].Distance, Test[e].Neighbors[k].Index );
			}
		}

		double	Hit, Miss;	
		Hit = Miss = 0.0;
		for ( int k = 0; k < K; k++ )
		{
			if ( Test[e].Neighbors[k].Index == INT_MAX )
			{
				printf ( "%s: Did not find enough neighbors\n", Test[e].ID );
				exit ( 1 );
			}

			if ( KnnType == RESULTS_CLASSIFICATION )
			{
				if ( strcmp ( Train[Test[e].Neighbors[k].Index].LabelC, Test[e].LabelC) == 0 )
				{
					Hit += 1.0;
					TotalHit += 1.0;
				}
				else
				{
					Miss += 1.0;
					TotalMiss += 1.0;
				}
			}
			else if ( KnnType == RESULTS_REGRESSION )
			{
				double	Percent = 100.0 * Train[Test[e].Neighbors[k].Index].LabelR / Test[e].LabelR;

				if ( Verbose )
				{
					fprintf ( ofp, "Test: %s %g  Train: %s %g  Distance: %g  Percent: %g\n",
						Test[e].ID, 
						Test[e].LabelR, 
						Train[Test[e].Neighbors[k].Index].ID,
						Train[Test[e].Neighbors[k].Index].LabelR,
						Test[e].Neighbors[k].Distance,
						Percent );
				}

				if ( Percent < 80.0 )
				{
					TotalLow++;
				}
				else if ( Percent < 120.0 )
				{
					TotalRange++;
				}
				else
				{
					TotalHigh++;
				}
			}
		}

		if ( KnnType == RESULTS_CLASSIFICATION && Debug )
		{
			Accuracy = Hit * 100.0 / (Hit + Miss);
			printf ( "Accuracy %.2f\n", Accuracy );
		}
	}

	if ( KnnType == RESULTS_CLASSIFICATION )
	{
		Accuracy = TotalHit * 100.0 / (TotalHit + TotalMiss);
	}
	else
	{
		Accuracy = 100.0 * TotalRange / (TotalLow + TotalRange + TotalHigh);
	}
	fprintf ( ofp, "Accuracy %.2f\n", Accuracy );

	fclose ( ofp );
}
