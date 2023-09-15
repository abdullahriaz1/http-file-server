#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include "httplibrary.h"
#define BUFSIZE 4096

int port_error() {
    char *error_message = "Invalid Port\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    return 1;
}

int main(int argc, char *argv[]) {
    //get port
    if (argv[1] == NULL) {
        return port_error();
    }
    int port = atoi(argv[1]);
    printf("Port %d\n", port);
    if ((argc != 2) || (argv[1] == NULL) || (port > 65536 || port < 1)) {
        return port_error();
    }

    //sock init
    Listener_Socket sock;
    int listening = listener_init(&sock, port);
    if (listening == -1) {
        return port_error();
    }
    printf("Socket fd = %d\n", sock.fd);

    //start server
    while (true) {
        //accept
        httpMessage message;
        message.status_code = 200;
        int listenfd = listener_accept(&sock);
        if (listenfd > 0) {
            read_request(listenfd, &message);
            parse_request(listenfd, &message);
            //send_response(listenfd, &message);
        }
        close(listenfd);
    }
    //end server
    return 0;
}
