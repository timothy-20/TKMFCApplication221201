#define _CTRDBG_MAP_ALLOC 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <crtdbg.h>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>

//#ifdef _DEBUG
//	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif // _DEBUG

typedef struct __ONLY_DOUBLE
{
	template<typename T>
	void getDouble(T) = delete;
	void getDouble(double d)
	{
		std::cout << "Get double: " << d << std::endl;
	}

} TKOnlyDouble;

std::string FormatWithChar(const char* format, ...) noexcept
{
	int fn(0);
	int n(static_cast<int>(strlen(format) * 2));

	std::unique_ptr<char[]> formatted{};
	va_list ap{};

	while (true)
	{
		formatted.reset(new char[n]);

		strcpy_s(&formatted[0], n, format);
		va_start(ap, format);

		fn = vsnprintf(&formatted[0], n, format, ap);

		va_end(ap);

		if (fn < 0 || fn >= n)
		{
			n += abs(fn - n + 1);
		}
		else
		{
			break;
		}
	}

	return std::string(formatted.get());
}

//: public std::enable_shared_from_this<TKElement>
class TKElement 
{
	using TKElementContainer = std::vector<std::shared_ptr<TKElement>>;

private:
	uint16_t m_id;
	TKElementContainer m_subelements;

public:
	TKElement() : m_id(0) { std::cout << "생성 수행. 기본 객체 식별 번호 할당." << std::endl; }
	TKElement(const uint16_t id) : 
		m_id(id), 
		m_subelements(TKElementContainer())
	{
		std::cout << "생성 수행. 현재 객체의 식별 번호 : " << this->m_id << std::endl;
	}
	TKElement(const TKElement& rElement)
	{
		for (auto spElement : rElement.m_subelements)
			this->m_subelements.push_back(std::make_shared<TKElement>(*spElement));

		this->m_id = uint16_t(rElement.m_id) + 1;

		std::cout << "복사 생성 수행" << std::endl;
	}
	
	//복사 할당 생성자 구현할 것.
	TKElement& operator=(const TKElement& rElement)
	{
		if (this != &rElement)
		{
			std::cout << "복사 할당 수행" << std::endl;
		}

		return *this;
	}

	//이동 생성자 및 이동 할당 생성자 구현할 것.

	~TKElement()
	{
		std::cout << "소멸 수행 객체의 식별 번호 : " << this->m_id << std::endl;
	}

	//Utils
	void AddSubelement(std::shared_ptr<TKElement> spElement)
	{
		this->m_subelements.push_back(spElement);
	}
};

//Linked songs 구현 중.
typedef struct __NAMED_NODE
{
	uint16_t m_id;
	void* m_data;
	std::shared_ptr<__NAMED_NODE> m_nextNode;

	__NAMED_NODE(uint16_t id, void* data = nullptr, std::shared_ptr<__NAMED_NODE> nextNode = nullptr) :
		m_id(id),
		m_data(data),
		m_nextNode(nextNode)
	{
		std::cout << "기본 생성 수행. 객체 식별 번호: " << this->m_id << std::endl;
	}
	virtual ~__NAMED_NODE()
	{
		std::cout << "소멸 수행. 객체 식별 번호: " << this->m_id << std::endl;
	}

} TKNamedNode;

using TKSharedNamedNode = std::shared_ptr<TKNamedNode>;

template<typename T>
class TKNamedNodeList
{
private:
	TKSharedNamedNode m_head;

public:
	size_t size;

	TKNamedNodeList(size_t size = 0) : 
		size(size),
		m_head(std::make_shared<TKNamedNode>(0))
	{
		if (size > 0)
		{
			TKSharedNamedNode temp = this->m_head;

			for (uint16_t i(0); i < (size - 1); i++)
			{
				TKSharedNamedNode newNode(std::make_shared<TKNamedNode>(i + 1));

				if (i == 0)
				{
					this->m_head->m_nextNode = newNode;
					temp = this->m_head->m_nextNode;
					continue;
				}
				
				temp->m_nextNode = newNode;
				temp = temp->m_nextNode;
			}
		}
	}

	// 1. push_back
	// 2. erase
	// 3. insert
	// 4. at

	void PushBack(T data)
	{
		TKSharedNamedNode temp(this->m_head);

		while (true)
		{
			if (temp->m_nextNode == nullptr)
			{
				TKSharedNamedNode newNode(std::make_shared<TKNamedNode>(temp->m_id + 1));
				temp->m_nextNode = newNode;

			}

			temp = temp->m_nextNode;
		}
	}

