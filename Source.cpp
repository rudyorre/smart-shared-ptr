#include <iostream>
#include <cassert>
#include <string>
#include "smart_ptr.cpp"
using namespace std;

// basic constructor tests //
void test1() 
{
	smart_ptr<int> sp1; // default constructor
	assert(sp1.ref_count() == 0);

	int* p2 = new int{ 42 };
	smart_ptr<int> sp2(p2); // l-value raw_ptr constructor
	assert(sp2.ref_count() == 1);

	smart_ptr<int> sp3(new int{ 12 }); // r-value raw_ptr constructor
	assert(sp3.ref_count() == 1);

	{
		smart_ptr<int> sp4{ sp2 }; // l-value copy constructor
		assert(sp2.ref_count() == 2 && sp4.ref_count() == 2);
		smart_ptr<int> sp5;
		sp5 = (sp4 = sp2);
	}
	assert(sp2.ref_count() == 1);

	smart_ptr<int> sp6(std::move(sp3)); // r-value copy constructor
	assert(sp6.ref_count() == 1 && sp3.ref_count() == 0);
}

// basic operation tests //
void test2() 
{
	smart_ptr<string> sp1{ new string("Rudy") };
	smart_ptr<string> sp2;
	sp2 = sp1; // LHS is nullptr
	assert(sp1.ref_count() == 2 && sp2.ref_count() == 2);

	smart_ptr<string> sp3{ new string("Rudy") };
	smart_ptr<string> sp4;
	sp3 = sp4;
	assert(sp3.ref_count() == 0 && sp4.ref_count() == 0);

	smart_ptr<string> sp5{ new string("Russell") };
	smart_ptr<string> sp6;
	sp6 = std::move(sp5);
	assert(sp5.ref_count() == 0 && sp6.ref_count() == 1);
}

// clone function //
void test3() 
{
	smart_ptr<double> sp1{ new double {3.14} };
	smart_ptr<double> sp2, sp3;
	sp3 = sp2 = sp1;

	assert(sp1.ref_count() == 3 && sp2.ref_count() == 3 && sp3.ref_count() == 3);
	assert(sp1.clone() == true);
	assert(sp1.ref_count() == 1 && sp2.ref_count() == 2 && sp3.ref_count() == 2);
}

// * and -> operators //
void test4() 
{
	smart_ptr<string> sp1(new string{ "Rudy" });
	smart_ptr<string> sp2;
	sp2 = sp1;
	assert(*sp1 == "Rudy" && *sp2 == "Rudy");
	assert(sp1.ref_count() == 2 && sp2.ref_count() == 2);
	
	smart_ptr<string> sp3(new string{ "Orre" });
	sp2 = sp1 = sp3;
	assert(*sp1 == "Orre" && *sp2 == "Orre" && *sp3 == "Orre");
	assert(sp1.ref_count() == 3 && sp2.ref_count() == 3 && sp3.ref_count() == 3);
}

// Prof. Russell's Tests //
void test5()
{
	{
		int* p{ new int { 42 } };
		smart_ptr<int> sp1{ p };
		assert(sp1.ref_count() == 1);
		{
			smart_ptr<int> sp2{ sp1 };
			assert(sp1.ref_count() == 2);
			assert(sp2.ref_count() == 2);
		}
		assert(sp1.ref_count() == 1);

		smart_ptr<int> sp3;
		assert(sp3.ref_count() == 0);

		sp3 = sp1;
		assert(sp1.ref_count() == 2);
		assert(sp3.ref_count() == 2);

		smart_ptr<int> sp4 = std::move(sp1);
		assert(*sp4 == 42 && *sp3 == 42);
		bool exception = false;
		int t = 10;
		try
		{
			t = *sp1;
		}
		catch (null_ptr_exception e)
		{
			exception = true;
		}
		assert(t == 10);
		assert(exception == true);
	}
	{
		struct Point { int x = 2; int y = -5; };
		smart_ptr<Point> sp{ new Point };
		assert(sp->x == 2 && sp->y == -5);
	}
}

void test6()
{

}

int main()
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();

	cout << "All tests passed! :))" << endl;
}