#include <iostream>
#include "cli.h"
#include "shellutil.h"

int main(int argc, char* argv[])
{
	if (argc > 1) {
		SwitchToRemote(argv[1]);
	}

	std::cout << "Interactive Shell [Version 1.0.0]\n(c) <TEAM> Corporation. All rights reserved.\n\n";

	StartCLI();

	return 0;
}