	void* TestCycle(uint32_t index)
	{
		if (index > this->size)
			return nullptr;

		if (index == 0)
			return this->m_head->m_data;

		uint32_t count(1);
		TKSharedNamedNode temp(this->m_head->m_nextNode);
		
		while (temp != nullptr)
		{
			if (index == count)
				return temp->m_data;

			temp = temp->m_nextNode;
			count++;
		}
	}
};

template<typename T>
struct TKArrayListNode
{
private:
	T* value;

public:
	TKArrayListNode() : value(nullptr) { }
	TKArrayListNode(const T& value) : value(new T(value)) { }
	TKArrayListNode(const TKArrayListNode& other) : value(new T(*other.value))
	{
		std::cout << "복사 생성자 호출." << std::endl;
	}
	TKArrayListNode(TKArrayListNode&& other) noexcept : value(std::exchange(other.value, nullptr))
	{
		std::cout << "이동 생성자 호출." << std::endl;
	}
	~TKArrayListNode() { delete this->value; }
	TKArrayListNode& operator=(const TKArrayListNode& other)
	{
		std::cout << "복사 할당 연산자 호출." << std::endl;

		delete this->value;
		this->value = new T(*other.value);

		return *this;
	}
	TKArrayListNode& operator=(TKArrayListNode&& other) noexcept
	{
		std::cout << "이동 할당 연산자 호출." << std::endl;

		if (&other != this)
		{
			delete this->value;
			this->value = std::exchange(other.value, nullptr);
		}

		return *this;
	}

	// Utils
	void SetValue(const T& value)
	{
		if (this->value != nullptr)
			delete this->value;

		this->value = new T(value);
	}
	T GetValue() const
	{
		return *this->value;
	}
};

// 객체의 타입 비교
// if (std::is_same_v<decltype(other.value), decltype(this->value)>)

struct TKRegularCopare { bool operator()(int n1, int n2) { return (n1 > n2); } };
struct TKReverceCompare { bool operator()(int n1, int n2) { return (n1 < n2); } };

// Memory pool implementation
#include <Windows.h>
template <typename T, int ALLOC_BLOCK_SIZE = 50>
class TKMemoryPool
{
public:
	static UCHAR* m_freePtr;

	static VOID AllocBlock()
	{
		m_freePtr = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		UCHAR** current(reinterpret_cast<UCHAR**>(m_freePtr));
		UCHAR* next(m_freePtr);

		for (INT i(0); i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			next += sizeof(T);
			*current = next;
			current = reinterpret_cast<UCHAR**>(next);
		}

		*current = 0;

		std::cout << "[!] freePtr: " << (int)m_freePtr << std::endl;
	}

	~TKMemoryPool() = default;

	static VOID* operator new(std::size_t allocLength)
	{
		if (m_freePtr == nullptr)
			AllocBlock();

		UCHAR* returnPtr(m_freePtr);
		m_freePtr = *reinterpret_cast<UCHAR**>(returnPtr);

		std::cout << "[!] returnPtr: " << (int)returnPtr << std::endl;
		std::cout << "[!] freePtr: " << (int)m_freePtr << std::endl;

		return returnPtr;
	}
	static VOID operator delete(VOID* deletePtr)
	{
		*reinterpret_cast<UCHAR**>(deletePtr) = m_freePtr;
		m_freePtr = static_cast<UCHAR*>(deletePtr);
	}
};

template <typename T, int ALLOC_BLOCK_SIZE>
UCHAR* TKMemoryPool<T, ALLOC_BLOCK_SIZE>::m_freePtr;

#include "TKMeasureTime.h"
#include "TKArrayList.hpp"

template <typename T>
struct TKNode
{
	T m_value;
	TKNode<T>* m_next;

	TKNode(const T& value) :
		m_value(value),
		m_next(nullptr)
	{}
};

template <typename T>
class TKForwardList
{
private:
	std::shared_ptr<TKNode<T>> front;

public:
	TKForwardList() : front(nullptr)
	{}

	//Utils
	void PushForward(const T& value)
	{
		std::shared_ptr<TKNode<T>> node(std::make_shared<TKNode<T>>(value));
		
		if (this->front == nullptr)
		{
			this->front.swap(node);
			return;
		}

		TKNode<T>* refNode(node.get());
		TKNode<T>* refFront(this->front.get());

		refNode->m_next = refFront;
		refFront = refNode;
	}
};

