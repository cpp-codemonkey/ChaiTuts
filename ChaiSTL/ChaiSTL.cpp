// ChaiSTL.cpp	
#include <iostream>
#include <chaiscript/chaiscript.hpp>
#include <string>
#include <functional>
#include <vector>

int main()
{
	typedef std::vector<chaiscript::Boxed_Value> VecType;

	// get instance of engine and load script file
	chaiscript::ChaiScript chai;
	chai.eval_file(R"(..\Scripts.chai)");

	// get a vector of 3 ints from a script
	std::cout << "A vector of 3 ints without calling a chaiscript function:\n";
	VecType ints = chai.eval<VecType>("[1, 2, 3]");
	for (auto var : ints)
	{
		std::cout << chaiscript::boxed_cast<int>(var) << std::endl;
	}

	// get an instance of a chaiscript function that takes an integer and returns
	// a chaiscript vector that contains integers from zero up to but not including
	// the integer parameter
	auto callGetVector = chai.eval<std::function<VecType(int)>>("getVector");
	auto ints2 = callGetVector(10);

	// display ints2 vector
	std::cout << "\nA vector of ints by calling the getVector() ChaiScript function:\n";
	for (auto var : ints2)
	{
		std::cout << chaiscript::boxed_cast<int>(var) << std::endl;
	}

	// pass a vector from C++ to a ChaiScript function
	auto callPassVector = chai.eval<std::function<void(VecType)>>("passVector");

	//auto callPassVector = chai.eval<
	//	std::function<void(std::vector<chaiscript::Boxed_Value>)>>("passVector");

	std::cout << "\nPassing a vector to ChaiScript:\n";
	callPassVector(ints);

	return 0;
}