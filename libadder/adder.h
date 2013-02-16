#define ADDER_VERSION "v1.0"

int add(int, int);
char * get_version(void);
char * make_greeting(char *name);

int add_sr(int x, int y, int *sum);
int get_version_sr(char * output, int buflen);
int make_greeting_sr(char * name, char * output, int buflen);
