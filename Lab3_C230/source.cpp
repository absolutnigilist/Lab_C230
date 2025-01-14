#include "header.h"

int getIntegerFromUser() {
	std::string input;
	std::cout << "Enter an integer value: ";
	std::cin >> input;

	try
	{
//Преобразуем строку в целое число
		return std::stoi(input);
	}
	catch (const std::invalid_argument& e)
	{
		throw std::invalid_argument("The entered number is not an integer \n");
	}
	catch (const std::out_of_range& e)
	{
		throw std::out_of_range("The value is out of range. \n");
	}
}