#include "net.h"
#include "logger.h"
#include <stdlib.h>

net_client *make_net_client() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        log_error("Failed to init curl\n");
        exit(EXIT_FAILURE);
    }
    net_client *client = malloc(sizeof(net_client));
    if (client == NULL) {
        log_error("Failed to init net client\n");
        exit(EXIT_FAILURE);
    }
    client->curl = curl;
    return client;
}

void free_net_client(net_client *client) {
    curl_easy_cleanup(client->curl);
    free(client);
}

size_t _write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *fp = (FILE *)userdata;
    return fwrite(ptr, size, nmemb, fp);
}

void do_get_request(net_client *client, FILE *output_file, char *url) {

    curl_easy_setopt(client->curl, CURLOPT_URL, url);
    curl_easy_setopt(client->curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, _write_callback);
    curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, output_file);

    // Optional but good crawler defaults
    curl_easy_setopt(client->curl, CURLOPT_USERAGENT, "SimpleCrawler/1.0");
    curl_easy_setopt(client->curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(client->curl, CURLOPT_CONNECTTIMEOUT, 10L);

    CURLcode res = curl_easy_perform(client->curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform: %s\n", curl_easy_strerror(res));
    }
    fclose(output_file);
}
