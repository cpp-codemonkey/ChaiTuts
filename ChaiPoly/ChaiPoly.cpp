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
	std::wcout << L"Virtual member function test:\n";

	// add Accumulator type to chaiscript and its sum member function
	chai.add(chaiscript::user_type<Accumulator>(), "Accumulator");
	chai.add(chaiscript::fun(&Accumulator::sum), "sum");
	chai.add(chaiscript::base_class<Accumulator, Multiplier>());

	// get instance of script function that takes a pointer to an Accumulator
	auto retMemberSum = chai.eval<std::function<int(Accumulator*, int)>>("callMemberFunc");

	// create a pointer to Accumulator that actuall points to a Multiplier object
	Multiplier mult;
	Accumulator* accumulatorPtr = &mult;

	// call the script function
	int res = retMemberSum(accumulatorPtr, 10);
	std::wcout << res << std::endl;

	// pass an object of type Accumulator
	Accumulator a;
	std::wcout << retMemberSum(&a, 100);
	return 0;
}