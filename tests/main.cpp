#include <iostream>
#include "../cli-framework/framework.hpp"
#include "vec_util.hpp"
#include <thread>
#include <conio.h>
#include <cassert>

void echoFn(cli::Args args)
{
	for (const std::string& arg : args)
		std::cout << arg << ' ';

	std::cout << '\n';
}

int main(int argc, const char* argv[])
{
	cli::Flags flags(argc, argv, true);

	bool x{};
	int i = 10;
	std::string str = "nope";

	flags
		.set(x, "x", "does something probably")
		.set(i, "i", "this is an int")
		.set(str, "s", "this is a string")
		.parse();

	/*std::cout << "i: " << i << '\n';
	std::cout << "x: " << x << '\n';
	std::cout << "s: " << str << '\n';*/

	cli::CommandHandler handler{};

	cli::Command echo =
	{
		{"e", "repeat"},
		"Repeats the arguments back",
		5000,
		echoFn
	};

	cli::Command help =
	{
		{"h"},
		"Shows command info",
		1000,
		[handler](cli::Args args)
		{
			for (auto& [k, v] : handler.cmds)
			{
				std::cout << k << ": " << v.description << '\n';
			}
		}
	};

	handler.cmds = 
	{
		{"echo", echo},
		{"help", help}
	};
	
	std::vector<std::string> args = flags.getArgs();

	if (args.size() == 0)
		return 0;

	std::string cmdName = args[0];

	args.erase(args.begin());

	bool run = handler.run(cmdName, args);

	if (!run)
		std::cout << "Command failed!";

	/*std::cout << cli::color("rgb go brrr", { "102", "255", "153" }) << '\n';
	std::cout << cli::double_underline("double underline me") << '\n';
	std::cout << cli::underline("single underline me") << '\n';
	std::cout << cli::strike("cross me out") << '\n';
	std::cout << cli::bold("bold for the win") << '\n';
	std::cout << cli::italic("itaaaaalic") << '\n';
	std::cout << cli::color("Testing", 99) << '\n';*/

	using namespace std::chrono;

	/*
	std::cout << cli::hide_cursor();

	int i = 0;
	while (true)
	{
		if(i != 0)
			std::cout << cli::cursorUp() << cli::delete_cursor();

		std::cout << cli::color(std::to_string(i), i);
		i++;

		std::this_thread::sleep_for(100ms);

		std::cout << cli::cursorDown();
	}

	std::cout << cli::show_cursor();*/

	/*std::cout << cli::hide_cursor();
	std::cout << cli::save_cursor();

	while (true)
	{
		std::cout << cli::restore_cursor();

		std::cout << "loading";
		
		std::this_thread::sleep_for(200ms);

		for (int i = 1; i <= 3; i++)
		{
			std::cout << ".";
			std::this_thread::sleep_for(500ms);
		}
		
		std::cout << cli::eraseline(2);
	}
	
	std::cout << cli::show_cursor();*/
	

	/*std::string symbol = cli::color("[o|o]", cli::colors::red);

	std::cout << symbol;

	std::cout << cli::hide_cursor();

	while (true)
	{
		int ch = _getch();

		switch (ch)
		{
		case 54:
				std::cout << cli::eraseLine(2);
				std::cout << cli::cursorForward(1);
				std::cout << symbol;
				break;
		case 52:
				std::cout << cli::eraseLine(2);
				std::cout << cli::cursorBack(2);
				std::cout << symbol;
				break;
		case 50:
				std::cout << cli::eraseLine(2);
				std::cout << cli::cursorDown();
				std::cout << cli::cursorBack();
				std::cout << symbol;
				break;
		case 56:
				std::cout << cli::eraseLine(2);
				std::cout << cli::cursorUp();
				std::cout << cli::cursorBack();
				std::cout << symbol;
				break;
		}
	}*/
	
	
}  