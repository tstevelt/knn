/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"shslib.h"

int GetTokensStd ( char xbuf[], char delim[], char *toks[], int maxtoks, int Verbose )
{
	int		rv = 0;
	int		xi, xl;

	xl = nsStrlen(delim);
	if ( xl == 0 )
	{
		return ( 0 );
	}

	xl = nsStrlen(xbuf);
	if ( xl == 0 )
	{
		return ( 0 );
	}

	if ( Verbose )
	{
		printf ( "%s<br>\n", xbuf );
	}

	toks[rv] = &xbuf[0];
	for ( xi = 0; xi < xl; xi++ )
	{
		if ( nsStrchr ( delim, xbuf[xi] ) != (char *)0 )
		{
			xbuf[xi] = '\0';

			if ( Verbose )
			{
				printf ( "%d %s<br>\n", rv, toks[rv] );
			}

			rv++;
			toks[rv] = &xbuf[xi+1];
		}
	}

	if ( Verbose )
	{
		printf ( "%d %s<br>\n", rv, toks[rv] );
	}

	rv++;

	return ( rv );
}

int GetTokensW ( char xbuf[], char *toks[], int maxtoks )
{
	int		rv = 0;
	int		xl, xi, xf;

	if ( maxtoks < 0 )
		return ( rv );

	xl = nsStrlen ( xbuf );
	xf = 1;
	for ( xi = 0; xi < xl; xi++ )
	{
		switch ( xbuf[xi] )
		{
			case ' ':
			case '\n':
			case '\r':
			case '\t':
			case '\b':	xbuf[xi] = '\0';
						if ( xf == 0 )
							xf = 1;
						break;

			default:	if ( xf )
						{
							xf = 0;
							toks[rv] = &xbuf[xi];
							rv++;

							if ( rv >= maxtoks )
								return ( rv );

						}
						break;
		}
	}

	return ( rv );
}

double stddev ( double Array[], int Count )
{
	double	Mean = 0.0;
	double	SumDiffSq = 0.0;
	double	rv;

	for (  int i = 0; i < Count; i++ )
	{
		Mean += Array[i];
	}
	Mean = Mean / Count;

	for ( int i = 0; i < Count; i++ )
	{
		SumDiffSq += pow(Array[i] - Mean,2);
	}

	rv = sqrt ( SumDiffSq / (double) Count );

	return ( rv );
}

void TrimRight ( char *b )
{
	int		bl;

	if ( b == (char *)0 )
	{
		return;
	}

	for ( bl = nsStrlen ( b ); bl; bl -- )
	{
		if ( (int) b[bl-1] <= 32 || (int) b[bl-1] > 126 )
		{
			b[bl-1] = '\0';
		}
		else
		{
			return;
		}
	}

    return;
}

int nsStrlen (const char *cpOne )
{
	if ( cpOne == (char *)0  )
		return ( 0 );

	return ( strlen( cpOne ) );
}

char *nsStrchr(const char *cpOne, int intChar )
{
	if ( cpOne == (char *)0  )
		return ( (char *)0 );

	return ( strchr(cpOne, intChar ) );
}
