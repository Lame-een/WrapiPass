#include <iostream>
#include <string>
#include <list>
#include <thread>
#include <cerrno>
#include <sstream>
#include <fstream>

#include "varipass.h"

//
#include <windows.h>

int main()
{
	std::string key;
	std::cout << "Enter acc key: " << std::endl;
	std::cin >> key;

	VariPass test(key);

	///*
	//Continuous read test
	std::cout << "Enter id:" << std::endl;

	std::string id;

	std::cin >> id;
	std::cout << std::endl;

	while (1)
	{
		std::string json = test.readRaw(id);

		std::cout << "Int:" << std::endl;
		VariPass::VPVar<int> a = test.readRawInt(json);
		if (a.valid)
		{
			std::cout << "\tint value: " << a.value << std::endl
					  << "\tint age: " << (a.current - a.time) << std::endl;
		}

		std::cout << "Float:" << std::endl;
		VariPass::VPVar<float> b = test.readRawFloat(json);
		if (b.valid)
		{
			std::cout << "\tfloat value: " << b.value << std::endl
					  << "\tfloat age: " << (b.current - b.time) << std::endl;
		}

		std::cout << "Bool:" << std::endl;
		VariPass::VPVar<bool> c = test.readRawBool(json);
		if (c.valid)
		{
			std::cout << "\tbool value: " << c.value << std::endl
					  << "\tbool age: " << (c.current - c.time) << std::endl;
		}

		std::cout << "String:" << std::endl;
		VariPass::VPVar<std::string> d = test.readRawString(json);
		if (d.valid)
		{
			std::cout << "\tstring value: " << d.value << std::endl
					  << "\tstring age: " << (d.current - d.time) << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		system("cls");
	}
	//*/

	std::cin.get();
}