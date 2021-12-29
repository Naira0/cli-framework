#pragma once

#include <vector>
#include <string>

namespace cli
{
    // puts systems arguments into a vector starting from index 1
	std::vector<std::string> vec_args(int argc, const char **argv)
	{
        if(argc == 1)
            return {};

		std::vector<std::string> data;

		data.reserve(argc);

		for (size_t i = 1; i < argc; i++)
			data.emplace_back(argv[i]);

		return data;
	}

}