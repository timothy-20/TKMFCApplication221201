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
	using TKTemplateNode = TKArrayListNode<T>;

private:
	size_t m_length;
	size_t m_capacity;
	TKTemplateNode* m_nodeList;

public:
	TKArrayList(int capacity) :
		m_length(0),
		m_capacity(capacity),
		m_nodeList(nullptr)
	{
		if (this->m_capacity > 0)
			this->m_nodeList = new TKTemplateNode[this->m_capacity];
	}
	TKArrayList(const TKArrayList& other) :
		m_length(other.m_length),
		m_capacity(other.m_capacity)
	{
		this->m_nodeList = new TKTemplateNode[this->capacity];

		for (int i(0); i < other.m_length; i++)
			this->m_nodeList[i] = *other.m_nodeList[i];
	}
	virtual ~TKArrayList()
	{
		delete[] this->m_nodeList;
	}

	// Utils
	bool AddElement(const T& value)
	{
		this->m_length++;

		// 적재된 값의 갯수가 list의 범위를 넘어선 경우
		if (this->m_length > this->m_capacity)
			this->ExpandCapacity();

		this->m_nodeList[this->m_length] = TKTemplateNode(value);

		return true;
	}
	bool InsertElement(size_t index, const T& value)
	{
		if (index > this->m_capacity)
			return false;

		this->m_length++;

		if (this->m_length > this->m_capacity)
			this->ExpandCapacity();

		for (int i(0); i < this->m_length; i++)
		{
			if (this->m_nodeList[i] != nullptr || i >= index)
				this->m_nodeList[i + 1] = this->m_nodeList[i];
		}

		this->m_nodeList[index]->SetValue(value);

		return true;
	}

	void ExpandCapacity()
	{
		this->m_capacity *= 2;

		TKTemplateNode* newNodeList(new TKTemplateNode[this->m_capacity]);

		for (int i(0); i < this->m_length; i++)
			newNodeList[i] = *this->m_nodeList[i];

		delete[] this->m_nodeList;
		this->m_nodeList = newNodeList;
	}
};

// 객체의 타입 비교
// if (std::is_same_v<decltype(other.value), decltype(this->value)>)

int main()
{
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::cout << std::endl;

	return 0;
}