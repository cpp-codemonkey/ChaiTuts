#include <iostream>
#include <chaiscript/chaiscript.hpp>
#include <string>
#include <functional>

int main()
{
	// get instance of engine and load script file
	chaiscript::ChaiScript chai;
	chai.eval_file(R"(..\Scripts.chai)");

	// add C++ lambda expression to the script engine
	chai.add(chaiscript::fun<std::function<std::string(bool)>>(
		[](bool type)
		{
			if (type)
			{
				return "true";
			}
			else
			{
				return "false";
			}
		}), 
		"cppLambda");

	// get instance of script function to call the C++ lambda and use it
	auto useCppLambda = chai.eval<std::function<std::string(bool)>>("callCppLambda");
	std::cout << "useLambda(true): " << useCppLambda(true) << std::endl;
	std::cout << "useLambda(false): " << useCppLambda(false) << std::endl;

	// call a chai lambda from C++
	auto useChaiLambda = chai.eval<std::function<std::string(bool)>>("callChaiLambda");
	std::cout << "useChaiLambda(true): " << useChaiLambda(true) << std::endl;
	std::cout << "useChaiLambda(false): " << useChaiLambda(false) << std::endl;

	// call a chai lambda that uses a capture list
	auto useChaiLambdaCapture = chai.eval<std::function<std::string(bool)>>
		("chaiLambdaCapture");
	std::cout << "useChaiLambdaCapture(true): " 
		<< useChaiLambdaCapture(true) 
		<< std::endl;
	std::cout << "useChaiLambdaCapture(false): " 
		<< useChaiLambdaCapture(false) 
		<< std::endl;

	return 0;
}