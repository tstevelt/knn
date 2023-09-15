#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<time.h>
#include	<math.h>
#include	<errno.h>
#include	<openssl/sha.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

/* shslib.c */
int GetTokensStd ( char xbuf [], char delim [], char *toks [], int maxtoks , int Verbose );
int GetTokensW ( char xbuf [], char *toks [], int maxtoks );
double stddev ( double Array [], int Count );
void TrimRight ( char *b );
int nsStrlen ( const char *cpOne );
char *nsStrchr ( const char *cpOne , int intChar );
