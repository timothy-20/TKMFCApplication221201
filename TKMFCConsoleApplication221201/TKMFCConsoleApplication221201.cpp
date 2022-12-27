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
#include <time.h>
#include <chrono>
#include <iomanip>

#ifdef _DEBUG
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

typedef struct __ONLY_DOUBLE
{
	template<typename T>
	void getDouble(T) = delete;
	void getDouble(double d)
	{
		std::cout << "Get double: " << d << std::endl;
	}

} TKOnlyDouble;

typedef enum _TKNormalEnum : char
{
	TKLetterA = 'a',
	TKLetterB = 'b',
	TKLetterC = 'c'

} TKNormalEnum;

enum class TKUserType : char
{
	TKNormalUser = 'N',
	TKPremiumUser = 'P',
	TKAdmin = 'A'
};

//typedef struct __USER_INFO
//{
//private:
//	TKUserType m_castType{ TKUserType::TKNormalUser };
//
//public:
//	uint16_t id;
//	const char* name;
//	const char* description;
//
//	char castTypeC() const { return static_cast<char>(this->m_castType); }
//
//	__USER_INFO() :
//		id(0),
//		name(""),
//		description("no description")
//	{
//	}
//	__USER_INFO(TKUserType castType) : __USER_INFO()
//	{
//		this->m_castType = castType;
//	}
//
//	static __USER_INFO GetDefaultUser() { return { TKUserType::TKNormalUser }; }
//
//} TKUserInfo;

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

typedef struct __USER_INFO
{
private:
	uint16_t m_userId;
	bool m_isPrivate;

public:
	const char* userName{ "" };
	const char* userDescription{ "no descript." };

	__USER_INFO() = default;
	__USER_INFO(uint16_t userId, bool isPrivate) : m_userId(userId), m_isPrivate(isPrivate) 
	{ 
		std::cout << "Create __USER_INFO: " << this->m_userId << std::endl; 
	}

	virtual ~__USER_INFO() 
	{ 
		std::cout << "Destroy __USER_INFO: " << this->m_userId << std::endl; 
	}

	virtual void ShowSummaryInfo()
	{
		std::cout << ::FormatWithChar("[INFO] %d : This user name is '%s'", this->m_userId, this->userName) << std::endl;
	}

} TKUserInfo;

typedef struct __PREMIUM_USER_INFO final : public TKUserInfo
{
private:

public:
	__PREMIUM_USER_INFO(uint16_t userId, bool isPrivate) : TKUserInfo(userId, isPrivate)
	{

	}

	void ShowSummaryInfo() override
	{
		TKUserInfo::ShowSummaryInfo();

		std::cout << ::FormatWithChar("[INFO]") << std::endl;
	}

} TKPremiumUserInfo;

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

//Linked list 구현 중.
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

// std::bind를 이용한 함수 호출.
//void TestFunction(const std::string &text1, const std::string& text2)
//{
//	std::cout << "1. " << text1 << std::endl;
//	std::cout << "2. " << text2 << std::endl;
//}

//auto testFunction = std::bind(::TestFunction, std::placeholders::_2, std::placeholders::_1);
//
//testFunction("Hello, timothy", "I'm peco!");

using TKNameList = std::shared_ptr<std::vector<std::string>>;

class TKDummy
{
private:
	//TKNameList names;
	
public:
	std::string name;
	std::vector<std::string>* names;

	explicit TKDummy() : 
		name(""),
		names(new std::vector<std::string>)
	{
		std::cout << "기본 생성자 호출" << std::endl;
	}
	explicit TKDummy(std::initializer_list<std::string> list) : 
		name(*list.begin()),
		names(new std::vector<std::string>)
	{
		std::cout << "유니폼 초기화 생성자 호출" << std::endl;

		for (auto iter(list.begin() + 1); iter != list.end(); iter++)
			this->names->push_back(*iter);
	}
	//name(std::move(other.name)),
	//names(std::exchange(other.names, std::move(nullptr)))
	
