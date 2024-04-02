#pragma once

#include <iostream>
#include <Windows.h>

std::string GetErrorMessage(DWORD errorMessageID);

#define LERROR(msg) {\
                           std::cerr << (msg) << " Error: " << GetErrorMessage(GetLastError()) << std::endl; \
                   }

#define ERROR_CODE(msg, error) {\
                           std::cerr << (msg) << " Error: " << GetErrorMessage(error) << std::endl; \
                   }