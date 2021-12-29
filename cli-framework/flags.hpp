#pragma once

#include <vector>
#include <string>
#include <map>
#include <ostream>
#include <string_view>

namespace cli
{
	class Flags
	{
	public:

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
            void*		buff;
            Type	    type;
            const char*	description;
        };

		Flags(int count, const char* argv[], bool auto_help = false, const char* help_keyword = "help")
			:
                auto_help(auto_help),
                argc(count),
                argv(argv),
                help_keyword(std::move(std::string("-")+help_keyword))
		{}

		Flags& set(bool& buff, const std::string& name, const char* description)
		{
			add_flag(Type::BOOL, buff, name, description);
			return *this;
		}

		Flags& set(int32_t& buff, const std::string& name, const char* description)
		{
			add_flag(Type::INT, buff, name, description);
			return *this;
		}

		Flags& set(int64_t& buff, const std::string& name, const char* description)
		{
			add_flag(Type::BIG_INT, buff, name, description);
			return *this;
		}

		Flags& set(std::string& buff, const std::string& name, const char* description)
		{
			add_flag(Type::STRING, buff, name, description);
			return *this;
		}
		
		Flags& set(float& buff, const std::string& name, const char* description)
		{
			add_flag(Type::FLOAT, buff, name, description);
			return *this;
		}

		void parse()
		{
            clean_args.reserve(argc);

			bool skip_next = false;

			for (int i = 1; i < argc; i++)
			{
				if (skip_next)
				{
                    skip_next = false;
					continue;
				}

				auto [p_flag, p_value] = get_equal(argv[i]);

                bool has_equal = !p_flag.empty();

				std::string flag_name = has_equal ? std::move(p_flag) : argv[i];

				bool is_flag = flags.contains(flag_name);

				// if the flag is the help keyword or if auto help is enabled and its an incorrect flag it will trigger the help message
				if ((flag_name == help_keyword) || (auto_help && argv[i][0] == '-' && !is_flag))
					return help(std::cout);

				if (!is_flag)
				{
					clean_args.emplace_back(argv[i]);
					continue;
				}

				FlagData flag = flags[flag_name];

                if(flag.type == Type::BOOL)
                {
                    *(bool*)flag.buff = true;
                    continue;
                }

                std::string value;

                if(has_equal)
                    value = std::move(p_value);
                else
                {
                    if(i+1 == argc)
                        return;
                    value = argv[i+1];
                    skip_next = true;
                }

				parse_type(flag.type, flag.buff, value);
			}
		}

        // outputs a help message to your ostream of choice
        void help(std::ostream& os)
        {
            os << "\nUsage:\n -flag=value, -flag value, -flag\n\n";

            for (const auto& [name, data] : flags)
            {
                os
                    << name
                    << " | "
                    << enum_to_str(data.type)
                    << " | " << data.description
                    << '\n';
            }
        }

        std::vector<std::string> clean_args{};
        std::map<std::string, FlagData> flags{};

	private:
		int argc;
		const char** argv;
		bool auto_help;
		std::string help_keyword;

		inline std::string_view enum_to_str(Type t)
		{
            std::string_view type;

			switch (t)
			{
                case Type::BOOL:    type = "bool";      break;
                case Type::BIG_INT: type = "Big int";   break;
                case Type::INT:     type = "int";	    break;
                case Type::STRING:  type = "string";	break;
                case Type::FLOAT:   type = "float";     break;
			}

			return type;
		}

		std::pair<std::string, std::string> get_equal(const std::string& str)
		{
            if(str[0] != '-')
                return {};

            size_t i = str.find('=');

            if(i == std::string::npos)
                return {};

			std::pair<std::string, std::string> values
            {
                str.substr(0, i),
                str.substr(i+1)
            };

			return values;
		}

		// this function is templated because only functions with allowed flag types will be calling it
		template<typename T>
		inline void add_flag(Type t, T& buff, const std::string& name, const char* description)
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
		inline void parse_type(Type t, void* buff, std::string& value)
		{
			try
            {
				switch (t)
				{
                    case Type::INT:		*(int32_t*)buff         = std::stol(value);		    break;
                    case Type::STRING:	*(std::string*)buff     = value;					break;
                    case Type::FLOAT:   *(float*)buff		    = std::stof(value);		    break;
                    case Type::BIG_INT: *(int64_t*)buff	        = std::stoll(value);		break;
				}
			}
			catch (...)
			{}
		}
	};
}