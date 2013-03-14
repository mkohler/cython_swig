/* Private. Kept here for unit-testing only. */
#define ADDER_GREETING "Hello, "

typedef struct _PAIR {
    int x;
    int y;
} PAIR;

int add(int, int);
char * get_version(void);
char * greeting(char *);

int add_rs(int x, int y, int *sum);
int get_version_rs(char * output, int buflen);
int sgreeting(char * name, char * outp, int buflen);

int pair_add(PAIR * ppair);
