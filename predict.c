/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"knn.h"

static	int		DebugPredict = 0;

void predict ()
{
	int		HasModel = 0;
	FILE	*pfp, *ifp, *ofp, *mfp;
	MIN_MAX_RECORD		*MinMax = NULL;
	double	*StddevArray;
	NEIGHBOR	*p;

	if (( pfp = fopen ( pfile, "r" )) == (FILE *)0 )
	{
		printf ( "Cannot open predict file %s\n", pfile );
		exit ( 1 );
	}

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

	if ( nsStrlen ( mfile ) > 0 )
	{
		if (( mfp = fopen ( mfile, "r" )) == (FILE *)0 )
		{
			printf ( "Cannot open model file %s\n", mfile );
			exit ( 1 );
		}
		HasModel = 1;

		/*----------------------------------------------------------
			alloc space for model. don't know how manu lines, so use MAX 
		----------------------------------------------------------*/
		if (( MinMax = calloc ( sizeof(MIN_MAX_RECORD), MAXCOLS )) == NULL )
		{
			printf ( "Cannot calloc MinMax\n" );
			exit ( 1 );
		}

		/*----------------------------------------------------------
			read model
		----------------------------------------------------------*/
		while ( fgets ( xbuffer, sizeof(xbuffer), mfp ) != NULL )
		{
			if (( tokcnt = GetTokensW ( xbuffer, tokens, MAXTOKS )) < 3 )
			{
				printf ( "predict: syntax error in model file\n" );
				exit ( 1 );
			}
			int Column = atoi ( tokens[0] );
			if ( Column >= MAXCOLS )
			{
				printf ( "predict: syntax error in model file\n" );
				exit ( 1 );
			}
			MinMax[Column].Min = atof(tokens[1]);
			MinMax[Column].Max = atof(tokens[2]);
		}

		printf ( "predict: loaded model\n" );
	}

	
	/*----------------------------------------------------------
		read entire input file (training data) into memory
		if has model, then expected to be normalized.
	----------------------------------------------------------*/
	TrainLines = count_lines ( ifp );
	rewind ( ifp );

	lineno = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), ifp ) != (char *)0 )
	{
		TrimRight ( xbuffer );

		tokcnt = GetTokensStd ( xbuffer, ",", tokens, MAXTOKS, 0 );
		if ( tokcnt < ColumnCount + 2 )
		{
			printf ( "Syntax error in line %d, tokcnt %d\n", lineno, tokcnt );
			continue;
		}

		AppendTrain ( tokens, tokcnt, 1 );

		if ( HasModel )
		{
			int		OverOne = 0;
			for ( int c = 0; c < ColumnCount; c++ )
			{
				if ( Train[lineno].Row->Columns[c] > 1.0 )
				{
					OverOne++;
				}
			}
			if ( OverOne )
			{
				printf ( "Training data is not normalized, lineno %d\n", lineno );
				exit ( 1 );
			}
		}

		lineno++;
	}
	printf ( "predict: loaded %d training rows\n", lineno );

	/*----------------------------------------------------------
		read predict data.
	----------------------------------------------------------*/
	TestLines = count_lines ( pfp );
	rewind ( pfp );

	lineno = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), pfp ) != (char *)0 )
	{
		lineno++;

		TrimRight ( xbuffer );

		if ( DebugPredict )
		{
			printf ( "%d: %s\n", lineno, xbuffer );
		}

		tokcnt = GetTokensStd ( xbuffer, ",", tokens, MAXTOKS, 0 );
		if ( tokcnt < ColumnCount + 1 )
		{
			printf ( "Syntax error in line %d, tokcnt %d\n", lineno, tokcnt );
			continue;
		}

		AppendTest ( tokens, tokcnt, 0 );

		if ( HasModel )
		{
			int		AssertFailure = 0;
			for ( xt = 1, xc = 0; xt <= ColumnCount; xt++, xc++ )
			{
				double	oval;
				double	nval;

				oval = Test[TestCount-1].Row->Columns[xc];

				if ( MinMax[xc].Min > oval || oval > MinMax[xc].Max )
				{
					if ( Debug )
					{
						printf ( "%s assert col %d: %g < %g < %g\n", 
							Test[TestCount-1].ID, xc, MinMax[xc].Min, oval,  MinMax[xc].Max );
					}
					AssertFailure = 1;
					break;
				}

				nval = (oval - MinMax[xc].Min) / (MinMax[xc].Max - MinMax[xc].Min);
				Test[TestCount-1].Row->Columns[xc] = nval;

				if ( DebugPredict )
				{
					printf ( " [%g %g]", oval, nval );
				}
			}
			if ( DebugPredict )
			{
				printf ( "\n" );
			}

			if ( AssertFailure )
			{
				DeleteLastTest ();
				continue;
			}
		}
	}

	printf ( "predict: loaded %d predict rows, kept %d, %d out of model range\n", 
			TestLines, TestCount, TestLines - TestCount );


	if ( KnnType == RESULTS_REGRESSION )
	{
		if (( StddevArray = malloc ( sizeof(double) * K )) == NULL )
		{
			printf ( "Cannot malloc StddevArray\n" );
			exit ( 1 );
		}
	}

	for ( int e = 0; e < TestCount; e++ )
	{
		for ( int r = 0; r < TrainCount; r++ )
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


		if ( KnnType == RESULTS_CLASSIFICATION )
		{
			fprintf ( ofp, "-- %d %s --\n", e, Test[e].ID );
			for ( int k = 0; k < K; k++ )
			{
				fprintf ( ofp, "   nearest %.4f %3d %s\n", 
						Test[e].Neighbors[k].Distance,
						Test[e].Neighbors[k].Index,
						Train[Test[e].Neighbors[k].Index].LabelC );
			}
		}
		else if ( KnnType == RESULTS_REGRESSION )
		{
			/*----------------------------------------------------------
				if -hilo used, find and eliminate min and max
			----------------------------------------------------------*/
			int		MinIndex = -1;
			int		MaxIndex = -1;
			double	MinValue = 0.0;
			double	MaxValue = 0.0;
			int		NumOut = K;

			if ( HiLo )
			{
				for ( int k = 0; k < K; k++ )
				{
					double	Value = Train[Test[e].Neighbors[k].Index].LabelR;
					if ( MinIndex == -1 || MinValue > Value )
					{
						MinValue = Value;
						MinIndex = k;
					}
					if ( MaxIndex == -1 || MaxValue < Value )
					{
						MaxValue = Value;
						MaxIndex = k;
					}
				}

				p = Test[e].Neighbors;
					
				if ( MaxIndex > MinIndex )
				{
					if ( MaxIndex + 1 < K )
					{
						memmove ( &p[MaxIndex], &p[MaxIndex+1], sizeof(NEIGHBOR) * (K-MaxIndex-1) );
					}
					if ( MinIndex + 1 < K )
					{
						memmove ( &p[MinIndex], &p[MinIndex+1], sizeof(NEIGHBOR) * (K-MinIndex-1) );
					}
				}
				else
				{
					if ( MinIndex + 1 < K )
					{
						memmove ( &p[MinIndex], &p[MinIndex+1], sizeof(NEIGHBOR) * (K-MinIndex-1) );
					}
					if ( MaxIndex + 1 < K )
					{
						memmove ( &p[MaxIndex], &p[MaxIndex+1], sizeof(NEIGHBOR) * (K-MaxIndex-1) );
					}
				}

				NumOut = K - 2;
			}

			if ( Verbose )
			{
				fprintf ( ofp, "-- %d %s --\n", e, Test[e].ID );
				double	Average  = 0;
				for ( int k = 0; k < NumOut; k++ )
				{
					Average += Train[Test[e].Neighbors[k].Index].LabelR;
					StddevArray[k] = Train[Test[e].Neighbors[k].Index].LabelR;

					fprintf ( ofp, "   nearest %.4f %3d %g\n", 
							Test[e].Neighbors[k].Distance,
							Test[e].Neighbors[k].Index,
							Train[Test[e].Neighbors[k].Index].LabelR );
				}
				fprintf ( ofp, "   average            %g\n",  Average / (double) NumOut  );
				fprintf ( ofp, "   stddev             %g\n",  stddev(StddevArray,NumOut) );
			}
			else
			{
				double	Average  = 0;
				for ( int k = 0; k < NumOut; k++ )
				{
					Average += Train[Test[e].Neighbors[k].Index].LabelR;
					StddevArray[k] = Train[Test[e].Neighbors[k].Index].LabelR;
				}
				Average = Average / (double) NumOut;

				if ( e == 0 )
				{
					fprintf ( ofp, "ID AVERAGE STDDEV STDPCT\n" );
				}

				double 	StdDev, StdPct;
				StdDev = stddev(StddevArray,NumOut);
				StdPct = 100.0 * StdDev / Average;

				fprintf ( ofp, "%s %g %g %.2f\n", 
							Test[e].ID,
							Average,
							StdDev,
							StdPct );
			}
		}
	}

	fclose ( ifp );
	fclose ( ofp );
	fclose ( pfp );
	if ( nsStrlen ( mfile ) > 0 )
	{
		fclose ( mfp );
	}


	if ( KnnType == RESULTS_REGRESSION )
	{
		free ( StddevArray );
	}

	if ( HasModel )
	{
		free ( MinMax );
	}
}