void GetAdjacencyMatrix(int node, int edge)
{
	int* adj[100];

	for (int i(0); i < 100; i++)
		adj[i] = new int[100];

	for (int i(0); i < edge; i++)
	{
		int n1(0), n2(0);

		std::cout << "insert coordinates: ";
		std::cin >> n1 >> n2;

		adj[n1][n2] = 1;
		adj[n2][n1] = 1;
	}

	for (int i(0); i < node; i++)
	{
		for (int j(0); j < node; j++)
		{
			if (adj[i][j] != 1)
				adj[i][j] = 0;

			std::cout << adj[i][j] << " ";
		}

		std::cout << '\n';
	}

	for (int i(0); i < 100; i++)
		delete adj[i];
}

#include <random>
#include <list>

template <typename T>
auto FisherYatesShuffle(const std::vector<T>& list)
{
	std::vector<T> result(list.size());
	std::random_device rd;
	std::mt19937_64 mt(rd());
	
	for (auto iterator(result.begin()); iterator != result.end(); iterator++)
	{
		std::uniform_int_distribution<int> distribution(0, list.size() - 1);
		auto point(list.begin() + distribution(mt));

		std::iter_swap(iterator, point);
		list.erase(point);
	}

	return result;
}

template <typename T>
void DurstenfeldShuffle(std::vector<T>& list)
{
	std::random_device rd;
	std::mt19937_64 mt(rd());
	int count(list.size());

	for (int i(1); i <= list.size(); i++)
	{
		std::uniform_int_distribution<int> distribution(0, --count);
		auto point(list.begin() + distribution(mt));

		std::iter_swap(point, list.end() - i);
	}
}

template <typename T>
auto MultiShuffle(const std::vector<T>& list)
{
	std::vector<T> result(list.size());
	std::random_device rd;
	std::mt19937_64 mt(rd());
	std::uniform_int_distribution<int> distribution(0, list.size() - 1);

	for (auto iterator(result.begin()); iterator != result.end(); iterator++)
		*iterator = list[distribution(mt)];

	return result;
}

struct TKSong
{
	uint32_t genre;
	uint32_t playTime;

	TKSong() = default;
	TKSong(uint32_t genre, uint32_t playTime) :
		genre(genre),
		playTime(playTime)
	{}
	TKSong(const TKSong& other) :
		genre(0),
		playTime(0)
	{
		*this = other;
	}
	TKSong(TKSong&& other) noexcept :
		genre(0),
		playTime(0)
	{
		*this = std::move(other);
	}
	TKSong& operator=(const TKSong& other)
	{
		if (this != &other)
		{
			this->genre = other.genre;
			this->playTime = other.playTime;
		}

		return *this;
	}
	TKSong& operator=(TKSong&& other)
	{
		if (this != &other)
		{
			this->genre = std::exchange(other.genre, 0);
			this->playTime = std::exchange(other.playTime, 0);
		}

		return *this;
	}
};

struct TKPlayList
{
private:
	std::vector<TKSong> m_songs;
	uint32_t m_totalPlayTime;

public:
	TKPlayList() :
		m_songs(std::vector<TKSong>()),
		m_totalPlayTime(0)
	{}
	TKPlayList(std::vector<TKSong> songs) : TKPlayList()
	{
		for (auto iterator(songs.cbegin()); iterator != songs.cend(); iterator++)
			this->AddSong(*iterator);
	}

	// Utils
	void AddSong(const TKSong& song)
	{
		this->m_songs.push_back(song);
		this->m_totalPlayTime += song.playTime;
	}
	std::vector<TKSong> GetPlayList() const
	{
		return this->m_songs;
	}
	uint32_t GetTotalPlayTime() const
	{
		return this->m_totalPlayTime;
	}
};

class TKPlayListShuffler
{
private:
	std::vector<TKSong> m_songs;
	std::vector<TKPlayList> m_patterns;

	void PlayListPatternsPermutation(int s = 0)
	{
		size_t size(this->m_songs.size());

		if (s == size)
		{
			this->m_patterns.push_back(this->m_songs);
			return;
		}

		for (int i(s); i < size; i++)
		{
			std::swap(this->m_songs[s], this->m_songs[i]);
			this->PlayListPatternsPermutation(s + 1);
			std::swap(this->m_songs[s], this->m_songs[i]);
		}
	}

