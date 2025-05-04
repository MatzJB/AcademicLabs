
/*
   Author: Matz Johansson
   News:

   status_lines is smarter [added %*s to ignore a variable, when we do not need to parse it]
   replaceIt->replaceIt2 , returns NULL instead of "", this is better since Linux is picky on this
   ...also eraded the else part, this is actually uncessecary

   strNtok has been discontinued
   changed tail [using static since linux wasn't too happy with returning an empty string
*/

#include "lab2_standard.h"
#include "requestParser.h"

void parseRequest(int sock, char *reqs, char *root, char *filename_alias, int *keepalive)
{
  int response_max = MAX_LINE * 5;
  char *response = CALLOC(char, response_max); // see below
  char *filename = CALLOC(char, MAX_LINE);     // filename shouldn't be larger, really not larger than MAX_ROW
  int ka = *keepalive;

  //////////////////////////////////////////////////////////////////////

  sstrcat(filename, root, MAX_LINE); // filename begins with root

  sstrcat(response, status_line(reqs, &filename, filename_alias, &ka), response_max); // 1 row response
  // response 4 rows, last row 2 chars \r\n
  sstrcat(response, entity_headers(keepalive, filename), response_max);

  disp("parseRequest", "reponse", response, observe, STYLE);

  write(sock, response, strlen(response));
  disp("parseRequest", "resonse", "sent", observe, STYLE);

  sendFile(sock, filename);

  *keepalive = ka;

  FREE2(response, filename);
}

// read only two lines, throws away the rest
char *readLines(int sock)
{
  char *line = CALLOC(char, MAX_LINE); // latest change (in prevention of memory leak)
  char *rule1 = "GET";
  int r1 = FALSE; // keeps track of which rule, and only concats one for each rule
  char *rule2 = "Connection: ";
  int r2 = FALSE;
  int request_max = 2 * MAX_LINE; // max chars for request_bucket
  char *requests_bucket = CALLOC(char, request_max + DELTA);

  //////////////////////////////////////////////////////////////////////

  /*
    if a rule has been applied then no more strcat, we must be sure
    that no more than 2 lines is added (MAX_LINE*2+DELTA)
  */

  do
  {
    readLine(sock, line, MAX_LINE);

    if (beginsWith(line, rule1) && !r1)
    {
      disp("readLines", "fetched", line, observe, STYLE);
      r1 = TRUE;
      sstrcat(requests_bucket, line, request_max);
      sstrcat(requests_bucket, "\n", request_max); // add newline since we want to tell them apart
    }

    if (beginsWith(line, rule2) && !r2)
    {
      disp("readLines", "fetched", line, observe, STYLE);
      r2 = TRUE;
      sstrcat(requests_bucket, line, request_max);
    }
  } while (0 != strcmp(line, "")); // reads all lines

  // printf("Done reading the lines!\n");

  free(line);
  return requests_bucket;
}

/*
  NEW:leaves the first argument unharmed if not a string was found
  changes its argument is needed
*/
int replaceIt2(char *string, char *key, char *replacement, int buffer)
{
  if (buffer < 0)
    disp(nameNline("replaceit2", __LINE__), "buffer is negative", "please be reasonable", extreme, STYLE);

  // not ok
  if (strlen(key) == 0)
    return 0;

  int tar_max = strlen(string) + strlen(replacement);
  int left_max = strlen(string);
  char *ans = strstr(string, key); // returns a pointer to where the substring key begins in string
  char *tar = CALLOC(char, tar_max + DELTA);
  char *left = CALLOC(char, left_max + DELTA);

  sstrcpy(left, string, strlen(string) + DELTA); // chop where key begins

  // empty string doenst apply
  if (NULL == ans) // || (strlen(string)==0))//check this
    return 0;

  /*chops the string so the left of the resulting string is created*/
  left[strlen(string) - strlen(ans)] = '\0';
  sstrcat(tar, left, tar_max);
  sstrcat(tar, replacement, tar_max);       // append the substitution
  sstrcat(tar, ans + strlen(key), tar_max); // the rest of ans from (index strlen of key)
  sstrcpy(string, tar, buffer);
  FREE2(left, tar);
  return 1;
}

