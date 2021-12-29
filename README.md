# cli-framework
A flag parser, command handler and an ANSI escape code wrapper into one library

## Flag usage

flags can be used like `-flag=value | -flag value | -flag`
```c++
int main(int argc, const char* argv[])
{

	// inits the flags. // the third (optional) param specifies if auto help should be enabled.
	// the fourth specifies the reserved help flag.
	cli::Flags flags(argc, argv, true, "h");

	// flag buffers. if the flag isnt found or a conversion error happens then the value wont be changed.
	// this acts as a default value system.
    bool x				= false;
    int i				= 10;
    float f				= 5.10;
    int64_t big			= 100000000000000;
    std::string str 	= "nope";

	// sets the flags and parses them
    flags
            .set(x,   "x", "does something probably")
            .set(i,   "i", "this is an int")
            .set(big, "big", "a very large int flag")
            .set(str, "s", "this is a string")
            .set(f,   "f", "this is a floating point number")
            .parse();

	// displays flags
    std::cout 
		<< "Flags:\n"
		<< "x: "   << x   << '\n'
		<< "i: "   << i   << '\n'
		<< "s: "   << str << '\n'
		<< "F: "   << f   << '\n'
		<< "big: " << big << '\n';

	// displays non flag args
    std::cout << "Clean args:\n";

	// clean args can be used to get all the non flag args not including the executable name
    for(const auto& arg : flags.clean_args)
        std::cout << arg << '\n';

	return 0;
}
```

## Command usage

```c++

	using namespace cli::CommandHandler;

	// defines a command that echos back the arguments
     Command echo =
		{
			.alias       = {"e", "repeat"}, // the command aliases
			.description = "Repeats the arguments back", // command description
			.cooldown    = 5000, // cooldown in ms
			.exec        = [](Args args) // the command logic implemnted as a lambda but can work as a function ptr as well. cli::Args is just a vector<string> alias
			{
				for (const std::string& arg : args)
					std::cout << arg << ' ';

				std::cout << '\n';
			}
		};

    // initializes the command handler with the desired commands
    // first is the command name/id the second is the command struct
    // you can set new commands like a normal map with handler.cmds["newcmd"] = data;
    CommandHandler handler =
		{
			{"echo", echo}
		};

    // gets system arguments into a vector. alternatively you can use Flags::clean_args instead if you're using this with the flag parser
    Args args = cli::vec_args(argc, argv);

    if (args.empty())
        return 0;

    std::string_view cmd_name = args[0];

	// removes command name from args
    args.erase(args.begin());

    // result returns a struct containing a command status as well as a command message
    // the message will always be a nullptr if the command did not run
    // always check with Result::ok
    auto result = handler.run(cmd_name, args);

    if (!result.ok)
        std::cout << "Error running command: " << result.message;

    return 0;
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
