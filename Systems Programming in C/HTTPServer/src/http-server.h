
/*Matz Johansson B 2005*/

void start_server(char *port, char *serverroot, char *filename_alias);
void parse_config_file(char *str, char *arg[]);
void write2log(FILE *file, char *str);
// void stopit();

void checkPort(char *port);
