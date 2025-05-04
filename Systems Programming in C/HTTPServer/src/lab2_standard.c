#include "lab2_standard.h"

/*
  Future version will actually just take the line and c-file name and
  then from them search which function that generated the error...

  The function trucate a line that is longer than 70 chars
*/

char *nameNline(char *function, int line)
{
  char *info = CALLOC(char, 70);
  char *temp = MALLOC(char, 61);
  char *lineNr = MALLOC(char, 10);

  if (line >= 0) // if line <0 then no line will be appended
  {
    sprintf(lineNr, "%d", line);
    sstrcat(info, " ", 70);    // 10 chars maximum for line number
    sstrcat(info, lineNr, 70); // 10 chars maximum for line number
    sstrcat(info, " ", 70);
  }

  if (strlen(function) > 70 - strlen(info))
  {
    function[70 - strlen(info) - 3] = '\0'; // for the "..."

    sstrcat(info, function, 70);
    sstrcat(info, "...", 70);
  }
  else
  {
    sstrcat(info, function, 70);
  }

  FREE2(temp, lineNr);

  return info;
}

// truncate to long lines if brief
void disp(char *where, char *what, char *extra, MessageType type, MessageStyle style)
{

  char *types[] = {"did you know", "OBS", "FATAL"};

  if (style == all)
  {

    if (type == extreme) // for all extremely important messages, stop execution.
      printf("ERROR->");

    if (strcmp(extra, "") == 0)
      printf("(%s) [%s] - %s\n", types[type], where, what);
    else
      printf("(%s) [%s] - %s  ->{%s}\n", types[type], where, what, extra);
  }
  else if (style == verbose) // prints all messages without comment "did you know" or so
  {
    if (type == extreme) // for all extremely important messages, stop execution.
      printf("ERROR");

    // if (type>=observe)
    if (strcmp(extra, "") == 0)
      printf("%s -> %s\n", where, what);
    else
      printf("%s -> %s {%s}\n", where, what, extra);
  }
  else if (style == brief) // only prints with type larger than did_you_know
  {

    if (type >= observe)
      if (style == brief)
        printf("[%s] - %s \n", where, what);
  }
  else if (style == nothing) // prints nothing
  {
    // nothing
  }
  else if (style == carlsson) // Tobias Carlsson
  {
    if (type >= observe)
    {
      if (strcmp(extra, "") == 0)
        printf("\nFunction: %s\nWhat: %s\n", where, what);
      else
        printf("\nFunction: %s\nWhat: %s\nExtra: %s\n", where, what, extra);

      if (type == extreme) // for all extremely important messages, stop execution.
        printf("Error:An error occured, exiting\n");
    }
  }

  else if (style == gedell) // prints a style, resembling Tobias Gedell, but no did_you_knows
  {
    if (type == extreme) // for all extremely important messages, stop execution.
      printf("\nKonstigt fel nedan\n");

    if (type > observe)
    {
      if (strcmp(extra, "") == 0)
        printf("<<<\n%s-> %s\n>>>\n", where, what);
      else
        printf("<<<\n%s \n %s, %s\n>>>\n", where, what, extra);
    }
  }

  else if (style == stromdahl) // prints a style, resembling of Per Str�mdahl
  {
    if (type == extreme) // for all extremely important messages, stop execution.
      printf("\n�hhhh men...");

    if (type >= observe)
    {

      // really should put a random triggerer here...
      if (strcmp(extra, "") == 0)
        printf("\"%s\"(var) f�r att \"%s\"(vad)\n\n", where, what);
      else
        printf("Allts� i \"%s\" f�r att \"%s\"(var) --> %s(vad)\n", where, what, extra);
    }
  }

  fflush(stdout);

  // effects

  if (type == extreme) // for all extremely important messages, stop execution.
    exit(-1);
}

/*
  This function is used when sending a file. The write isn't good for
  reliable transfer, to make it somewhat more reliable, you will have
  to do this.
*/
void delay(int a)
{
  while (a--)
    ;
}

// this version check if the two arguments exceeds some known maximum
// limit, if it does then exit
void sstrcat(char *a, char *b, int buffer)
{
  if (strlen(a) + strlen(b) > buffer)
    disp("sstrcat", "buffer overflow", "make buffer bigger!", extreme, STYLE);

  strcat(a, b);
}

void sstrcpy(char *a, char *b, int buffer)
{
  if (strlen(a) + strlen(b) > buffer)
    disp("sstrcat", "buffer overflow", "make buffer bigger!", extreme, STYLE);

  strcpy(a, b);
}

const void *xalloc(const void *a)
{
  if (a == NULL)
    disp("xalloc", "xalloc error", "free RAM to provide more mem", extreme, STYLE);
  return a;
}
