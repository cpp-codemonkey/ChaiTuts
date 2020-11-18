// ChaiExceptions.cpp 

#include <iostream>
#include <chaiscript/chaiscript.hpp>
#include <string>
#include <functional>
#include <exception>

void throwExection()
{
	throw std::runtime_error("Exception from C++");
}

int main()
{
	// get instance of engine and load script file
	chaiscript::ChaiScript chai;
	chai.eval_file(R"(..\Scripts.chai)");

	// throw in C++, catch in ChaiSript
	chai.add(chaiscript::fun(&throwExection), "throwException");
	chai("try {throwException();}catch(e){print(e.what());}");

	// throw in ChaiScript, catch in C++
	try
	{
		chai("throw(\"Exception from ChaiScript\")");
	}
	catch (chaiscript::Boxed_Value bv)
	{
		std::string i = chaiscript::boxed_cast<std::string>(bv);
		std::cout << i << std::endl;
	}
}