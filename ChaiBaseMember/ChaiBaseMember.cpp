#include <iostream>
#include <chaiscript/chaiscript.hpp>
#include <string>
#include <functional>

class Accumulator
{
public:
	int data;

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
	std::wcout << L"Exposing base member data test:\n";

	// add Accumulator and Multiplier types to chaiscript and its the sum member function
	chai.add(chaiscript::user_type<Accumulator>(), "Accumulator");
	chai.add(chaiscript::user_type<Multiplier>(), "Multiplier");
	chai.add(chaiscript::fun(&Accumulator::sum), "sum");
	chai.add(chaiscript::base_class<Accumulator, Multiplier>());

	// expose base member data
	chai.add(
		chaiscript::fun(static_cast<int(Accumulator::*)>(&Accumulator::data)),
		"data");

	// get instance of script function
	auto getBaseDataFunc = chai.eval<std::function<int(Accumulator*)>>("getBaseData");

	// create object
	Multiplier mult;
	Accumulator* const obj = &mult;
	obj->data = 32;

	// send result of function to the console output stream
	std::wcout << getBaseDataFunc(obj) << std::endl;

	Accumulator a;
	a.data = 42;
	std::wcout << getBaseDataFunc(&a) << std::endl;
	return 0;
}