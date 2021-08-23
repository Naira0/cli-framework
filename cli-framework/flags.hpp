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
		BIG_INT,
		STRING,
		FLOAT
	};

	struct FlagData
	{
		void*		     buff;
		enum class Type	 type;
		std::string		 description;
	};

	class Flags
	{
	public:
		Flags(int count, const char* argv[], bool auto_help = false, const std::string& help_keyword = "help") 
			: 
				ah(auto_help),
				argc(count),
				argv(argv)
		{
			hkw = "-";
			hkw += help_keyword;
		}

		Flags& set(bool& buff, const std::string& name, const std::string& description)
		{
			add_flag(Type::BOOL, buff, name, description);
			return *this;
		}

		Flags& set(int32_t& buff, const std::string& name, const std::string& description)
		{
			add_flag(Type::INT, buff, name, description);
			return *this;
		}

		Flags& set(int64_t& buff, const std::string& name, const std::string& description)
		{
			add_flag(Type::BIG_INT, buff, name, description);
			return *this;
		}

		Flags& set(std::string& buff, const std::string& name, const std::string& description)
		{
			add_flag(Type::STRING, buff, name, description);
			return *this;
		}
		
		Flags& set(float& buff, const std::string& name, const std::string& description)
		{
			add_flag(Type::FLOAT, buff, name, description);
			return *this;
		}

		void parse()
		{
			bool skipNext = false;

			for (int i = 1; i < argc; i++)
			{
				if (skipNext)
				{
					skipNext = false;
					continue;
				}

				std::pair<std::string, std::string> values;

				bool eqSign = has_equal(argv[i]);

				if (eqSign)
					values = get_equal(argv[i]);

				std::string flagName = eqSign ? values.first : argv[i];

				bool isFlag = flags.contains(flagName);

				// if the flag is the help keyword or if auto help is enabled and its an incorrect flag it will trigger the help message
				if ((flagName == hkw) || ah && argv[i][0] == '-' && !isFlag)
					return help();

				if (!isFlag)
				{
					clean_args.push_back(argv[i]);
					continue;
				}

				FlagData flag = flags[flagName];

				parse_type(flag.type, flag, values.second, i, eqSign, skipNext);
			}
		}

		void help()
		{
			std::cout << "\nUsage:\n -flag=value, -flag value, -flag\n\n";

			for (const auto& [name, data] : flags)
			{

				std::cout << name << ' ' << enum_to_str(data.type) << " | " << data.description << '\n';
			}
		}

		std::string help_str(bool usage = false)
		{
			std::stringstream ss;

			if(usage)
				std::cout << "\nUsage:\n -flag=value, -flag value, -flag\n\n";

			for (const auto& [name, data] : flags)
			{
				ss << name << ' ' << enum_to_str(data.type) << " | " << data.description << '\n';
			}

			return ss.str();
		}

		// gets the command line args without the flags.
		// if parse has not been called before this function the flags wont be removed
		std::vector<std::string> get_args() { return clean_args; }

	private:
		std::vector<std::string> clean_args{};
		std::map<std::string, FlagData> flags{};
		int argc;
		const char** argv;
		bool ah;
		std::string hkw;

		inline std::string enum_to_str(enum class Type t)
		{
			std::string type;

			switch (t)
			{
			case Type::BOOL:
				type = "bool";
				break;
			case Type::BIG_INT:
				type = "Big int";
				break;
			case Type::INT:
				type = "number";
				break;
			case Type::STRING:
				type = "string";
				break;
			case Type::FLOAT:
				type = "floating point number";
				break;
			}

			return type;
		}

		inline bool has_equal(const std::string& str)
		{
			if (str[0] == '-' && str.find("=") != std::string::npos)
				return true;

			return false;
		}

		// the first value will be the flag name including the slash
		// the second value will be the value of the flag
		// this function shouldnt be call unless you know the flag has an equal sign
		// check with the previous function
		std::pair<std::string, std::string> get_equal(const std::string& str)
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

		// this function is templeted because only functions with allowed flag types will be calling it 
		template<typename T>
		inline void add_flag(enum class Type t, T& buff, const std::string& name, const std::string& description)
		{
			FlagData data
			{
				&buff,
				t,
				description,
			};

			std::string flag_name = "-";
			flag_name += name;

			flags[flag_name] = data;
		}

		// parses a single type to the appropriate value
		void parse_type(enum class Type t, FlagData& flag, std::string& value, int i, bool eq, bool& skip_next)
		{

			if (t == Type::BOOL)
			{
				*(bool*)flag.buff = true;
				return;
			}

			std::string raw = eq ? value : argv[i + 1];

			if (!eq)
				skip_next = true;

			try {
				switch (t)
				{
				case Type::INT:		*(int32_t*)flag.buff     = std::stol(raw);		break;
				case Type::STRING:	*(std::string*)flag.buff = raw;					break;
				case Type::FLOAT:   *(float*)flag.buff		 = std::stof(raw);		break;
				case Type::BIG_INT: *(int64_t*)flag.buff	 = std::stoll(raw);		break;
				}
			}
			catch (std::exception e)
			{}
		}
	};
}