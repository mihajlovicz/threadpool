#pragma once
#include <iostream>
#include <string>


class Base
{
public:
	virtual int getInt() const { return 0; };//nije htelo da radi sa apstraktnom funkcijom
	virtual const std::string& getString()  const { return ""; };
	virtual ~Base() {}
};



class MyString;
class MyInt : public Base
{
public:
	MyInt(int i) : mValue(i) {}
	MyInt() : mValue(0) {}
	int getInt()  const override { return mValue; }
	MyInt operator+(const MyString& rhs) const;
	virtual ~MyInt() {}
private:
	int mValue;
};



class MyString : public Base
{
public:
	MyString(const std::string& str) : mString(str) {}
	MyString() : mString("") {}
	const std::string& getString() const override { return mString; }
	MyString operator+(const MyInt& rhs) const;
	virtual ~MyString() {}
private:
	std::string mString;
};

MyInt MyInt::operator+(const MyString& rhs) const
{
	return mValue + stoi(rhs.getString());
}

MyString MyString::operator+(const MyInt& rhs) const
{
	std::string str = mString;
	str.append(std::to_string(rhs.getInt()));
	return str;
}



template<typename T1, typename T2>
T1 DoAdd(const T1& t1, const T2& t2)//-> decltype(t1+t2) c++11
{
	return t1 + t2;
}


template<typename T>
void print_base(T* t) {

	if (dynamic_cast<MyInt*>(t)) {
		std::cout << t->getInt();
	}
	if (dynamic_cast<MyString*>(t)) {
		std::cout << t->getString();
	}
}
