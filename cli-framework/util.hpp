#pragma once

#include <vector>
#include <string>

namespace cli
{
	std::vector<std::string> vecArgs(int argc, const char* argv[])
	{
		std::vector<std::string> data;

		data.reserve(argc);

		for (int i = 0; i < argc; i++)
			data.push_back(argv[i]);

		return data;
	}

}