
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<limits.h>
#include	<math.h>
#include	<assert.h>

#include	"shslib.h"

typedef struct
{
	double	*Columns;
} DATA;

#define		MAXLABEL	20
#define		MAXCOLS		24
/*----------------------------------------------------------
	training data has ID,inputs,label,newline
----------------------------------------------------------*/
#define		MAXTOKS		28

typedef struct
{
	int		Index;
	double	Distance;
} NEIGHBOR;

typedef struct
{
	char		ID[MAXLABEL];
	DATA		*Row;
	char		LabelC[MAXLABEL];
	double		LabelR;
	NEIGHBOR	*Neighbors;
} RECORD;

typedef struct
{
	double	Min;
	double	Max;
} MIN_MAX_RECORD;

#define		MODE_NORM		1
#define		MODE_TEST		2
#define		MODE_PREDICT	3
#define		RESULTS_CLASSIFICATION	'C'
#define		RESULTS_REGRESSION		'R'

#ifndef TYPE
#ifdef MAIN
#define		TYPE		/* */
#else
#define		TYPE		extern
#endif
#endif

TYPE	int		RunMode;
TYPE	int		KnnType;
TYPE	int		HiLo;
TYPE	char	*ifile;
TYPE	char	*ofile;
TYPE	char	*mfile;
TYPE	char	*pfile;

TYPE	RECORD	*Train;
TYPE	RECORD	*Test;
TYPE	int		TrainLines;
TYPE	int		TestLines;
TYPE	int		TrainCount;
TYPE	int		TestCount;
TYPE	int		ColumnCount;
TYPE	int		K;
TYPE	char	xbuffer[1024];
TYPE	char	*tokens[MAXTOKS];
TYPE	int		tokcnt;
TYPE	int		xt;
TYPE	int		xc;
TYPE	int		lineno;
TYPE	int		Verbose;
TYPE	int		Debug;

/*----------------------------
:.,$d
:r ! mkproto -p *.c
----------------------------*/

/* AppendTest.c */
void AppendTest ( char *tokens [], int tokcnt, int HasLabel );
void DeleteLastTest ( void );

/* AppendTrain.c */
void AppendTrain ( char *tokens [], int tokcnt, int HasLabel );

/* calc_dist.c */
double calc_dist ( double *From , double *To );

/* check_and_save.c */
void check_and_save ( int TestIndex , int TrainIndex , double Distance );

/* count_lines.c */
int count_lines ( FILE *fp );

/* getargs.c */
void getargs ( int argc , char **argv );

/* knn.c */
int main ( int argc , char **argv );

/* normalize.c */
void normalize ( void );

/* predict.c */
void predict ();

/* test.c */
void test ( void );
