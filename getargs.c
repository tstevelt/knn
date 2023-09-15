/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"knn.h"

static void Usage ()
{
	printf ( "USAGE: knn -normalize -f input -o output             -m model  [options]\n" );
	printf ( "USAGE: knn -test      -f input                                 [options]\n" );
	printf ( "USAGE: knn -predict   -f input -o output -p predict [-m model] [options]\n" );
	printf ( "{-c|-r} classification (string) vs regression (numeric) (default is %c)\n", RESULTS_CLASSIFICATION );
	printf ( "-i #    input count (default %d)\n", ColumnCount );
	printf ( "-k #    neighbor count (default %d)\n", K );
	printf ( "-hilo   during regression predition, eliminate highest and lowest neighbor\n" );
	printf ( "-v      verbose\n" );
	printf ( "-d      debug\n" );
	exit ( 1 );
}

void getargs (int argc, char ** argv)
{
	if ( argc < 2 )
	{
		Usage ();
	}

	RunMode = 0;
	KnnType = RESULTS_CLASSIFICATION;
	HiLo = 0;
	Verbose = 0;

	for ( int xa = 1; xa < argc; xa++ )
	{
		if ( strcmp ( argv[xa], "-test" ) == 0 )
		{
			RunMode = MODE_TEST;
		}
		else if ( strcmp ( argv[xa], "-normalize" ) == 0 )
		{
			RunMode = MODE_NORM;
		}
		else if ( strcmp ( argv[xa], "-predict" ) == 0 )
		{
			RunMode = MODE_PREDICT;
		}
		else if ( strcmp ( argv[xa], "-c" ) == 0 )
		{
			KnnType = RESULTS_CLASSIFICATION;
		}
		else if ( strcmp ( argv[xa], "-r" ) == 0 )
		{
			KnnType = RESULTS_REGRESSION;
		}
		else if ( strcmp ( argv[xa], "-hilo" ) == 0 )
		{
			HiLo = 1;
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-f" ) == 0 )
		{
			xa++;
			ifile = argv[xa];
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-o" ) == 0 )
		{
			xa++;
			ofile = argv[xa];
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-p" ) == 0 )
		{
			xa++;
			pfile = argv[xa];
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-m" ) == 0 )
		{
			xa++;
			mfile = argv[xa];
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-k" ) == 0 )
		{
			xa++;
			K = atoi(argv[xa]);
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-i" ) == 0 )
		{
			xa++;
			ColumnCount = atoi(argv[xa]);
			if ( ColumnCount + 2 > MAXTOKS )
			{
				printf ( "Input Count %d exceeds MAXTOKS\n", ColumnCount );
				exit ( 1 );
			}
		}
		else if ( strcmp ( argv[xa], "-v" ) == 0 )
		{
			Verbose = 1;
		}
		else if ( strcmp ( argv[xa], "-d" ) == 0 )
		{
			Debug = 1;
		}
		else
		{
			printf ( "UNKNOWN ARG [%s]\n", argv[xa] );
			exit ( 1 );
		}
	}

	if ( RunMode == 0 )
	{
		Usage ();
	}
	
	if ( KnnType != RESULTS_REGRESSION && HiLo  )
	{
		printf ( "-hilo only applies to regression option (-r)\n" );
		exit ( 1 );
	}
	if ( KnnType == RESULTS_REGRESSION && HiLo && K <= 2 )
	{
		printf ( "K too small to use -hilo\n" );
		exit ( 1 );
	}


}
