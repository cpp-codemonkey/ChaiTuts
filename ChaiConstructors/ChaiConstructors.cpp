#include <iostream>
#include <chaiscript/chaiscript.hpp>
#include <string>
#include <functional>

class Accumulator
{
public:
	int data;

	Accumulator() : data{0}
	{
	}

	Accumulator(int const d) : data{d}
	{
	}

	Accumulator(Accumulator const& a) : data{a.data}
	{
	}

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
	Multiplier() : Accumulator{0}
	{
	}

	Multiplier(int const d) : Accumulator{d}
	{
	}

	Multiplier(Multiplier const& a) : Accumulator(a)
	{

	}

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

	// Add constructors for Accumulator
	chai.add(chaiscript::constructor<Accumulator()>(), "Accumulator");
	chai.add(chaiscript::constructor<Accumulator(int const)>(), "Accumulator");
	chai.add(chaiscript::constructor<Accumulator(Accumulator const&)>(), "Accumulator");

	// Add constructors for Multiplier
	chai.add(chaiscript::constructor<Multiplier()>(), "Multiplier");
	chai.add(chaiscript::constructor<Multiplier(int const)>(), "Multiplier");
	chai.add(chaiscript::constructor<Multiplier(Multiplier const&)>(), "Multiplier");

	// expose base member data
	chai.add(
		chaiscript::fun(static_cast<int(Accumulator::*)>(&Accumulator::data)),
		"data");

	// get instances of script functions
	auto createAnAccumulatorFunc =
		chai.eval<std::function<int()>>("constructAnAccumulator");
	auto createAMultiplierFunc =
		chai.eval<std::function<int()>>("constructAMultiplier");

	// call functions and send results to the console output
	std::wcout 
		<< L"Using an Accumulator object: " 
		<< createAnAccumulatorFunc() 
		<< std::endl;
	std::wcout
		<< L"Using a Multiplier object: "
		<< createAMultiplierFunc()
		<< std::endl;
	return 0;
}