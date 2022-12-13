#pragma once
#include <iostream>

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

