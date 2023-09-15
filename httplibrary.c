#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include "httplibrary.h"

//validate the uri
bool check_uri(char uri[]) {
    int length = strlen(uri);
    printf("uri length is %d\n", length);
    if (length < 2 || length > 64) {
        printf("URI too long or too short\n");
        return false;
    }
    if (uri[0] != '/') {
        return false;
    }
    for (int i = 1; i < length; i++) {
        if (!(isalpha(uri[i]) || isdigit(uri[i]) || uri[i] == '.' || uri[i] == '-')) {
            printf("URI not alphabetic or numeric or .-\n");
            return false;
        }
    }
    return true;
}
//Read client input into buf,find request line, store vals in httpMessage, check for carriage return and newline, set status code
void read_request(int listenfd, httpMessage *message) {
    int bytes_read = read(listenfd, message->buf, BUFFER_SIZE);
    if (bytes_read == -1) {
        printf("read error\n");
    } else {
        message->buf[bytes_read] = 0;
        printf("Received the following input: \n%s\n", message->buf);
        int store_vals
            = sscanf(message->buf, "%s %s %s", message->method, message->uri, message->version);
        if (store_vals != 3) {
            printf("Error storing request in httpMessage");
            return;
        }
        //check uri
        printf(
            "Method: %s, URI: %s, Version: %s\n", message->method, message->uri, message->version);
        if (check_uri(message->uri)) {
            printf("Valid URI\n");
        } else {
            printf("Invalid URI\n");
            message->status_code = 404;
        }
        //check version
        if (strcmp(message->version, "HTTP/1.1")) {
            printf("Invalid HTTP Version\n");
            message->status_code = 505;
        }
    }
}

//For PUT request, use content length to determine how many bytes we need to read from httpMessage and loop through the message
void handle_input() {
}

//For GET request, loop through file and send to client
void send_file(int listenfd, int fd, httpMessage *message) {
    message->content_length = listenfd;
    int bytes_read;
    int filelength = lseek(fd, 0, SEEK_END);
    message->content_length = filelength;
    if (lseek(fd, 0, SEEK_SET) != 0) {
        printf("bad seek\n");
    }
    message->status_code = 200;
    sprintf(message->buf, "%s %d OK\r\nContent-Length: %d\r\n\r\n", message->version,
        message->status_code, message->content_length);
    int x = strlen(message->buf);
    int y = write(listenfd, message->buf, x);
    if (y == -1) {
        printf("y == -1\n");
    }
    while ((bytes_read = read(fd, message->buf, BUFFER_SIZE)) > 0) {
        int bytes_written = write(listenfd, message->buf, bytes_read);
        if (bytes_written != bytes_read) {
            close(fd);
            printf("write error\n");
            return;
        }
    }
    if (bytes_read == -1) {
        close(fd);
        printf("read error in send_file\n");
        return;
    }
    return;
}
//Check if get or put
void parse_request(int listenfd, httpMessage *message) {
    printf("parse_request\n");
    message->content_length = listenfd;
    if (!strcmp(message->method, "PUT")) {
        //handle_input();
    } else if (!strcmp(message->method, "GET")) {
        char *uriline = message->uri;
        uriline++;
        int fd = open(uriline, O_RDONLY);
        if (fd == -1) {
            printf("Error opening file for GET\n");
            return;
        }

        send_file(listenfd, fd, message);
        close(fd);
    } else {
        printf("not put or get\n");
        message->status_code = 501;
    }
    return;
}

//Respond with status. For GET with status 200 or 201, invoke send_file.
void send_response(int listenfd, httpMessage *message) {
    message->content_length = listenfd;
    if (strcmp(message->method, "PUT")) {
        handle_input();
    } else if (strcmp(message->method, "GET")) {
        *message->uri += 1;
        printf("%s\n", message->uri);
    } else {
    }
    return;
}
