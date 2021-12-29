#pragma once

#include <string_view>
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <initializer_list>
#include <functional>
#include <ostream>

namespace cli
{
	class CommandHandler
    {
    public:

        using Args = std::vector<std::string>;
        using ExecFN = std::function<void(Args)>;

        struct Command
        {
            std::vector<std::string> alias;
            std::string_view description;
            size_t cooldown;
            ExecFN exec;
        };

        struct Cooldown
        {
            std::chrono::milliseconds amount;
            std::chrono::time_point<std::chrono::system_clock> time;
        };

        std::map<std::string_view, Command> cmds;

        CommandHandler(std::initializer_list<std::pair<const std::string_view, Command>> commands)
                : cmds(commands)
        {}

        CommandHandler() = default;

        bool run(std::string_view name, Args& args)
        {

            std::string_view cmd_name;

            if (cmds.contains(name))
                cmd_name = name;
            else
            {
                std::string_view alias = find_by_alias(name);

                if (!cmds.contains(alias))
                    return false;

                cmd_name = alias;
            }

            Command &cmd = cmds[cmd_name];

            if(!manage_cooldown(cmd_name, cmd))
                return false;

            cmd.exec(args);

            return true;
        }

        void help(std::ostream &os)
        {
            for (auto&[k, v]: cmds)
            {
                os << k << ": " << v.description << '\n';
            }
        }

    private:
        std::map<std::string_view, Cooldown> cooldowns;

        bool manage_cooldown(std::string_view cmd_name, Command& cmd)
        {
            using namespace std::chrono;

            if (!cooldowns.contains(cmd_name))
            {
                cooldowns[cmd_name] = Cooldown
                {
                    .amount =  milliseconds(cmd.cooldown),
                    .time   = system_clock::now()
                };
            }
            else
            {
                auto &cooldown  = cooldowns[cmd_name];
                auto now        = system_clock::now();
                auto diff       = duration_cast<milliseconds>(now - cooldown.time);

                if (diff <= cooldown.amount)
                    return false;
                else
                    cooldowns.erase(cmd_name);
            }

            return true;
        }

		std::string_view find_by_alias(std::string_view alias)
		{
			for (auto& [k, v] : cmds)
			{
				if (v.alias.empty())
					return {};

				for (auto &a : v.alias)
				{
					if (a == alias)
						return k;
				}
			}

			return {};
		}
	};
}


