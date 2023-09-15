/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"knn.h"

void normalize ()
{
	FILE	*ifp;
	FILE	*ofp;
	FILE	*mfp;
	MIN_MAX_RECORD		*MinMax;
	double	*Data;

	if (( ifp = fopen ( ifile, "r" )) == NULL )
	{
		printf ( "Cannot open input file %s\n", ifile );
		exit ( 1 );
	}
	if (( ofp = fopen ( ofile, "w" )) == NULL )
	{
		printf ( "Cannot create output file %s\n", ofile );
		exit ( 1 );
	}
	if (( mfp = fopen ( mfile, "w" )) == NULL )
	{
		printf ( "Cannot create model file %s\n", mfile );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		alloc space for model. don't know how manu lines, so use MAX 
	----------------------------------------------------------*/
	if (( MinMax = calloc ( sizeof(MIN_MAX_RECORD), MAXCOLS )) == NULL )
	{
		printf ( "Cannot calloc MinMax\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		count lines and get min and max of each column
	----------------------------------------------------------*/
	lineno = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), ifp ) != (char *)0 )
	{
		lineno++;

		tokcnt = GetTokensStd ( xbuffer, ",", tokens, MAXTOKS, 0 );
		if ( tokcnt < ColumnCount + 2 )
		{
			printf ( "Syntax error in line %d, tokcnt %d\n", lineno, tokcnt );
			continue;
		}

		for ( xt = 1, xc = 0; xt <= ColumnCount; xt++, xc++ )
		{
			double value = atof(tokens[xt]);

			if ( lineno == 1 )
			{
				MinMax[xc].Min = value;
				MinMax[xc].Max = value;
			}
			else
			{
				if ( MinMax[xc].Min > value )
				{
					MinMax[xc].Min = value;
				}
				if ( MinMax[xc].Max < value )
				{
					MinMax[xc].Max = value;
				}
			}
		}
	}

	/*----------------------------------------------------------
		alloc space for data 
	----------------------------------------------------------*/
	if (( Data = calloc ( sizeof(double), ColumnCount )) == NULL )
	{
		printf ( "Cannot calloc Data\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		read data. already have min max so normalize and write
	----------------------------------------------------------*/
	rewind ( ifp );
	lineno = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), ifp ) != (char *)0 )
	{
		lineno++;

		TrimRight ( xbuffer );

		tokcnt = GetTokensStd ( xbuffer, ",", tokens, MAXTOKS, 0 );
		if ( tokcnt < ColumnCount + 2 )
		{
			printf ( "Syntax error in line %d, tokcnt %d\n", lineno, tokcnt );
			continue;
		}

		fprintf ( ofp, "%s,", tokens[0] );
		for ( xt = 1, xc = 0; xt <= ColumnCount; xt++, xc++ )
		{
			double oval = atof(tokens[xt]);

			double nval = (oval - MinMax[xc].Min) / (MinMax[xc].Max - MinMax[xc].Min);
			
			fprintf ( ofp, "%s%.4f", xc ? "," : "", nval );
		}
		fprintf ( ofp, ",%s\n", tokens[xt] );
	}

	/*----------------------------------------------------------
		write model
	----------------------------------------------------------*/
	for ( xc = 0; xc < ColumnCount; xc++ )
	{
		fprintf ( mfp, "%6d %14.4f %14.4f\n", xc, MinMax[xc].Min, MinMax[xc].Max );
	}

	/*----------------------------------------------------------
		close files and exit
	----------------------------------------------------------*/
	fclose ( ifp );
	fclose ( ofp );
	fclose ( mfp );
	exit ( 0 );
}