/*
  calculates the number of bytes a file contains, limited to a 4 GB
  large file
*/
unsigned long fileSize(FILE *file)
{
  unsigned long size = 0;
  int c;

  if (file == NULL)
    disp(nameNline("fileSize", __LINE__), "file doesn't exist", "", extreme, STYLE);

  while ((c = getc(file)) != EOF)
    size++;

  rewind(file); // reset pointer
  return size;
}

/*
  Send a file to socket, buffering the file is invoked to prevent
  memory failures.
*/

void sendFile(int socket, char *filename)
{
  FILE *file;
  int buffer_size = 10; // bytes in buffer
  char *buffer = MALLOC(char, buffer_size);

  unsigned long bytes_sent = 0; // current bytes
  unsigned long num_bytes;

  //////////////////////////////////////////////////////////////////////

  file = fopen(filename, "r");

  if (file == NULL)
    disp("sendFile", "file doesn't exist", filename, extreme, STYLE);

  if (buffer_size > 10)
    disp("sendFile", "buffer is probably too large, the sendfile isn't reliable", "", observe, STYLE);

  num_bytes = fileSize(file);

  if (num_bytes == 0)
  {
    free(buffer);
    disp("sendFile", "file to send is empty", "contact author", extreme, STYLE);
  }

  disp("sendFile", "writing to socket", "", did_you_know, STYLE);

  /*
    Memory failure control (if buffer set wisely ofcourse)
    Send if number of bytes is strict less than the byffer_size
  */

  do
  {
    if (num_bytes - bytes_sent < buffer_size) // leftovers
      buffer_size = num_bytes - bytes_sent;

    fread(buffer, sizeof(char), buffer_size, file);
    if (-1 == write(socket, buffer, buffer_size))
      disp(nameNline("sendFile", __LINE__), "write function error", "contact author", extreme, STYLE);

    bytes_sent += buffer_size;
    delay(100000);

    // if (bytes_sent<200)
    // printf("BUFF:%s\n", nameNline(buffer, -1));

    if (bytes_sent > num_bytes)
      disp("sendFile", "somthing is wrong with the algorithm", "", extreme, STYLE);

  } while (bytes_sent < num_bytes);

  //  printf("sent %d\n", (int)bytes_sent);

  buffer = CALLOC(char, 4);

  // send last two CRLF??
  sstrcat(buffer, CRLF, 4);
  sstrcat(buffer, CRLF, 4);
  write(socket, buffer, strlen(buffer));

  disp("sendFile", filename, "...done", did_you_know, STYLE);
  free(buffer);
}

// Tobias code
int readLine(int sock, char *buffer, int max_size)
{
  int bytes_read = 0;

  while (bytes_read < max_size)
  {
    /*-Try read one byte, return -1 upon error */
    if (read(sock, &buffer[bytes_read], sizeof(char)) != 1)
      return -1;

    /*did we get an newline? */
    /*if so, replace it with a NULL character and return */
    if (buffer[bytes_read] == '\n')
    {
      buffer[bytes_read] = '\0'; /*end the string */
      return bytes_read;
    }

    /*ignore '\r'' */

    if (buffer[bytes_read] != '\r')
      bytes_read++;
  }

  /*buffer full return -1 */
  return -1;
}

// a small HASKELL library
char *tail(char *x_xs)
{
  static char *empty = "";

  return (x_xs == NULL) ? empty : x_xs + min(1, strlen(x_xs)); // x_xs+ (if strlen==0 then 0 else 1)
}

// perhaps return (x_xs==NULL)?empty:x_xs+(1>=strlen(str))

char head(char *x_xs)
{
  if (x_xs != NULL)
    return x_xs[0];
  return '\0';
}

char last(char *x_xs)
{
  if (x_xs != NULL)
    return x_xs[max(strlen(x_xs) - 1, 0)];
  return '\0';
}

