#define BUFFER 500
#define VERSION 1

struct envoi 
{
    int version;
    unsigned long time;
    char password[20];
    int query;
};
typedef struct envoi envoi;

struct reponse
{
    int version;
    unsigned long time;
    int query;
    char message[20];
};
typedef struct reponse reponse;

int send_query(char *data, char *password, int query, int *len1);
int send_reponse(char *data, char *message, int query, int *len1);

int parse_query(char *data, envoi *envoi_data);
int parse_reponse(char *data, reponse *reponse_data);

int test1();
