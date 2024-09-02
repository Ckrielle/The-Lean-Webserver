#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

#include "lean.h"

#define BUFFER_SIZE 1024
#define DATA_SIZE 8192

void create_http_response_headers(char *resp, char *version, int status_code, char* status_msg, int content_length, char *content_type)
{
    sprintf(resp, "%s %d %s\r\n", version, status_code, status_msg);
    sprintf(resp, "%sServer: Lean Web Server\r\n", resp);
    sprintf(resp, "%sConnection: close\r\n", resp);
    sprintf(resp, "%sContent-length: %d\r\n", resp, content_length);
    sprintf(resp, "%sContent-type: %s\r\n\r\n", resp, content_type);
}

void client_error(int cfd, char *version, char *error_cause, int error_code, char *error_msg, char *error_description)
{
    char html[DATA_SIZE], response[BUFFER_SIZE];

    sprintf(html, "<html><title>Lean Error</title>");
    sprintf(html, "%s<body><h1>%d: %s</h1>\r\n", html, error_code, error_msg);
    sprintf(html, "%s<p>%s: %s</p></html>\r\n", html, error_description, error_cause);

    create_http_response_headers(response, version, error_code, error_msg, strlen(html), "text/html");

    send(cfd, response, strlen(response), 0);
    send(cfd, html, strlen(html), 0);
}

void parse_request(char *request, char *method, char *path, char *version)
{
    sscanf(request, "%s %s %s", method, path, version);
}

void parse_path(char *path, char *fname)
{
    strcpy(fname, ".");
    strcat(fname, path);
    if (path[strlen(path)-1] == '/')
        strcat(fname, "index.html");
}

void get_filetype(char *fname, char *ftype)
{
    if (strstr(fname, ".html"))
	    strcpy(ftype, "text/html");
    else if (strstr(fname, ".gif"))
	    strcpy(ftype, "image/gif");
    else if (strstr(fname, ".png"))
        strcpy(ftype, "image/png");
    else if (strstr(fname, ".jpg"))
	    strcpy(ftype, "image/jpeg");
    else
	    strcpy(ftype, "text/plain");
}

void send_response(int cfd, char *version, char *filename)
{
    char response[BUFFER_SIZE], data[DATA_SIZE];

    int file_fd = open(filename, O_RDONLY);
    if (file_fd < 0)
    {
        client_error(cfd, version, &filename[1], 404, "Not found", "Lean couldn't find this file");
        return;
    }
    else
    {
        int filesize = 0, bytes_read = 0;
        while ((bytes_read = read(file_fd, data, DATA_SIZE)) > 0)
            filesize += bytes_read;
        data[filesize] = 0;

        char filetype[BUFFER_SIZE];
        get_filetype(filename, filetype);
        
        create_http_response_headers(response, version, 200, "OK", strlen(data), filetype);

        printf("Response:\n%s", response);
        
        send(cfd, response, strlen(response), 0);
        send(cfd, data, filesize, 0);

        close(file_fd);
    }
}

void handle_request(int client_fd)
{
    char request[BUFFER_SIZE];
    int bytes_read = recv(client_fd, request, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0)
    {
        perror("Error reading request:");
        return;
    }
    request[bytes_read] = '\0';
    printf("Request:\n%s", request);

    char method[BUFFER_SIZE], path[BUFFER_SIZE], version[BUFFER_SIZE];
    parse_request(request, method, path, version);

    if (strncmp(method, "GET", 4) != 0)
    {
        client_error(client_fd, version, method, 501, "Not Implemented", "Lean doesn't support this method");
        return;
    }

    char filename[BUFFER_SIZE];
    parse_path(path, filename);
    send_response(client_fd, version, filename);    
}