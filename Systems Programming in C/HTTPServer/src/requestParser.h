#include "lab2_standard.h"

// read all lines from a client and puts them together with newline as a separator

/*
  Only returns the important ones, in future versions perhaps a linked
  list should contain the words the readLine should return upon match
*/

char *readLines(int sock);
int readLine(int sock, char *buffer, int max_size);
void sendFile(int socket, char *filename);

/*
  Uses entity_headers and status_line to parse and build a response to later send a file
*/

void parseRequest(int sock, char *reqs, char *root, char *alias_file, int *keepalive);

char *status_line(char *requests, char **filename, char *alias, int *keepalive);
char *build_response(char *code_and_phrase);
char *error_file(char *code_and_phrase);
int siff_aliases(char *alias_file, char *file);

char *replaceIt(char *string, char *key, char *replacement);
int replaceIt2(char *string, char *key, char *replacement, int buffer);

// for mime-types
// char *extract_suffix(char * filename);
char *extract_mime(char *mimes_file, char *filename);
char *extract_suffix(char *filename);

char *entity_headers(int *keeaplive, char *filename);

int is_repeating(char *filename, char c);
unsigned long fileSize(FILE *file);

//////////////////////////////////////////////////////////////////////

// haskell inspired
char *rev(char *);
char *tail(char *);
char *tail2(char *);
char head(char *);
char last(char *);

// miscellaneous
int beginsWith(char *string, char *substring);

int max(signed int, signed int);
int min(signed int, signed int);

/*
  counts the number of tokens created by delimit b (OBS: in this
  version b is only a char)
*/
int strNtok(char *a, char *b);
