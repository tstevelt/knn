/*----------------------------------------------------------------------------
	Program : knn.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Sep 2023
	Synopsis: KNN classification and regression.
	Return  : 

	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		09/12/2023	Added ID column (first column) to all data files.

----------------------------------------------------------------------------*/

#define		MAIN

#include	"knn.h"

int main (int argc, char ** argv)
{
	Train = NULL;
	Test = NULL;
	TrainLines = 0;
	TestLines = 0;
	ColumnCount = 4;
	ifile = ofile = mfile = pfile = NULL;
	K = 5;

	getargs ( argc, argv );

	switch ( RunMode )
	{
		case MODE_NORM:
			if ( ifile == NULL || ofile == NULL || mfile == NULL )
			{
				printf ( "Input, Output and Model names are required to normalize\n" );
				exit ( 1 );
			}
			normalize ();
			break;

		case MODE_TEST:
			if ( ifile == NULL || ofile == NULL )
			{
				printf ( "Input and  Output names are required for testing\n" );
				exit ( 1 );
			}
			test ();
			break;

		case MODE_PREDICT:
			if ( ifile == NULL || ofile == NULL || pfile == NULL )
			{
				printf ( "Input, Output and Predict file names are required\n" );
				printf ( "Use model file if predict file is not normalized\n" );
				exit ( 1 );
			}
			predict ();
			exit ( 1 );
	}

    return (0);
}