	TKDummy(TKDummy&& other) noexcept :
		name(std::move(other.name)),
		names(std::exchange(other.names, nullptr))
	{
		std::cout << "이동 생성자 호출." << std::endl;
	}
	//TKDummy(TKDummy&& other) noexcept :
	//	name(""),
	//	names(nullptr)
	//{
	//	std::cout << "이동 생성자 호출." << std::endl;

	//	*this = std::move(other);
	//}
	//TKDummy& operator=(TKDummy&& other) noexcept
	//{
	//	std::cout << "이동 할당 생성자 호출." << std::endl;

	//	if (this != &other)
	//	{
	//		delete this->names;

	//		this->name = other.name;
	//		this->names = other.names;
	//		other.name = "";
	//		other.names = nullptr;
	//	}

	//	return *this;
	//}
	~TKDummy()
	{
		std::cout << "소멸자 호출. 객체의 주소 값: " << this << std::endl;

		if (this->names != nullptr)
		{
			std::cout << "소멸자 호출. 'names' 객체 제거. 객체의 주소 값: " << this << std::endl;
			delete this->names;
		}
	}
};

// typedef(혹은 using)으로 정의한 const 타입
typedef const std::string TKConstString;
typedef const char* TKConstChar;

void PrintText(TKConstString& text)
{
	std::cout << text << std::endl;
}

void PrintCharArray(TKConstChar& charArray)
{
	std::cout << charArray << std::endl;
}

//void DoWork(const int& n)
//{
//	const_cast<int&>(n) = 100;
//}
//
//int a(1);
//::DoWork(a);
//
//std::cout << a;

class TKClass
{
public:
	int id;
	
	TKClass(int id) : id(id) {}
	TKClass operator+(const TKClass& other)
	{
		std::cout << "사용자 정의 덧셈 연산자 수행." << std::endl;

		this->id += other.id;
		return *this;
	}
};

void TestCodes221221()
{
	//::PrintText("ABCDE");
	//::PrintCharArray("ABCDE");

	char name[5]{ 112, 101, 99, 111, '\0' };

	std::cout << "result: " << name << std::endl;

	int ipe(13680); // little endian에서 읽을 수 있는 "pe"의 형태를 int type으로 수치화한 것입니다.
	char cpe(ipe); // 이를 다시 char type으로 downcast하여 첫 글짜가 'p'가 맞는지 확인합니다.

	std::cout << "result: " << cpe << std::endl;

	TKClass class1(1);
	TKClass class2(2);
	const TKClass& cRefClass = class1 + class2;

	std::cout << "result: " << cRefClass.id << std::endl;

	//TKDummy dummy1{ "timothy", "peco", "ray" };
	//
	//std::cout << "dummy1's address: " << &dummy1 << std::endl;

	//TKDummy dummy2(std::move(dummy1));

	//std::cout << "dummy1\'s address: " << &dummy1 << std::endl;
	//std::cout << "dummy2\'s address: " << &dummy2 << std::endl;

	//std::vector<std::string> container{ "A", "B", "C" };
	//
	//container.push_back(std::move(std::string("timothy")));
	//container.push_back(std::move(std::string("peco")));

	//std::string newName("");
	//std::swap(container.at(1), newName);
	//
	//std::cout << "New name: " << newName << std::endl;

	//std::for_each(container.cbegin(), container.cend(), )

	//테스트 중
	//TKNamedNodeList<std::string> list(5);
	//list.TestCycle(3);

	//TKCopying a(1);
	//TKCopying b(a);

	// 테스트를 위한 other data.
	//
	//using TKDoubleContainer = std::vector<std::vector<int>>;
	//
	//constexpr size_t size(5);
	//TKDoubleContainer container(0);
	//
	//for (int count(1); count <= size; count++)
	//	container.push_back(std::vector<int>(count));
	//
	//std::vector<int> sizeContainer(size);

	// funtion 객체 생성을 통한 멤버함수 이용.
	//std::function<size_t(const std::vector<int>&)>functional = &std::vector<int>::size;

	//std::transform(
	//	container.cbegin(),
	//	container.cend(),
	//	sizeContainer.begin(),
	//	functional
	//);

	// function 객체를 리턴하는 함수 'std::mem_fn'을 사용.
	//std::transform(
	//	container.cbegin(),
	//	container.cend(),
	//	sizeContainer.begin(),
	//	std::mem_fn(std::vector<int>::size)
	//);

	//std::for_each(sizeContainer.cbegin(), sizeContainer.cend(), )
}


