#include "fsutil.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include "herrors.h"

bool IsDirectory(std::string path) {
    DWORD attrib = GetFileAttributesA(path.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

std::string FormatCreationTime(const FILETIME ftCreationTime) {
    // Convert file creation time to system time
    SYSTEMTIME sysTime;
    FileTimeToSystemTime(&ftCreationTime, &sysTime);

    // Format the date and time
    char buffer[20];
    sprintf_s(buffer, "%02d/%02d/%04d  %02d:%02d", sysTime.wDay, sysTime.wMonth, sysTime.wYear,
        sysTime.wHour, sysTime.wMinute);

    return std::string(buffer);
}