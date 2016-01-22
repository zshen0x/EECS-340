#include "minet_socket.h"
#include <stdlib.h>
#include <ctype.h>
#include <string>

#define BUFSIZE 1024

int write_n_bytes(int fd, char * buf, int count);

int main(int argc, char * argv[]) {
    char * server_name = NULL;
    int server_port = 0;
    char * server_path = NULL;

    int sock = 0;
    int rc = -1;
    int datalen = 0;
    bool ok = true;
    struct sockaddr_in sa;
    FILE * wheretoprint = stdout;
    struct hostent * site = NULL;
    char * req = NULL;

    char buf[BUFSIZE + 1];
    char * bptr = NULL;
    char * bptr2 = NULL;
    char * endheaders = NULL;

    struct timeval timeout;
    fd_set set;

    /*parse args */
    if (argc != 5) {
	    fprintf(stderr, "usage: http_client k|u server port path\n");
    	exit(-1);
    }

    server_name = argv[2];
    server_port = atoi(argv[3]);
    server_path = argv[4];

    /* initialize minet */
    if (toupper(*(argv[1])) == 'K') {
	    minet_init(MINET_KERNEL);
    } else if (toupper(*(argv[1])) == 'U') {
	    minet_init(MINET_USER);
    } else {
	    fprintf(stderr, "First argument must be k or u\n");
	    exit(-1);
    }

    // TODO use minet_perror() minet_error()
    /* create socket */
    sock = minet_socket(SOCK_STREAM);
    if (sock == -1) {
        minet_perror("failed to create a socket\n");
        exit(-1);
    }
    // Do DNS lookup
    /* Hint: use gethostbyname() */
    site = gethostbyname(server_name);
    if (site == NULL) {
        minet_perror("unable to resolve host name\n");
        exit(-1);
    }
    /* set address */
    sa.sin_family = AF_INET;
    sa.sin_addr = *(struct in_addr *) site->h_addr;
    sa.sin_port = htons(server_port);
    /* connect socket */
    if (minet_connect(sock, &sa) == -1) {
        minet_perror("failed to make a onnection\n");
        exit(-1);
    }

    /* send request */
    const std::string CRLF("\r\n");
    std::string requestMsg = "hello tcp";
    std::string requestLine;
    std::string requestHeaders;
    std::string body;
    requestMsg
        .append(requestLine)
        .append(requestHeaders)
        .append(CRLF)
        .append(body);
    char requestMsg_array[BUFSIZE];
    strcpy(requestMsg_array, requestMsg.c_str());
    write_n_bytes(sock, requestMsg_array, requestMsg.size());

    minet_close(sock);

    return 0;

    /* wait till socket can be read */
    /* Hint: use select(), and ignore timeout for now. */


    /* first read loop -- read headers */

    /* examine return code */
    //Skip "HTTP/1.0"
    //remove the '\0'
    // Normal reply has return code 200

    /* print first part of response */

    /* second read loop -- print out the rest of the response */

    /*close socket and deinitialize */

    if (ok) {
	return 0;
    } else {
	return -1;
    }
}

int write_n_bytes(int fd, char * buf, int count) {
    int rc = 0;
    int totalwritten = 0;

    while ((rc = minet_write(fd, buf + totalwritten, count - totalwritten)) > 0) {
	totalwritten += rc;
    }

    if (rc < 0) {
	return -1;
    } else {
	return totalwritten;
    }
}