// Test code - try.1
// 
//void TestBufferOverrun()
//{
//	constexpr size_t capacity(5);
//	int* integerList1(new int[capacity] {1, 2, 3, 4, 5});
//
//	// 배열 포인터
//	//int normalList[5]{ 1, 2, 3, 4, 5 };
//	//int(*integerList2)[5](&normalList);
//
//	integerList1[6] = 2;
//
//	for (int i(0); i <= capacity; i++)
//	{
//		std::cout << integerList1[i] << std::endl;
//	}
//}
//
//using TKStringNode = TKArrayListNode<std::string>;
//
//TKStringNode* nodeList(new TKStringNode[3]);
//
//nodeList[0] = TKStringNode("timothy");
//nodeList[1] = TKStringNode("peco");
//nodeList[2] = TKStringNode("ray");
//
//nodeList[1].SetValue("another peco");
//
//for (int i(0); i < 3; i++)
//{
//	std::cout << "value: " << nodeList[i].GetValue() << std::endl;
//}
//
//delete[] nodeList;

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

// 객체의 타입 비교
// if (std::is_same_v<decltype(other.value), decltype(this->value)>)

namespace TKMeasureTime
{
	void PrintWithSeconds(std::function<void()> callback)
	{
		clock_t start(clock());
		callback();
		std::cout << "실행에 소요한 초: " << ((int)clock() - start) / (CLOCKS_PER_SEC / 1000) << "'s" << std::endl;
	}

	using TKChronoSystemClock = std::chrono::system_clock;
	template<typename TYPE, typename PERIOD = std::ratio<1>> using TKChronoDuration = std::chrono::duration<TYPE, PERIOD>;

	void PrintWithMicroseconds(std::function<void()> callback)
	{
		TKChronoSystemClock::time_point startPoint(TKChronoSystemClock::now());

		callback();

		TKChronoSystemClock::time_point endPoint(TKChronoSystemClock::now());
		double sDuration(TKChronoDuration<double>(endPoint - startPoint).count());
		double mDuration(TKChronoDuration<double, std::milli>(endPoint - startPoint).count());

		std::cout << std::setprecision(7);
	 	std::cout << std::fixed; // std::cout.unsetf(std::ios::scientific);
		std::cout << "실행에 소요한 초: " << sDuration << "(s)" << std::endl;
		std::cout << std::setprecision(2);
		std::cout << "실행에 소요한 밀리초: " << mDuration << "(ms)" << std::endl;
	}
}

void PrintNowTime()
{
	auto now(std::chrono::system_clock::now());
	std::time_t nowTime(std::chrono::system_clock::to_time_t(now));
	char nowTimeChar[30];

	::ctime_s(nowTimeChar, sizeof(nowTimeChar), &nowTime);

	std::cout << "Now time: " << nowTimeChar << std::endl;
}

void PrintNowSummaryTime()
{
	std::time_t time(std::time(0));
	std::tm tm{};

	::localtime_s(&tm, &time);

	std::cout << "Hour: " << (tm.tm_hour - 12) << "h" << std::endl;
	std::cout << "Minute: " << tm.tm_min << "m" << std::endl;
}

std::function<std::chrono::duration<double>&(void)> GetDurationTime()
{
	auto startTime(std::chrono::system_clock::now());
	
	return [=] () -> std::chrono::duration<double>& { 
		std::chrono::duration<double> duration((std::chrono::system_clock::now() - startTime));
		return duration; 
	};
}

long GetFibonacci(uint32_t value)
{
	if (value < 2)
		return value;

	return ::GetFibonacci(value - 1) + ::GetFibonacci(value - 2);
}

#include <thread>

