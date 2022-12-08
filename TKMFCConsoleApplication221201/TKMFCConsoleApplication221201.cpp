#define _CTRDBG_MAP_ALLOC 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <crtdbg.h>

#include <vector>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG


//enum TKErrorType{
//	TKErrorNullPointer = 0x00,
//	TKErrorOutOfRange,
//} ;

template<typename TYPE, size_t LENGTH>
class TKDummyList
{
private:
	TYPE m_list[LENGTH];

public:
	struct Iterator
	{
		using IteratorCategory = std::forward_iterator_tag;
		using DifferenceType = std::ptrdiff_t;
		using GenericType = TYPE;
		using PointerType = TYPE*;
		using ReferenceType = TYPE&;

	private:
		PointerType m_ptr;

	public:
		Iterator(PointerType ptr) : m_ptr(ptr) {}
		//접두사에 붙은 증감연산자
		Iterator& operator++() 
		{ 
			this->m_ptr++; 
			return *this;
		}
		//접미사에 붙은 증감연산자
		Iterator operator++(int)
		{
			Iterator weakThis = *this;
			++(*this);
			return weakThis;
		}
		ReferenceType operator*() const { return *this->m_ptr; }
		PointerType operator->() { return this->m_ptr; }
		friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
		friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }
	};

	Iterator begin() { return Iterator(&this->m_list[0]); }
	Iterator end() { return Iterator(&this->m_list[LENGTH]); }
};

int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	/*int* npArray[10] = {};
	std::fill(std::begin(npArray), std::end(npArray), new int(1));

	for (const int* element : npArray)
	{
		std::cout << "element: " << element << std::endl;
	}*/

	TKDummyList<int, 10> dummyList;

	std::fill(dummyList.begin(), dummyList.end(), 10);

	for (auto element : dummyList)
		std::cout << "1. element: " << element << std::endl;

	std::cout << std::endl;

	for (auto begin = dummyList.begin(), end = dummyList.end(); begin != end; begin++)
		std::cout << "2. element: " << *begin << std::endl;

	std::string text = std::string("abcde");
	const char* appendChar = "EEEE";
	
	text.append(appendChar);

	const char* cText = text.c_str();

	_CrtDumpMemoryLeaks();
	return 0;
}