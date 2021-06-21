# cli-framework
A flag parser, command handler and an ANSI escape code wrapper into one library

## Flag usage

flags can be used like `-flag=value | -flag value | -flag`
```c++
int main(int argc, const char* argv[])
{
  // defines the flag class
  // the third in initializer is an autohelp that will trigger when a user enters an incorrect flag
	cli::Flags flags(argc, argv, true);

  // our buffer vairables
  // the values wont be changed if no flag is found acting as a default value system
	bool x = false;
	int i = 10;
	std::string str = "nope";

  // sets the buffer flag name and flag description and then calls parse to parse the flags
	flags
		.set(x, "x", "does something probably")
		.set(i, "i", "this is an int")
		.set(str, "s", "this is a string")
		.parse();

  // displays the flags called
	std::cout << "i: " << i << '\n';
	std::cout << "x: " << x << '\n';
	std::cout << "s: " << str << '\n';
  
  return 0;
}
```

## Command usage

```c++
// defines a command that echos back the arguments
cli::Command echo =
	{
		{"e", "repeat"}, // the command aliases
		"Repeats the arguments back", // command description
		5000, // cooldown in ms 
		[](cli::Args args) // the command logic implemnted as a lambda but can work as a function ptr as well. cli::Args is just a vector<string> alias
		{
			for (const std::string& arg : args)
				std::cout << arg << ' ';

			std::cout << '\n';
		}
	};
	
  // initializes the command handler with the command desired commands
  // first is the command name/id the second is the command struct
  // you can set new commands like a normal map with handler.cmds["newcmd"] = data;
	cli::CommandHandler handler =
	{
		{"echo", echo}
	};
	
  // gets system arguments into a vector. alternatively you can use Flags::getArgs() instead if you're using this with the flag parser
	std::vector<std::string> args = cli::vecArgs(argc, argv);

	if (args.size() == 0)
		return 0;

	std::string cmdName = args[0];

	args.erase(args.begin());

  // run with cmd name and arguments 
  // returns false if command failed
	bool run = handler.run(cmdName, args);

	if (!run)
		std::cout << "Command failed!";
```

## ANSI usage

note that not all of the text formatting functions will work with every terminal
all of these work great with windows terminal but only half worked with command prompt 

since the functions only wrap ansi codes they still need to be outputed to the console 

```c++
  std::cout << cli::color("rgb go brrr", { "102", "255", "153" }) << '\n'; // sets the color to the given rgb value
	std::cout << cli::color("Testing", 99) << '\n'; // sets the color with a number in a 255 bit range
  std::cout << cli::color("make me red", cli::colors::red) << '\n'; // sets the color to red using a 3-4 bit range color using an enum
  std::cout << cli::color("make the background white", cli::bg_colors::white) << '\n'; // sets the background color to white also works with other color functions
	std::cout << cli::double_underline("double underline me") << '\n'; // sets a double underline
	std::cout << cli::underline("single underline me") << '\n'; // sets a single underline
	std::cout << cli::strike("cross me out") << '\n'; // crosses out text
	std::cout << cli::bold("bold for the win") << '\n'; // makes the text bold
	std::cout << cli::italic("itaaaaalic") << '\n'; // makes the text italic

  // hides cursor
  std::cout << cli::hide_cursor();

	int i = 0;
	while (true)
	{
    // if its not the first iteration it will move the cursor up and delete the line
		if(i != 0)
			std::cout << cli::cursorUp() << cli::eraseLine();
  
    // draws the current iteration with the correct color in a 255 bit range
		std::cout << cli::color(std::to_string(i), i);
		i++;

    // sleeps for 100ms 
		std::this_thread::sleep_for(100ms);
  
    // moves the cursor back up
		std::cout << cli::cursorDown();
	}
```
