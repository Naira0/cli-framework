#include <vector>
#include <string>
#include <sstream>
#include <type_traits>

namespace vec_util
{
	template<typename T>
	void reverse(std::vector<T>& arr)
	{
		int fromStart = 0;
		int fromEnd = arr.size() - 1;

		while (fromStart < fromEnd)
		{
			T temp = arr[fromStart];

			arr[fromStart] = arr[fromEnd];
			arr[fromEnd] = temp;

			fromStart++;
			fromEnd--;
		}
	}

	template<typename T>
	std::string format(const std::vector<T>& vec)
	{
		if (vec.empty())
			return "[]";

		std::string result;

		const char* open = "[ ";
		const char* close = " ]";
		const char* comma = ", ";

		size_t length = vec.size() - 1;

		for (int i = 0; i <= length; i++)
		{
			std::stringstream stream;

			stream << vec[i];

			const std::string element = stream.str();

			if (i == 0)
				result += open + element + comma;
			else if (i == length)
				result += element + close;
			else
				result += element + comma;
		}

		return result;
	}

	std::vector<std::string> split(const std::string& str, char seperator = ' ')
	{
		std::vector<std::string> result;
		std::string store;

		int i = 0;
		for (const char c : str)
		{
			i++;

			if (c == seperator)
			{
				result.push_back(store);
				store.clear();

				continue;
			}

			store += c;

			if (i == str.size())
				result.push_back(store);
		}

		return result;
	}

	template<typename T>
	std::string join(const std::vector<T>& vec, const char* symbol = "")
	{
		static_assert(
			std::is_integral<T>::value			||
			std::is_same<T, char>::value		||
			std::is_same<T, std::string>::value ||
			std::is_same<T, const char*>::value, "Invalid template type provided. valid types are strings and intergral types");

		std::stringstream result;

		int i = 0;
		size_t size = vec.size();

		for (const auto& element : vec)
		{
			i++;
			result << element;

			if (i != size)
				result << symbol;
		}

		return result.str();
	}

	// cpu time is identical to using something like find from the algorithem library 
	// but it depends on the compiler. compiling it with GCC it was identical. on clang it was 1 times slower
	template<typename T>
	bool includes(const std::vector<T>& vec, T value)
	{
		for (const auto& element : vec)
			if (element == value)
				return true;

		return false;
	}
}

