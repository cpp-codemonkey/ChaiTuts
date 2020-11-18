#include <iostream>
#include <chaiscript/chaiscript.hpp>
#include <string>
#include <functional>

class Accumulator
{
public:
	virtual int sum(int const N)
	{
		std::wcout << __FUNCTIONW__ << std::endl;
		int s = 0;
		for (int x = 0; x <= N; ++x)
		{
			s += x;
		}
		return s;
	}

	void overload(int const x)
	{
		std::wcout << L"int version of overload function:\n";
		std::wcout << L"x=" << x << std::endl;
	}

	void overload(float const x)
	{
		std::wcout << L"float version of overload function:\n";
		std::wcout << L"x=" << x << std::endl;
	}
};

class Multiplier : public Accumulator
{
public:
	virtual int sum(int const N) override
	{
		std::wcout << __FUNCTIONW__ << std::endl;
		int s = 1;
		for (int x = 1; x <= N; ++x)
		{
			s *= x;
		}
		return s;
	}
};

int main()
{
	// obtain instance of script engine and load external script file
	chaiscript::ChaiScript chai;
	chai.eval_file(R"(..\Scripts.chai)");
	std::wcout << L"Overloaded member function test:\n";

	// add Accumulator type to chaiscript and its sum member function
	chai.add(chaiscript::user_type<Accumulator>(), "Accumulator");
	chai.add(chaiscript::fun(&Accumulator::sum), "sum");
	chai.add(chaiscript::base_class<Accumulator, Multiplier>());

	// add overloaded functions to ChaiScript engine
	chai.add(chaiscript::fun(static_cast<void(Accumulator::*)(int const)>
		(&Accumulator::overload)), "overload");
	chai.add(chaiscript::fun(static_cast<void(Accumulator::*)(float const)>
		(&Accumulator::overload)), "overload");

	// create a pointer to an Accumulator that holds a Multiplier
	Multiplier mult;
	Accumulator* const accumulatorPtr = &mult;

	// get instance of script functions
	auto retOverloadedIntMemberFunc = chai.eval<std::function<void(Accumulator*, int)>>
		("callOverloadedMemberFunc");
	auto retOverloadedFloatMemberFunc = 
		chai.eval<std::function<void(Accumulator*, float)>>("callOverloadedMemberFunc");

	// actually call these functions
	retOverloadedIntMemberFunc(accumulatorPtr, 10);
	retOverloadedFloatMemberFunc(accumulatorPtr, 100.123f);

	return 0;
}