int max(signed int a, signed int b)
{
  return (a > b) ? a : b;
}

int min(signed int a, signed int b)
{
  return (a < b) ? a : b;
}

// Can make this neater by checking filename[i]==filename[i-1]
int is_repeating(char *filename, char c)
{
  int index = -1;
  int i;

  for (i = 0; i < strlen(filename); i++)
  {
    if (index > -1 && (filename[i] == c) && (i == index + 1))
      return 1;

    /*save the index where the first '.' was spotted*/
    if (filename[i] == c)
      index = i;
  }

  return 0;
}

char *entity_headers(int *keepalive, char *filename)
{
  int resp_max = MAX_LINE * 4;
  char *resp = CALLOC(char, resp_max + DELTA); // we add 4 rows at most
  char *fileSize_in_bytes = MALLOC(char, 11);  // log10(pow(2, 8*sizeof(uns)))
  // log10(ULONG_MAX)

  FILE *file;

  //////////////////////////////////////////////////////////////////////

  file = fopen(filename, "r");

  disp("entity-header", "filename", filename, did_you_know, STYLE);

  if (file == NULL)
    disp(nameNline("entity-header", __LINE__), "file is errornous", "check the requested filename", extreme, STYLE);

  if (keepalive)
  {
    sstrcat(resp, "Connection: Keep-Alive", resp_max);
    sstrcat(resp, CRLF, resp_max);
  }

  // content type:
  sstrcat(resp, "Content-Type: ", resp_max);
  sstrcat(resp, extract_mime(MIMES, filename), resp_max);
  sstrcat(resp, CRLF, resp_max);

  // content length:
  sprintf(fileSize_in_bytes, "%lu", fileSize(file));

  sstrcat(resp, "Content-Length: ", resp_max);
  sstrcat(resp, fileSize_in_bytes, resp_max);
  sstrcat(resp, CRLF, resp_max);

  sstrcat(resp, CRLF, resp_max);

  free(fileSize_in_bytes);
  return resp;
}

// 0.12
char *rev(char *g)
{
  char *ans = CALLOC(char, strlen(g) + DELTA);
  int r;

  for (r = 0; r < strlen(g); r++)
    ans[strlen(g) - r] = g[r];

  (r + 1)[ans] = '\0'; // kul va?
  return ans;
}

/*
  I thought I could assume that method and version was 1024, but if
  you really want to break the security you might just put in a
  larger method, that doesnt exist ofcourse, so therefore I chose to
  put MAX_LINE
*/

