/*
   Author: Matz Johansson

   News:
   disp is used in greater extent
   ,parse_config_file is safer [using MAX_ROW]
  */

#include "semaphores.h"
#include "lab2_standard.h"
#include "requestParser.h"
#include "http-server.h"

/*
  The server focuses on concurrency, establishing a connection and
  logging the connection.  The other header files: requestParser,
  takes care of a clients request and sends response back to a client.
  It also takes care of the zombies!
*/

void start_server(char *port, char *server_root, char *filename_alias)
{

  int wp; // waitpid temp var
  int num_children = 0;
  int keepalive = 0; // by default
  int server_sock;
  int client_sock;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  int value; // fork

  int date_max = strlen("Thu, 10 2025 22:53:58") + DELTA;
  char *date = CALLOC(char, date_max);

  semaphore mutex; // binary semaphore for locking of logfile

  FILE *log;
  int log_max = MAX_LINE * 7 + 15; /*
             date, IP, PID, keep-alive, get, connection, separator + some
             newlines
           */

  char *log_string = CALLOC(char, log_max); // 2 rules, 1 ip 1 pid (15) 1 date (22)
  char *reqs = MALLOC(char, MAX_LINE * 2 + DELTA);
  char *pid = MALLOC(char, 15); // 10 should be enough log10(2^32)
  time_t now;                   // date

  //////////////////////////////////////////////////////////////////////

  /*open socket*/
  if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    fprintf(stderr, "Cannot open socket\n");
    exit(-1);
  }

  // create server address
  memset(&server_addr, '\0', sizeof(struct sockaddr_in));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(port)); // the port we want to use
  server_addr.sin_addr.s_addr = INADDR_ANY;
  // convert a location to an IP address

  // bind or socket to the address
  if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
  {
    close(server_sock);
    fprintf(stderr, "Cannot bind socket!\n");
    disp(nameNline("start_server", __LINE__), "I'm a cop you idiot! I'm detective John Kimble!", "you better not irritate John Kimble, choose another port or try again later", extreme, STYLE);
  }

  // put socket into listen mode
  if (listen(server_sock, 10) == -1)
  {
    close(server_sock);
    fprintf(stderr, "Cannot put socket into listen mode!\n");
    exit(-1);
  }

  printf("\n");
  printf("Something went wrong\n");

  mutex = create_semaphore(1); // for locking the log

  while (TRUE) // the main server
  {
    socklen_t addr_length = sizeof(struct sockaddr_in);

    printf("Listens on port %d\n", atoi(port));

    // blocks until a new connection is accepted
    if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_length)) == -1)
    {
      close(server_sock);
      fprintf(stderr, "Cannot accept incoming connection!\n");
      exit(-1);
    }

    disp("start_server", "New connection established", "", did_you_know, STYLE);
    disp("start-server", "collecting the zombies", "", did_you_know, STYLE);

    /*
Everytime I call waitPid, an childprocess will be taken care of.
If the call to waitPid is -1, then something is wrong.
    */
    while (num_children > 0 && 0 < (wp = waitpid(-1, NULL, WNOHANG)))
      num_children--;

    if (wp == -1)
      disp(nameNline("start_server", __LINE__), "waitPid", "", extreme, STYLE);

    num_children++; // we create a new child for serving one client
    value = fork();

    if (value == 0) // A child-process serves a client
    {
      printf("Connection IP %s assigned PID %d\n", inet_ntoa(client_addr.sin_addr), (int)getpid());

      do // we parse a request, at least once
      {
        // memset(log_string, log_max, sizeof(char));//reset memory
        log_string = CALLOC(char, log_max);

        now = time(NULL);

        strftime(date, date_max, "%a, %d %Y %H:%M:%S", localtime(&now));
        disp("start-server", "date", date, did_you_know, STYLE);

        if (keepalive)
          sstrcat(log_string, "\n*keep alive*\n", log_max);

        sprintf(pid, "%d", (int)getpid());
        sstrcat(log_string, date, log_max);
        sstrcat(log_string, "\nIP: ", log_max);
        sstrcat(log_string, inet_ntoa(client_addr.sin_addr), log_max);

        sstrcat(log_string, "\nPID: ", log_max);
        sstrcat(log_string, pid, log_max);

        sstrcat(log_string, "\n", log_max);
        sstrcat(log_string, reqs = readLines(client_sock), log_max);
        sstrcat(log_string, "\n      ~~\n", log_max);

        p(mutex);
        {
          log = fopen("logg.log", "a");
          write2log(log, log_string);
          fclose(log);
        }
        v(mutex);

        parseRequest(client_sock, reqs, server_root, filename_alias, &keepalive);

        if (keepalive)
          disp("start-server", "keepalive!", "", did_you_know, STYLE);
        else
          disp("start-server", "no keepalive", "", did_you_know, STYLE);

      } while (keepalive);

      close(client_sock); // closing the socket after every finished request
      disp("start_server", "child is closing socket", nameNline("", (int)getpid()), did_you_know, STYLE);
      // printf("PID %d: child/closing socket!\n", (int)getpid());
      FREE3(log_string, reqs, date);
      break; // terminate in a normal fashion
    }
    else // parent
    {
      close(client_sock); // must close the parents copy of the socket
      printf("server/closing socka :-)\n");
    }

  } // while

  close(server_sock);
} // start_server

