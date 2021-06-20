#pragma once

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <array>

namespace cli
{
	enum class Type
	{
		BOOL,
		INT,
		STRING
	};

	struct FlagData
	{
		bool*			 boolBuff;
		int*			 intBuff;
		std::string*     stringBuff;
		enum class Type	 type;
		std::string		 description;
	};

	class Flags
	{
	public:
		Flags(int count, const char* argv[], bool autoHelp = false) : ah(autoHelp)
		{
			args.reserve(count);

			for (int i = 0; i < count; i++)
			{
				args.push_back(argv[i]);
			}
		}

		Flags(std::vector<std::string> argv, bool autoHelp = false)
			: 
			args(argv),
			ah(autoHelp)
		{}

		Flags& set(bool& buff, const std::string& name, const std::string& description)
		{
			FlagData data
			{
				&buff,
				nullptr,
				nullptr,
				Type::BOOL,
				description,
			};

			std::string flagName = "-";
			flagName += name;
			
			flags[flagName] = data;

			return *this;
		}

		Flags& set(int& buff, const std::string& name, const std::string& description)
		{
			FlagData data
			{
				nullptr,
				&buff,
				nullptr,
				Type::INT,
				description,
			};

			std::string flagName = "-";
			flagName += name;

			flags[flagName] = data;

			return *this;
		}

		Flags& set(std::string& buff, const std::string& name, const std::string& description)
		{
			FlagData data
			{
				nullptr,
				nullptr,
				&buff,
				Type::STRING,
				description,
			};

			std::string flagName = "-";
			flagName += name;

			flags[flagName] = data;

			return *this;
		}

		void parse()
		{
			bool skipNext = false;
			size_t n = args.size();

			for (size_t i = 1; i < n; i++)
			{
				if (skipNext)
				{
					skipNext = false;
					continue;
				}

				std::pair<std::string, std::string> values;

				bool eqSign = hasEqual(args[i]);

				if (eqSign)
					values = getEqual(args[i]);

				std::string flagName = eqSign ? values.first : args[i];

				bool isFlag = flags.contains(flagName);

				if (ah && args[i][0] == '-' && !isFlag)
					return help();

				if (!isFlag)
				{
					cleanArgs.push_back(args[i]);
					continue;
				}

				FlagData flag = flags[flagName];

				switch (flag.type)
				{
				case Type::BOOL:
					*flag.boolBuff = true;
					break;
				case Type::INT:
					if (eqSign)
						*flag.intBuff = std::stoi(values.second);
					else
					{
						if (i + 1 >= n)
							break;
						*flag.intBuff = std::stoi(args[i + 1]);
						skipNext = true;
					}
					break;
				case Type::STRING:
					if (eqSign)
						*flag.stringBuff = values.second;
					else
					{
						if (i + 1 >= n)
							break;
						*flag.stringBuff = args[i + 1];
						skipNext = true;
					}
				}
			}
		}

		void help()
		{
			std::cout << "\nUsage:\n -flag=value, -flag value, -flag\n\n";

			for (const auto& [name, data] : flags)
			{

				std::cout << name << ' ' << enumToStr(data.type) << " | " << data.description << '\n';
			}
		}

		std::string helpStr(bool usage = false)
		{
			std::stringstream ss;

			if(usage)
				std::cout << "\nUsage:\n -flag=value, -flag value, -flag\n\n";

			for (const auto& [name, data] : flags)
			{
				ss << name << ' ' << enumToStr(data.type) << " | " << data.description << '\n';
			}

			return ss.str();
		}

		// gets the command line args without the flags.
		// if parse has not been called before this function the flags wont be removed
		std::vector<std::string> getArgs() { return cleanArgs; }

	private:
		// the main reason why i have 2 vectors instead of one that i just remove the args from is
		// because having 2 vectors seems like it would be cleaner, faster and safer then erasing the elements
		std::vector<std::string> args{};
		std::vector<std::string> cleanArgs{};
		std::map<std::string, FlagData> flags;
		bool ah;

		inline bool hasEqual(const std::string& str)
		{
			if (str[0] == '-' && str.find("=") != std::string::npos)
				return true;

			return false;
		}

		inline std::string enumToStr(enum class Type t)
		{
			std::string type;

			switch (t)
			{
			case Type::BOOL:
				type = "bool";
				break;
			case Type::INT:
				type = "number";
				break;
			case Type::STRING:
				type = "string";
				break;
			}

			return type;
		}

		// the first value will be the flag name including the slash
		// the second value will be the value of the flag
		// this function shouldnt be call unless you know the flag has an equal sign
		// check with the previous function
		std::pair<std::string, std::string> getEqual(const std::string& str)
		{
			std::pair<std::string, std::string> values;
			std::string store;
			
			for (const char c : str)
			{
				if (flags.contains(store))
				{
					values.first = store;
					store.clear();
					continue;
				}

				if (c == '=')
					continue;

				store += c;
			}

			values.second = store;

			return values;
		}
	};
}