char *status_line(char *requests, char **file_, char *filename_alias, int *keepalive) //, FILE *file)
{

  //"reference" to file_, the file_ is to be built,
  char *file_ref = *file_; // MAX_LINE is alloced

  char *temp = MALLOC(char, MAX_LINE);
  char *part = MALLOC(char, MAX_LINE);
  char *method = MALLOC(char, MAX_LINE);
  char *abs_path = MALLOC(char, MAX_LINE);
  char *version = MALLOC(char, MAX_LINE); // HTTP-version

  // keep alive
  char *conn = MALLOC(char, MAX_LINE);

  FILE *file;

  //////////////////////////////////////////////////////////////////////

  // printf("parseRequest must parse %s\n", requests);

  sstrcpy(temp, requests, MAX_LINE); // since we dont want to destroy our argument

  part = strtok(temp, "\n");

  do
  {
    // errornous request
    if (beginsWith(part, " "))
    {
      disp("status-line", "poor syntax in beginning of requests", part, observe, STYLE);
      *file_ = error_file(INVALID_REQUEST);
      FREE6(temp, part, method, abs_path, version, conn);
      return build_response(INVALID_REQUEST);
    }

    if (beginsWith(part, "GET"))
      sscanf(part, "%s %s %s", method, abs_path, version);

    // could do it with substring, but...
    if (beginsWith(part, "Connection: "))
      sscanf(part, "%*s %s", conn);
  } while (NULL != (part = strtok(NULL, "\n")));

  disp("status-line", "method check", method, did_you_know, STYLE);

  if (0 != strcmp(method, "GET"))
  {
    *file_ = error_file(INVALID_REQUEST);
    FREE6(temp, part, method, abs_path, version, conn);
    return build_response(INVALID_REQUEST);
  }

  // only applicable if not using mozilla
  if (!beginsWith(abs_path, "/"))
  {
    *file_ = error_file(INVALID_REQUEST);
    FREE6(temp, part, method, abs_path, version, conn);
    return build_response(INVALID_REQUEST);
  }

  disp("status-line", "version check", version, did_you_know, STYLE);

  if (0 != strcmp(version, "HTTP/1.0") && (0 != strcmp(version, "HTTP/1.1")))
  {
    *file_ = error_file(INVALID_REQUEST);
    FREE6(temp, part, method, abs_path, version, conn);
    return build_response(INVALID_REQUEST);
  }

  disp("status-line", "abs_path", abs_path, did_you_know, STYLE);
  sstrcat(file_ref, tail(abs_path), MAX_ROW);
  printf("file_ref=%s\n", file_ref);

  // if / at the end append index.html
  if (last(abs_path) == '/') // 0==strcmp(abs_path, "/"))
  {
    disp("status-line", "appending index", "", did_you_know, STYLE);
    sstrcat(file_ref, "index.html", MAX_LINE);
  }

  // alias
  if (0 == strcmp(filename_alias, ""))
    disp("status-line", "No alias file specified", "check config file \"alias\"", observe, STYLE);
  else if (siff_aliases(filename_alias, file_ref))
    disp("status-line", "alias found!", file_ref, did_you_know, STYLE);
  else
    disp("status-line", "no alias found", file_ref, did_you_know, STYLE);

  /*
    Well, the alias file itself probably doesnt create extra ".." but
    you never know...
  */

  disp("status-line", "security check", "", did_you_know, STYLE);

  // security, before file existance check
  if (is_repeating(file_ref, '.'))
  {
    *file_ = error_file(FORBIDDEN);
    FREE6(temp, part, method, abs_path, version, conn);
    return build_response(FORBIDDEN);
  }

  disp("status-line", "filename test", file_ref, did_you_know, STYLE);

  file = fopen(file_ref, "r");

  // file not found or not a filename just a / at the end
  if (file == NULL || last(file_ref) == '/')
  {
    disp("status_line", "file not found", file_ref, observe, STYLE);
    *file_ = error_file(FILE_NOT_FOUND);
    FREE6(temp, part, method, abs_path, version, conn);
    return build_response(FILE_NOT_FOUND);
  }

  fclose(file); // test done

  //*keepalive = ((0==strcmp(version,"HTTP/1.0")) && (0==strcmp(conn, "keep-alive"))) || ((0==strcmp(version,"HTTP/1.1")) && (0!=strcmp(conn, "close")));

  // keep alive connection
  if ((0 == strcmp(version, "HTTP/1.0")) && (0 == strcmp(conn, "keep-alive")))
  {
    disp("status_line", "Keep alive first version", "", observe, STYLE);
    *keepalive = TRUE;
  }
  if ((0 == strcmp(version, "HTTP/1.1")) && (0 != strcmp(conn, "close")))
  {
    disp("status_line", "Keep alive second version", "", observe, STYLE);
    *keepalive = TRUE;
  }

  disp("status_line", "OK", "", did_you_know, STYLE);
  FREE6(temp, part, method, abs_path, version, conn);

  return build_response(OK);
}

// build a response (status line) from macros
char *build_response(char *code_and_phrase)
{
  char *response = CALLOC(char, MAX_LINE);

  sstrcat(response, HTTP_VERSION, MAX_LINE);
  sstrcat(response, SP, MAX_LINE);
  sstrcat(response, code_and_phrase, MAX_LINE);
  sstrcat(response, CRLF, MAX_LINE);

  return response;
}