template<typename T, typename... Args>
void Wait(uint32_t seconds, bool isAsync, T&& func, Args... parameters)
{
	std::function<typename std::result_of<T(Args...)>::type()> task(std::bind(std::forward<T>(func), std::forward<Args>(parameters)...));

	if (isAsync)
	{
		std::thread([seconds, task]() -> void
			{
				std::this_thread::sleep_for(std::chrono::seconds(seconds));
				task();
			}
		).detach();

		return;
	}

	std::this_thread::sleep_for(std::chrono::seconds(seconds));
	task();
}






struct TKRegularCopare
{
	bool operator()(int n1, int n2) { return (n1 > n2); }
};

struct TKReverceCompare
{
	bool operator()(int n1, int n2) { return (n1 < n2); }
};

// 예제: https://koreanfoodie.me/841
template <typename Container, typename Compare>
void BubbleSort(Container& container, const Compare compare)
{
	for (int i(0); i < container.size(); i++)
	{

	}
}

template <template<typename ...> class T, typename ...Ts>
struct IsInstanceOf : public std::false_type
{};

template <template<typename ...> class T, typename ...Ts>
struct IsInstanceOf<T, T<Ts...>> : public std::true_type
{};

template <typename T>
struct TKTemplate1 {};

template <typename T>
struct TKTemplate2
{
	static_assert(IsInstanceOf<TKTemplate1, T>::value, "Fail");
};


// template specialization
template <class T>
struct PTS
{
	enum
	{
		IsPointer = 0,
		IsPointerToDataMember = 0
	};
};

template <class T>
struct PTS<T*>
{
	enum
	{
		IsPointer = 1,
		IsPointerToDataMember = 0
	};
};

template <class T, class U>
struct PTS<T U::*> // how to use?
{
	enum
	{
		IsPointer = 0,
		IsPointerToDataMember = 1
	};
};

//TKTemplate2<int> temp1;
//TKTemplate2<TKTemplate1<int>> temp2;

//if (static_cast<bool>(::PTS<TKStorage*>::IsPointer))
//	std::cout << "S is pointer\n";

//if (static_cast<bool>(::PTS<int TKStorage::*>::IsPointerToDataMember))
//	std::cout << "S is pointer to data member\n";

template <typename T>
class TKBag
{
private:
	T* m_element;
	int m_size;
	int m_maxSize;

public:
	TKBag() :
		m_element(new T[1]),
		m_size(0),
		m_maxSize(1)
	{ }
	~TKBag()
	{
		delete[] this->m_element;
	}

	// utils
	void Add(T value)
	{
		T* temp;

		if (this->m_size + 1 > this->m_maxSize)
		{
			this->m_maxSize *= 2;
			temp = new T[this->m_maxSize];

			for (int i(0); i < this->m_size; i++)
				temp[i] = this->m_element[i];

			temp[this->m_size++] = value;
			delete[] this->m_element;
			this->m_element = temp;
			return;
		}

		this->m_element[this->m_size++] = value;
	}
	void Print() const
	{
		for (int i(0); i < this->m_size; i++)
			std::cout << this->m_element[i] << " ";

		std::cout << std::endl;
	}
};

template <typename T>
class TKBag<T*>
{
private:
	T* m_element;
	int m_size;
	int m_maxSize;

public:
	TKBag() :
		m_element(new T[1]),
		m_size(0),
		m_maxSize(1)
	{ }
	~TKBag()
	{
		delete[] this->m_element;
	}

	// utils
	void Add(const T* value)
	{
		T* temp;

		if (value == nullptr)
		{
			std::cout << "Unable to get pointer value." << std::endl;
			return;
		}

		if (this->m_size + 1 > this->m_maxSize)
		{
			this->m_maxSize *= 2;
			temp = new T[this->m_maxSize];

			for (int i(0); i < this->m_size; i++)
				temp[i] = this->m_element[i];

			temp[this->m_size++] = *value;
			delete[] this->m_element;
			this->m_element = temp;
			return;
		}

		this->m_element[this->m_size++] = *value;
	}
	void Print() const
	{
		for (int i(0); i < this->m_size; i++)
			std::cout << this->m_element[i] << " ";

		std::cout << std::endl;
	}

};

