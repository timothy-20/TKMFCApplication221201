#pragma once
#include <cstring>
#include <iostream>

const char* Reverse(const char* text)
{
	char* result(new char[std::strlen(text) + 2]);
	int index(0);
	int wordLength(0);

	do
	{
		char next(text[index + 1]);

		if ((next == ' ' || next == '\0'))
		{
			if (wordLength < 1)
			{
				result[index] = text[index];
				continue;
			}

			int firstWordIndex(index - wordLength);

			for (int i(0); (index - i) >= firstWordIndex; i++)
				result[firstWordIndex + i] = text[index - i];

			wordLength = 0;
		}
		else
		{
			if (text[index] != ' ') wordLength++;
			result[index] = text[index];
		}
	} while (text[++index] != '\0');

	result[index] = '\0';

	return result;
}

// Test code
//int n(0);
//
//std::cin >> n;
//std::cin.ignore();
//
//while (n--)
//{
//	char ch[1000];
//
//	std::cin.getline(ch, 1000, '\n');
//	std::cout << ::Reverse(ch) << '\n';
//}