// builds the error file from a statuscode
char *error_file(char *code_and_phrase)
{
  char *file = CALLOC(char, MAX_LINE);

  if (beginsWith(code_and_phrase, "200"))
    disp(nameNline("error_file", __LINE__), "this code_and_phrase is not to be used here", "", extreme, STYLE);

  sstrcat(file, ERROR_ROOT, MAX_LINE);

  if (beginsWith(code_and_phrase, "400"))
    sstrcat(file, "Invalid_request.html", MAX_LINE);
  else if (beginsWith(code_and_phrase, "403"))
    sstrcat(file, "Forbidden.html", MAX_LINE);
  else if (beginsWith(code_and_phrase, "404"))
    sstrcat(file, "File_not_found.html", MAX_LINE);
  else
    disp(nameNline("error_file", __LINE__), "code does not exist in this version", "contact author", extreme, STYLE);

  return file;
}

// eventuellt anvnds strstr och kolla om den returnerar 1 annars
int beginsWith(char *str, char *substr)
{
  int i;

  if ((substr != NULL) && (str != NULL) && (strlen(substr) <= strlen(str)))
  {
    for (i = 0; i < strlen(substr); i++)
      if (str[i] != substr[i])
        return 0;

    return 1;
  }

  return 0;
}

char *extract_mime(char *mimetypes, char *filename)
{
  char *suffix_cand = MALLOC(char, MAX_LINE);
  char *mime_cand = MALLOC(char, MAX_LINE);
  char *str = MALLOC(char, MAX_LINE);

  FILE *in = fopen(mimetypes, "r");
  char *the_suffix = MALLOC(char, strlen(filename) + DELTA);

  int index, c;

  //////////////////////////////////////////////////////////////////////

  index = c = 0;

  if (in == NULL)
    disp("extract_mime", "cannot find mime-file", filename, did_you_know, STYLE);

  sstrcpy(the_suffix, extract_suffix(filename), strlen(filename));

  do
  {
    while (((c = fgetc(in)) != EOF) && (c != '\n'))
      str[index++] = c;

    sscanf(str, "%s %s", suffix_cand, mime_cand);

    if (0 == strcmp(suffix_cand, the_suffix))
    {
      disp("extract_mime", "found the suffix", the_suffix, did_you_know, STYLE);
      return mime_cand; /*return the mime*/
    }

    index = 0;
    str = CALLOC(char, MAX_LINE);
  } while (c != EOF);

  return "application/octet-stream"; // if no appropriate mime-type was found
}

int siff_aliases(char *alias_file, char *file)
{

  FILE *alias = fopen(alias_file, "r");
  char *row = MALLOC(char, MAX_ROW);
  char *key_cand = MALLOC(char, MAX_ROW);
  char *replace_cand = MALLOC(char, MAX_ROW);

  //////////////////////////////////////////////////////////////////////

  if (alias == NULL)
    disp(nameNline("siff_aliases", __LINE__), "file not found", file, extreme, STYLE);

  while (!feof(alias))
  {
    fgets(row, MAX_ROW, alias); // prevent overflow

    if ((0 != strcmp(row, "")) && (!beginsWith(row, "--"))) // if not comment or empty row
    {
      disp("siff_aliases", row, "", did_you_know, STYLE);
      sscanf(row, "%s %s", key_cand, replace_cand); //(key_cand+replace_cand)<=MAX_ROW

      // check that key_cand is !=""  and replace_cand is !=""
      if (replaceIt2(file, key_cand, replace_cand, MAX_ROW))
      {
        FREE3(row, key_cand, replace_cand);
        fclose(alias);
        return 1; // hittade ett alias
      }
    }
  }

  FREE3(row, key_cand, replace_cand);

  fclose(alias);
  return 0;
}

/*
  Go through the filename from last character and return the pointer
  to next to the where the . was found
*/

char *extract_suffix(char *filename)
{
  int dot_i;
  for (dot_i = strlen(filename); dot_i >= 0; dot_i--) // from next to last
    if (filename[dot_i] == '.')
      return filename + (dot_i + 1);
  return "";
}
