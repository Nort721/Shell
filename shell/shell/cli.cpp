#include "cli.h"
#include "shellutil.h"
#include "fsutil.h"

#include <iostream>
#include <map>
#include <Windows.h>
#include <math.h>
#include "herrors.h"

std::map<std::string, command*> commands;

class helpCommand : public command {
public:
	helpCommand() {
		helpMessage = "Provides Help information for commands.";
	}
	void onCommand(std::vector<std::string> args) override {
		std::string helpMsg = "";

		for (const std::pair<std::string, command*>& pair : commands) {
			std::string spaces(max(15 - pair.first.size(), 5), ' ');
			helpMsg += pair.first + spaces + pair.second->helpMessage + "\n";
		}

		std::cout << helpMsg << std::endl;
	}
};

class cdCommand : public command {
public:
	cdCommand() {
		helpMessage = "Displays the name of or changes the current directory.";
	}
	void onCommand(std::vector<std::string> args) override {
		// no directory provided, print the current one
		if (args.size() == 1) {
			std::cout << GetProcessDirectory() << std::endl;
			return;
		}

		std::string path = args[1];

		// set the current dictory to the new path
		if (SetCurrentDirectoryA(path.c_str()) == 0) {
			LERROR("Unable to change directory to " + path + ".");
		}
	}
};

class lsCommand : public command {
public:
	lsCommand() {
		helpMessage = "Displays a list of files and subdirectories in a directory.";
	}
	void onCommand(std::vector<std::string> args) override {
		std::string path;
		
		// if dir is provided use it, otherwise use current working dir
		if (args.size() == 1) {
			path = GetProcessDirectory();
		}
		else {
			path = args[1];
		}

		path += "\\*";

		WIN32_FIND_DATAA findData;
		HANDLE hFind = FindFirstFileA(path.c_str(), &findData);

		if (hFind == INVALID_HANDLE_VALUE) {
			LERROR("Failed listing directory contents.");
			return;
		}

		do {
			std::cout << FormatCreationTime(findData.ftCreationTime) << "    " << findData.cFileName << std::endl;
		} while (FindNextFileA(hFind, &findData) != 0);

		FindClose(hFind);
	}
};

class catCommand : public command {
public:
	catCommand() {
		helpMessage = "Prints a file to the standard output.";
	}
	void onCommand(std::vector<std::string> args) override {
		// validate arguments count
		if (args.size() != 2) {
			std::cout << "Usage: cat <filename>" << std::endl;
			return;
		}

		std::string path = args[1];

		// make sure path leads to a file
		if (IsDirectory(path)) {
			std::cout << "Path must point to a file." << std::endl;
			return;
		}

		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			LERROR("Unable to open the file.");
			return;
		}

		DWORD fSize = GetFileSize(hFile, NULL);
		if (fSize == INVALID_FILE_SIZE) {
			LERROR("Unable to get file size.");
			CloseHandle(hFile);
			return;
		}

		char* buffer = (char*)malloc(fSize);
		if (buffer == NULL) {
			LERROR("Unable to allocate memory.");
			CloseHandle(hFile);
			return;
		}

		DWORD bytesRead;
		if (!ReadFile(hFile, buffer, fSize, &bytesRead, NULL)) {
			LERROR("Unable to read file.");
			CloseHandle(hFile);
			free(buffer);
			return;
		}

		std::cout << std::string(buffer, bytesRead) << "\n";

		CloseHandle(hFile);
		free(buffer);
	}
};

void StartCLI()
{
	// register supported commands
	commands.insert(std::make_pair("CD", new cdCommand()));
	commands.insert(std::make_pair("LS", new lsCommand()));
	commands.insert(std::make_pair("CAT", new catCommand()));
	commands.insert(std::make_pair("HELP", new helpCommand()));

	// start interactive interface
	std::string input = "";

	while (TRUE)
	{
		// generate and print the current workingdir
		std::string prefix = GetProcessDirectory() + '>';
		std::cout << prefix;

		getline(std::cin, input);

		// skip empty commands
		if (input.size() == 0) {
			continue;
		}

		std::vector<std::string> args = split(input, ' ');
		std::string commandName = ToUpperCase(args[0]);

		// if the command is registered execute its onCommand function
		if (commands.count(commandName)) {
			commands[commandName]->onCommand(args);
		}
		else
		{
			std::cout << "Unknown command, type help for help." << std::endl;
		}
	}
}