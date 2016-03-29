#pragma once
 
class A
{
	int a = 0;
public:

	A(int aa): a(aa){}

	int get_a(int b) { return a*b; }

	int get_a2() const { return a; }
};