// 0.1 see also documentation for syntax
void parse_config_file(char *config_file, char *arg[])
{
  FILE *config = fopen(config_file, "r");

  char *row = MALLOC(char, MAX_ROW);  // temp storage
  char *word = MALLOC(char, MAX_ROW); // port|serverroot|alias
  char *val = MALLOC(char, MAX_ROW);  // value

  int found = 0;

  //////////////////////////////////////////////////////////////////////

  disp("parse_config_file", "open config file", config_file, did_you_know, STYLE);

  if (config == NULL)
    disp(nameNline("parse_config_file", __LINE__), "file for config does not exist", "", extreme, brief);

  // 1:a port, 2:a root 3:e alias file
  for (int i = 0; i < 3; i++)
    arg[i] = CALLOC(char, MAX_ROW);

  while (!feof(config))
  {
    fgets(row, MAX_ROW, config);

    if ((strlen(row) != 0) && (!beginsWith(row, "--"))) // comment
    {
      sscanf(row, "%s %s", word, val);

      if ((0 == strcmp(word, "port")) && (strlen(arg[0]) == 0))
      {
        disp("parse_config_file", "port applied", val, observe, STYLE);
        sstrcpy(arg[0], val, MAX_ROW);
        found++;
      }
      else if ((0 == strcmp(word, "serverroot")) && (strlen(arg[1]) == 0))
      {
        if (last(val) != '/')
          disp("parse_config_file", "checking serverroot", "make sure the path ends with /", extreme, STYLE);

        disp("parse_config_file", "serverroot applied", val, observe, STYLE);
        sstrcpy(arg[1], val, MAX_ROW);
        found++;
      }

      // the alias file
      else if ((0 == strcmp(word, "alias")) && (strlen(arg[2]) == 0))
      {
        disp("parse_config_file", "alias applied", val, observe, STYLE);
        sstrcpy(arg[2], val, MAX_ROW);
        found++;
      }
      if (found == 3) // no reason to look any further
        break;
    }
  }

  // checking if all arguments was found
  if (found != 3) // fatal!
  {
    if (strlen(arg[0]) == 0)
      disp("parse_config_file", "keyword \"port\" not found", "check config file, it must be a space between the two words", did_you_know, STYLE);

    if (strlen(arg[1]) == 0)
      disp("parse_config_file", "keyword \"serverroot\" not found", "check config file, it must be a space between the words", did_you_know, STYLE);

    if (strlen(arg[2]) == 0)
      disp("parse_config_file", "keyword \"alias\" not found", "check config file, it must be a space between the words", did_you_know, STYLE);

    disp(nameNline("parse_config_file", __LINE__), "something is wrong in config file, for this reason I have descided to exit", "", extreme, STYLE);
  }

  // printf("\n");
  free(row);
  fclose(config);
}

void write2log(FILE *file, char *str)
{
  if (file == NULL)
    disp("write2log", "the file used for log is corrupt or something", "check if semaphore usage is errornous e.g. unbalanced p and v operations, this should not occur!", extreme, STYLE);

  int index = 0;

  while (index < strlen(str))
    putc(str[index++], file);

  putc('\n', file);
}

int main(int argc, char *argv[])
{
  int f_flag = FALSE; // om -f
  char *args[3];
  FILE *alias;

  //////////////////////////////////////////////////////////////////////

  printf("\n");

  if (argc == 1 || (argc == 3 && 0 == strcmp(argv[1], "-f")))
  {
    f_flag = TRUE;

    for (int i = 0; i < 3; i++)
      args[i] = CALLOC(char, MAX_ROW);
  }

  if (argc == 1) // no arguments on prompt
  {
    parse_config_file("config", args);
    checkPort(args[0]);
    alias = fopen(args[2], "r");

    if (alias == NULL)
      disp(nameNline("main", __LINE__), "alias file not found", args[2], extreme, STYLE);

    printf("---\nport: %s\nserverroot: %s\nalias-file: %s\n---\n", args[0], args[1], args[2]);
    start_server(args[0], args[1], args[2]);
    return 0;
  }
  else if (argc == 3 && f_flag)
  {
    printf("//-f arg1 arg2\n");
    parse_config_file(argv[2], args);
    checkPort(args[0]);
    alias = fopen(args[2], "r");

    if (alias == NULL)
      disp(nameNline("main", __LINE__), "alias file not found", args[2], extreme, STYLE);

    printf("---\nport: %s\nserverroot: %s\nalias-file: %s\n---\n", args[0], args[1], args[2]);
    start_server(args[0], args[1], args[2]);
    return 0;
  }

  else if (argc == 3 && !f_flag)
  {
    printf("//no arguments\n");
    checkPort(argv[1]);
    printf("using port %s and serverroot %s\n", argv[1], argv[2]);

    printf("---\nport: %s\nserverroot: %s\nalias-file: none%s\n---\n", args[0], args[1], args[2]);

    // ingen alias?
    start_server(argv[1], argv[2], ""); // inga alias
    return 0;
  }
  else
  {
    printf("\n");
    printf("%s usage: [-f <config file> | <port> <serverroot>]", argv[0]);
    printf("\n");
  }

  return 0;
}

void checkPort(char *portNr)
{
  int port = atoi(portNr);

  // if NULL, cannot say just ==NULL gives a warning
  if ((int)port == 0)
    disp(nameNline("checkPort", __LINE__), "invalid port number", "use an integer for port", extreme, STYLE);

  if (port < 1024 || port > 32767) // enl. LANSA
    disp(nameNline("checkPort", __LINE__), "range out of bounds", "port <- [1024,32767]", extreme, STYLE);
}
