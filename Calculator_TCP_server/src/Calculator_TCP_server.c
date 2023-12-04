#include "header.h"

// Define a global calculator structure and variables
calculator cal;
int flag;
char res[BUFFSIZE] = "";

void clearConsole() {
#ifdef _WIN32
    system("cls");  // For Windows
#else
    system("clear");  // For macOS and Linux
#endif
}
// Function to check if all characters in operands are numeric
int areAllNumericChars() {
	const char *app[] = { cal.op1, cal.op2 };
	int size = sizeof(app) / sizeof(app[0]);
	for (int i = 0; i < size; i++) {
		const char *str = app[i];
		while (*str) {
			if (!isdigit(*str)) {
				return 1;
			}
			str++;
		}
	}
	return 0;
}


// Function to perform addition
void add(char ope1[], char ope2[]) {
	snprintf(res, sizeof(res), "%.2f", (float) atoi(ope1) + (float) atoi(ope2));
}

// Function to perform subtraction
void sub(char ope1[], char ope2[]) {
	snprintf(res, sizeof(res), "%.2f", (float) atoi(ope1) - (float) atoi(ope2));
}

// Function to perform multiplication
void mult(char ope1[], char ope2[]) {
	snprintf(res, sizeof(res), "%.2f", (float) atoi(ope1) * (float) atoi(ope2));
}

// Function to perform division
void division(char ope1[], char ope2[]) {
	float op2 = atof(ope2);
	if (op2 != 0) {
		snprintf(res, sizeof(res), "%.2f",
				(float) atoi(ope1) / (float) atoi(ope2));
	} else {
		strcpy(res, "Error: Division by zero\n");
	}
}

int main() {
#ifdef _WIN32
	// Initialize Winsock for Windows platform
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Error initializing Winsock\n");
		return 1;
	}
#endif

	// Declare server and client socket variables, and address structures
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);

	// Create a socket for the server
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		printf("Error creating socket\n");
		return 1;
	}

	// Configure the server address structure
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	// Bind the server socket to the specified address
	if (bind(serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
		printf("Error binding socket\n");
		return 1;
	}

	// Start listening for incoming connections
	listen(serverSocket, MAX_NCLIENTS);
	puts("The server is listening...");

	// Infinite loop to handle multiple client connections
	while (1) {
		// Accept a connection from a client
		clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddr, &clientAddrSize);
		if (clientSocket == -1) {
			printf("Error accepting connection\n");
			return 1;
		}

		// Get the IP address of the connected client
		char *clientIP = inet_ntoa(clientAddr.sin_addr);
		clearConsole();
		printf("Connection successfully established with %s:%i\n", clientIP, PORT);

		// Loop to receive and process data from the client
		while ((flag = recv(clientSocket, (char*) &cal, sizeof(calculator), 0)) > 0) {
			if (flag == -1) {
				printf("Error receiving data\n");
				return 1;
			}

			// Check if the operands are numeric
			if (areAllNumericChars() != 1) {
				// Perform the appropriate mathematical operation based on the operator
				if (strcmp(cal.operator, "+") == 0) {
					add(cal.op1, cal.op2);
				} else if (strcmp(cal.operator, "-") == 0) {
					sub(cal.op1, cal.op2);
				} else if (strcmp(cal.operator, "x") == 0) {
					mult(cal.op1, cal.op2);
				} else if (strcmp(cal.operator, "/") == 0) {
					division(cal.op1, cal.op2);
				} else {
					// If the operator is not recognized, set an error message
					strcpy(res, "This mathematical operation is invalid");
				}
			} else {
				// If operands are not numeric, set an error message
				strcpy(res, "This mathematical operation is invalid");
			}

			// Print the operands, operator, and result
			printf("%s %s %s =\t%s\n", cal.op1, cal.operator, cal.op2, res);

			// Send the result back to the client
			send(clientSocket, res, BUFFSIZE, 0);
			memset(res, '\0', BUFFSIZE);

			// Check for client disconnection
			if (flag == 0) {
				printf("The client has disconnected\n");
				closesocket(clientSocket);
			} else if (flag == -1) {
				printf("Error receiving data\n");
				return 1;
			}
		}
	}

	// Close the server socket and cleanup resources
#ifdef _WIN32
	closesocket(serverSocket);
#else
	close(serverSocket);
#endif

#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
}
