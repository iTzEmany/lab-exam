#ifndef HEADER_H_
#define HEADER_H_

#ifdef _WIN32
// For Windows platform
typedef int socklen_t;
#include <winsock.h>
#else
// For UNIX-like platforms
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#include <termios.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_NCLIENTS 5
#define BUFFSIZE 1024
#define MAXLENGHT 10
#define PORT 80

// Structure to represent a calculator with operator and operands
typedef struct {
    char operator[MAXLENGHT];
    char op1[MAXLENGHT];
    char op2[MAXLENGHT];
} calculator;

#endif /* HEADER_H_ */
