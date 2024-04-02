#pragma once

#include <vector>
#include <string>

void SwitchToRemote(char* address);
std::vector<std::string> split(std::string args, char seperator);
std::string GetProcessDirectory();
std::string ToUpperCase(std::string str);