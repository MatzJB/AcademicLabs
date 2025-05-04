/*
  Author: Matz Johansson
  News:
       added macros:MAX_ROW, DELTA,
       added functions:sstrcat [a secure strcat].

       changed definitions of CALLOC/MALLOC [they where swapped which
       made things go crazy]
*/

#ifndef _LAB2_STANDARDH_
#define _LAB2_STANDARDH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*server*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <math.h>
#include <time.h>
#include <limits.h>

/*processes*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

//////////////////////////////////////////////////////////////////////

#define STYLE brief // verbose is ok, messagedetail used by disp

/*
  request bounds
  max line=maximum number of characters on a line
  max lines= maximum number of lines
*/

#define MAX_LINE 10240
#define MAX_LINES 100
#define MAX_ROW 250 // OBS 250 �r lagom

#define DELTA 10
// #define  MAX_PRI      70

#define SERVER_ROOT "./myroot/" // roten f�r requests
#define ERROR_ROOT "./errors/"  // fel filer
#define MIMES "mime_types.config"

// responses
#define HTTP_VERSION "HTTP/1.1" // versionen som skickas till browser
#define CRLF "\r\n"
#define SP " "

// bara definierar kod med tillh�rande fras
#define OK "200 OK"
#define INVALID_REQUEST "400 Invalid request"
#define FORBIDDEN "403 Forbidden"
#define FILE_NOT_FOUND "404 File not found"

#define TRUE !FALSE
#define FALSE 0

// FREE##n
#define FREE2(v1, v2) \
  free(v1);           \
  free(v2)
#define FREE3(v1, v2, v3) \
  FREE2(v1, v2);          \
  free(v3)
#define FREE4(v1, v2, v3, v4) \
  FREE2(v1, v2);              \
  FREE2(v3, v4)
#define FREE5(v1, v2, v3, v4, v5) \
  FREE2(v1, v2);                  \
  FREE3(v3, v4, v5)
#define FREE6(v1, v2, v3, v4, v5, v6) \
  FREE3(v1, v2, v3);                  \
  FREE3(v4, v5, v6)

// allocs
#define CALLOC(X, Y) (X *)xalloc(calloc(Y, sizeof(X)))
#define MALLOC(X, Y) (X *)xalloc(malloc(Y * sizeof(X)))

//////////////////////////////////////////////////////////////////////
// stromdahl isnt yet implemented
typedef enum
{
  nothing = 0,
  gedell,
  carlsson,
  stromdahl,
  brief,
  verbose,
  all
} MessageStyle;
typedef enum
{
  did_you_know = 0,
  observe,
  warning,
  extreme
} MessageType;

//////////////////////////////////////////////////////////////////////

void disp(char *where, char *what, char *extra, MessageType caution, MessageStyle style);
char *nameNline(char *function, int line);

void sstrcat(char *a, char *b, int buffer);
void sstrcpy(char *a, char *b, int buffer);

void delay(int a);

/*memory handling*/
const void *xalloc(const void *a);

#endif
