#include "WeakReference.h"

//-----------------------------------------------------------------
//----------------|Unit Testing|-----------------------------------
//-----------------------------------------------------------------


#ifdef NOSDL

#include "../Libs/Catch.hpp"

class TestA : public WeaklyReferable<TestA> {
public:
	TestA(int val = 0) : value(val) {};
	int value;
};

struct TestB {
	TestB(TestA& a) : testARef(a) {};
	WeakReference<TestA> testARef;
};

int ReadTestValue(TestA& val) {
	return val.value;
}

TEST_CASE("Weak reference testing") {

	SECTION("stack") {
		TestA foo{10};

		WeakReference<TestA> bar{ foo };
		WeakReference<TestA> bar2{foo};

		REQUIRE(bar);
		REQUIRE(bar2);
		REQUIRE(bar.Get().value == foo.value);
		REQUIRE(bar.Get().value == bar2.Get().value);
		REQUIRE(ReadTestValue(foo) == ReadTestValue(bar));
	}

	SECTION("Pointer") {
		TestA*  foo{ new TestA(10) };

		WeakReference<TestA> bar{ *foo };
		WeakReference<TestA> bar2{ *foo };

		REQUIRE(bar);
		REQUIRE(bar2);
		REQUIRE(bar.Get().value == (*foo).value);
		REQUIRE(bar.Get().value == bar2.Get().value);
		REQUIRE(ReadTestValue(*foo) == ReadTestValue(bar));

		bar.Get().value = 25;

		REQUIRE(bar.Get().value == 25);
		REQUIRE(bar2.Get().value == 25);
		REQUIRE(foo->value == 25);
		REQUIRE(bar.Get().value == (*foo).value);
		REQUIRE(bar.Get().value == bar2.Get().value);
		REQUIRE(ReadTestValue(*foo) == ReadTestValue(bar));

		{ // block scope 
			WeakReference<TestA> bar3{ *foo };
			REQUIRE(bar3);

			bar3.Get().value = 30;

			REQUIRE(bar.Get().value == 30);
			REQUIRE(bar2.Get().value == 30);
			REQUIRE(foo->value == 30);
			REQUIRE(bar.Get().value == (*foo).value);
			REQUIRE(bar.Get().value == bar2.Get().value);
			REQUIRE(ReadTestValue(*foo) == ReadTestValue(bar));
		}

		REQUIRE(bar.Get().value == 30);
		REQUIRE(bar2.Get().value == 30);
		REQUIRE(foo->value == 30);
		REQUIRE(bar.Get().value == (*foo).value);
		REQUIRE(bar.Get().value == bar2.Get().value);
		REQUIRE(ReadTestValue(*foo) == ReadTestValue(bar));

		WeakReference<TestA> bar4{ *foo };
		REQUIRE(bar4);

		foo->value = 25;

		REQUIRE(bar.Get().value == 25);
		REQUIRE(bar2.Get().value == 25);
		REQUIRE(foo->value == 25);
		REQUIRE(bar.Get().value == (*foo).value);
		REQUIRE(bar.Get().value == bar2.Get().value);
		REQUIRE(ReadTestValue(*foo) == ReadTestValue(bar));

		TestB* foo2{new TestB(*foo)};

		REQUIRE(foo2->testARef.Get().value == foo->value);

		int barVal = bar ? bar.Get().value : 5;
		REQUIRE(barVal == 25);

		delete foo;

		REQUIRE(not bar);
		REQUIRE(not bar2);
		REQUIRE(not bar4);

		int barVal2 = bar ? bar.Get().value : 5;
		REQUIRE(barVal2 == 5);

		REQUIRE(not foo2->testARef);
	}
}

#endif // !NOSDL
