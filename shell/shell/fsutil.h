#pragma once

#include <string>
#include <Windows.h>

std::string GetAbsolutePath(std::string relativePath);
bool IsDirectory(std::string path);
std::string FormatCreationTime(const FILETIME ftCreationTime);