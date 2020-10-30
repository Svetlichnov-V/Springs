#include <iostream>

int strLen(char* str)
{
	int i = 0;
	while (true)
	{
		if (str[i] == 0)
			return i;
		i++;
	}
}

int main()
{
	char str[] = "My sfhcrt";
	std::cout << strLen(str);
	return 0;
}