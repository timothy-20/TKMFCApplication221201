#pragma once
#include <iostream>

class TKStack
{
private:
	int* m_bucket;
	int m_size;

public:
	TKStack() :
		m_bucket(new int[10001] {0, }),
		m_size(0)
	{}
	~TKStack()
	{
		delete[] this->m_bucket;
		this->m_size = 0;
	}

	void Push(int value)
	{
		this->m_bucket[this->m_size++] = value;
	}
	int Pop()
	{
		if (this->Empty())
			return -1;

		return this->m_bucket[--this->m_size];
	}
	int Size() const
	{
		return this->m_size;
	}
	bool Empty() const
	{
		return this->m_size > 0 ? false : true;
	}
	int Top()
	{
		return this->Empty() ? -1 : this->m_bucket[this->m_size - 1];
	}
};

// Test code
// 
//TKStack stack;
//int n(0);
//std::cin >> n;
//
//while (n--)
//{
//	std::string query("");
//	int value(0);
//	int result(0);
//
//	std::cin >> query;
//
//	if (query.compare("push") == 0)
//	{
//		std::cin >> value;
//		stack.Push(value);
//	}
//	else if (query.compare("pop") == 0)
//		std::cout << stack.Pop() << '\n';
//
//	else if (query.compare("size") == 0)
//		std::cout << stack.Size() << '\n';
//
//	else if (query.compare("empty") == 0)
//		std::cout << stack.Empty() << '\n';
//
//	else if (query.compare("top") == 0)
//		std::cout << stack.Top() << '\n';
//}