	void PlayListPatternsCombination(std::vector<TKSong> combination, int index = 0)
	{
		if (combination.size() == index)
		{
			this->m_patterns.push_back(combination);
			return;
		}

		for (int i(0); i < this->m_songs.size(); i++)
		{
			combination[index] = this->m_songs[i];

			this->PlayListPatternsCombination(combination, index + 1);
		}
	}

	void MakeRandomPlayCondition()
	{

	}

public:
	TKPlayListShuffler(const TKPlayList& totalPlayList) : m_songs(totalPlayList.GetPlayList())
	{}

	// Utils
	std::vector<TKPlayList> PatternWithTotalPlayTime(int totalPlayTime)
	{
		for (int i(1); i < this->m_songs.size(); i++)
		{
			std::vector<TKSong> pattern(i);

			this->PlayListPatternsCombination(pattern);
		}

		this->m_patterns.erase(std::remove_if(this->m_patterns.begin(), this->m_patterns.end(), [totalPlayTime](TKPlayList playList) -> bool
			{
				return playList.GetTotalPlayTime() != totalPlayTime;
			}), this->m_patterns.end());

		std::vector<TKPlayList> specificPattern(this->m_patterns);

		this->m_patterns.clear();

		return specificPattern;
	}

	// 테스트 용 함수
	void PrintAllPatterns()
	{
		for (auto playList : this->m_patterns)
		{
			std::cout << "Play list: " << std::endl;

			for (auto song : playList.GetPlayList())
				std::cout << song.genre << ' ' << song.playTime << std::endl;

			std::cout << '\n';
		}
	}
};

void Run()
{
	//int songsNumber;

	//std::cout << "Number of songs: ";
	//std::cin >> songsNumber;

	//for (int i; i < songsNumber; i++)
	//{
	//	int n1, n2;

	//	std::cout << " Genre and length: ";
	//	std::cin >> n1 >> n2;

	//}

	//std::vector<TKPlayList> result;
	//int a, b;

	//std::cout << "range: ";
	//std::cin >> a >> b;

	//for (; a <= b; a++)
	//{
	//	auto patterns(shuffler.PatternWithTotalPlayTime(a));

	//	result.insert(result.end(), patterns.begin(), patterns.end());
	//}
}

int main()
{
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	TKPlayList playList;

	playList.AddSong(TKSong(0, 2));
	playList.AddSong(TKSong(0, 3));
	playList.AddSong(TKSong(1, 2));
	
	TKPlayListShuffler shuffler(playList);
	std::vector<TKPlayList> result;
	int a, b;

	std::cout << "range: ";
	std::cin >> a >> b;

	for (; a <= b; a++)
	{
		auto patterns(shuffler.PatternWithTotalPlayTime(a));

		result.insert(result.end(), patterns.begin(), patterns.end());
	}

	std::cout << result.size() << std::endl;
	std::cout << std::endl;

	return 0;
}

// 23.01.02. test code_15.03 =========================================================================
//std::vector<std::string> l1{ "A1", "B1", "A2" };
////auto result(::FisherYatesShuffle(l1));
////::DurstenfeldShuffle(l1);
//auto result(::MultiShuffle(l1));

//std::cout << "seed: ";

//for (auto element : l1)
//	std::cout << element << ' ';

//std::cout << "\nresult: ";

//for (auto element : result)
//	std::cout << element << ' ';

// 22.12.30. test code_20.19 =========================================================================
//TKForwardList<std::string> songs;

//songs.PushForward("timothy");
//songs.PushForward("peco");
//songs.PushForward("lay");

// 22.12.30. test code_13.43 =========================================================================
//TKArrayList<std::string> songs(3);

//TKMeasureTime::PrintWithMicroseconds([&songs]() -> void {
//	// 지정 capacity보다 많은 element를 적재할 경우
//	songs.AddElement("timothy");
//	songs.AddElement("peco");
//	songs.AddElement("ray");
//	songs.AddElement("another one");

//	// 배열 리스트의 1번에 element를 삽입한 경우.
//	songs.InsertElement(1, "i'm no.2");

//	// 지정 capacity보다 많은 element를 삽입한 경우.
//	songs.InsertElement(2, "i'm no.3");
//	songs.InsertElement(3, "i'm no.4");
//	songs.InsertElement(4, "i'm no.5");

//	// 특정 위치의 element를 제거한 경우.
//	songs.RemoveElement(2);

//	std::cout << std::endl;
//});

//TKMeasureTime::PrintWithMicroseconds([&songs]() -> void {
//	for (int i(0); songs[i].compare(""); i++)
//		std::cout << "result: " << songs[i] << std::endl;
//});