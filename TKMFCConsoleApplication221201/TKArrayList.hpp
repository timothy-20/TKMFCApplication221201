#pragma once
#include <iostream>

template<typename T>
class TKArrayList
{
private:
	size_t m_length;
	size_t m_capacity;
	T* m_container;

public:
	TKArrayList(int capacity = 1) :
		m_length(0),
		m_capacity(capacity),
		m_container(nullptr)
	{
		if (this->m_capacity > 0)
			this->m_container = new T[this->m_capacity];
	}
	TKArrayList(const TKArrayList& other) :
		m_length(other.m_length),
		m_capacity(other.m_capacity)
	{
		this->m_container = new T[this->capacity];

		for (int i(0); i < other.m_length; i++)
			this->m_container[i] = *other.m_container[i];
	}
	virtual ~TKArrayList()
	{
		delete[] this->m_container;
	}

	// Utils
	T operator[](size_t index) const
	{
		return this->m_container[index];
	}
	bool AddElement(const T& value)
	{
		this->m_length++;

		// 적재된 값의 갯수가 list의 범위를 넘어선 경우
		if (this->m_length > this->m_capacity)
			this->ExpandCapacity();

		this->m_container[this->m_length - 1] = value;

		return true;
	}
	bool InsertElement(size_t index, const T& value)
	{
		if (index > this->m_capacity)
			return false;

		this->m_length++;

		if (this->m_length > this->m_capacity)
			this->ExpandCapacity();

		T temp;

		for (int i(0); i < this->m_length; i++)
		{
			if (i >= index)
				std::swap(temp, this->m_container[i]);
		}

		this->m_container[index] = value;

		return true;
	}
	// 삭제 Utils 함수 구현
	bool RemoveElement(size_t index)
	{
		if (index > this->m_capacity)
			return false;

		for (int i(0); i < this->m_length; i++)
		{
			if (i >= index)
				std::swap(this->m_container[i], this->m_container[i + 1]);
		}

		T temp;
		std::swap(temp, this->m_container[--this->m_length]);

		return true;
	}

	void ExpandCapacity()
	{
		this->m_capacity *= 2;

		T* newNodeList(new T[this->m_capacity]);

		for (int i(0); i < (this->m_length - 1); i++)
			newNodeList[i] = this->m_container[i];

		delete[] this->m_container;
		this->m_container = newNodeList;
	}
};
