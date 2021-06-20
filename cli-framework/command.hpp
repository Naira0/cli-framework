#pragma once

#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <initializer_list>
#include <functional>

namespace cli
{
	using Args = std::vector<std::string>;

	struct Command
	{
		std::vector<std::string> alias;
		std::string description;
		size_t cooldown;
		std::function<void(std::vector<std::string>)> exec;
	};

	struct Cooldown
	{
		std::chrono::milliseconds amount;
		std::chrono::time_point<std::chrono::system_clock> time;
	};

	class CommandHandler
	{
	public:
		std::map<std::string, Command> cmds;

		CommandHandler(std::initializer_list<std::pair<const std::string, Command>> commands)
			: cmds(commands) {}

		CommandHandler() = default;

		bool run(std::string name, std::vector<std::string> args)
		{
			std::string alias = findbyAlias(name);
			std::string parsedName = alias.empty() ? name : alias ;

			if (!cmds.contains(parsedName))
				return false;

			Command& cmd = cmds[parsedName];

			if (!cooldowns.contains(parsedName))
			{
				Cooldown data =
				{
					std::chrono::milliseconds(cmd.cooldown),
					std::chrono::system_clock::now()
				};

				cooldowns[parsedName] = data;
			}
			else {

				using namespace std::chrono;

				auto& cooldown = cooldowns[parsedName];
				auto now = system_clock::now();
				auto diff = duration_cast<milliseconds>(now - cooldown.time);

				if (diff <= cooldown.amount)
					return false;
				else
					cooldowns.erase(parsedName);
			}

			cmd.exec(args);

			return true;
		}

		void help()
		{
			for (auto& [k, v] : cmds)
			{
				std::cout << k << ": " << v.description << '\n';
			}
		}

	private:
		std::map<std::string, Cooldown> cooldowns;

		std::string findbyAlias(std::string& alias)
		{
			for (auto& [k, v] : cmds)
			{
				if (v.alias.size() == 0)
					return "";

				for (auto &a : v.alias)
				{
					if (a == alias)
						return k;
				}
			}

			return "";
		}
	};
}