//TKBag<char> cBag;
//TKBag<char*> cpBag;
//
//cBag.Add('a');
//cBag.Add('b');
//cBag.Add('c');
//cBag.Add('d');
//
//const char* cp1("timothy");
//const char* cp2("peco");
//const char* cp3("lay");
//
//cpBag.Add(cp1);
//cpBag.Add(cp2);
//cpBag.Add(cp3);
//cBag.Print();
//cpBag.Print();

template <typename KEY, typename VALUE> 
class TKDictionary
{
protected:
	KEY* m_keys;
	VALUE* m_values;
	int m_size;
	int m_maxSize;

public:
	TKDictionary(int size) : 
		m_size(0),
		m_maxSize(1)
	{
		while (size >= this->m_maxSize)
			this->m_maxSize *= 2;

		this->m_keys = new KEY[this->m_maxSize];
		this->m_values = new VALUE[this->m_maxSize];
	}

	virtual ~TKDictionary()
	{
		delete[] this->m_keys;
		delete[] this->m_values;
	}

	// utils
	virtual void Add(KEY key, VALUE value)
	{
		KEY* keyTemp(nullptr);
		VALUE* valueTemp(nullptr);

		if (this->m_size + 1 > this->m_maxSize)
		{
			this->m_maxSize *= 2;
			keyTemp = new KEY[this->m_maxSize];
			valueTemp = new VALUE[this->m_maxSize];

			for (int i(0); i < this->m_size; i++)
			{
				keyTemp[i] = this->m_keys[i];
				valueTemp[i] = this->m_values[i];
			}

			keyTemp[this->m_size] = key;
			valueTemp[this->m_size] = value;
			delete[] this->m_keys;
			delete[] this->m_values;
			this->m_keys = keyTemp;
			this->m_values = valueTemp;
		}
		else
		{
			this->m_keys[this->m_size] = key;
			this->m_values[this->m_size] = value;
		}

		this->m_size++;
	}

	virtual void Print() const
	{
		for (int i(0); i < this->m_size; i++)
			std::cout << "{ " << this->m_keys[i] << ", " << this->m_values[i] << " }" << std::endl;
	}
};

template <typename VALUE>
class TKNumericDictionary : public TKDictionary<int, VALUE>
{
public:
	TKNumericDictionary(int size) : TKNumericDictionary::TKDictionary(size)
	{}

	void BubbleSort()
	{
		int smallest(0);

		for (int i(0); i < this->m_size - 1; i++)
		{
			for (int j(i); j < this->m_size; j++)
			{
				if (this->m_keys[j] < this->m_keys[smallest])
					smallest = j;
			}

			std::swap(this->m_keys[i], this->m_keys[smallest]);
			std::swap(this->m_values[i], this->m_values[smallest]);
		}
	}
};

int main()
{
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	TKNumericDictionary<const char*> dictionary(5);

	dictionary.Add(43, "peco");
	dictionary.Add(26, "timothy");
	dictionary.Add(74, "lay");
	dictionary.BubbleSort();
	dictionary.Print();



	





	//TKArrayList<std::string> list(3);

	//TKMeasureTime::PrintWithMicroseconds([&list]() -> void {
	//	// 지정 capacity보다 많은 element를 적재할 경우
	//	list.AddElement("timothy");
	//	list.AddElement("peco");
	//	list.AddElement("ray");
	//	list.AddElement("another one");

	//	// 배열 리스트의 1번에 element를 삽입한 경우.
	//	list.InsertElement(1, "i'm no.2");

	//	// 지정 capacity보다 많은 element를 삽입한 경우
	//	list.InsertElement(2, "i'm no.3");
	//	list.InsertElement(3, "i'm no.4");
	//	list.InsertElement(4, "i'm no.5");
	//});

	//TKMeasureTime::PrintWithMicroseconds([&list]() -> void {
	//	for (int i(0); list[i].compare(""); i++)
	//		std::cout << "result: " << list[i] << std::endl;
	//});
	
	std::cout << std::endl;

	return 0;
}
