#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "protocole.h"

int send_query(char *data, char *password, int query, int *len1){
    *len1 = 0;
	*len1 += sprintf(data+*len1, "version: %d\r\n", VERSION);
	*len1 += sprintf(data+*len1, "time: %lu\r\n", time(NULL));
	*len1 += sprintf(data+*len1, "password: %s\r\n", password);
	*len1 += sprintf(data+*len1, "query: %d", query);

	return 0;
}

int send_reponse(char *data, char *message, int query, int *len1){
    *len1 = 0;
	*len1 += sprintf(data+*len1, "version: %d\r\n", VERSION);
	*len1 += sprintf(data+*len1, "time: %lu\r\n", time(NULL));
	*len1 += sprintf(data+*len1, "query: %d\r\n", query);
	*len1 += sprintf(data+*len1, "message: %s", message);

	return 0;
}

int parse_query(char *data, envoi *envoi_data){
	int check;
	check = sscanf(data, "version: %d time: %lu password: %s query: %d", 
		&(envoi_data->version),
		&(envoi_data->time),
		&(envoi_data->password)[0],
        &(envoi_data->query)
		);

	if (check < 0){
        return 1;
    }
    
    return 0;
}

int parse_reponse(char *data, reponse *reponse_data){
	int check;
    check = sscanf(data, "version: %d time: %lu query: %d message: %s", 
		&(reponse_data->version),
		&(reponse_data->time),
		&(reponse_data->query),
		&(reponse_data->message)[0]
		);

    if (check < 0){
        return 1;
    }

	return 0;
}

int test1() {
    uint8_t buffer[500] = {0};
    int len;

    envoi yolo = {0}; 
    reponse yolo2 = {0}; 

    send_query((char*)&buffer, "loool", 1, &len);

    parse_query((char*)&buffer, &yolo); 

    printf("%d\n", yolo.version);
    printf("%lu\n", yolo.time);
    printf("%s\n", yolo.password);
    printf("%d\n", yolo.query);

    send_reponse((char*)&buffer, "loooazazdl", 1, &len);

    parse_reponse((char*)&buffer, &yolo2); 

    printf("%d\n", yolo2.version);
    printf("%lu\n", yolo2.time);
    printf("%d\n", yolo2.query);
    printf("%s\n", yolo2.message);

	return 0;
}
