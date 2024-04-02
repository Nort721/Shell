#pragma once

#include <string>
#include <vector>

class command {
public:
	std::string helpMessage = "";
	command() {}
	virtual void onCommand(std::vector<std::string> args) = 0;
};

void StartCLI();