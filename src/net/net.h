#ifndef NET_H
#define NET_H

#include "logger.h"
#include "utils.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READ_BUFFER_SIZE 4096

typedef struct {
    CURL *curl;
} net_client;

net_client *make_net_client();
void do_get_request(net_client *client, FILE *output_file, char *url);
void free_net_client(net_client *client);

#endif
