#include "syncstream.h"

#include <iostream>
#include <thread>
#include <string>
#include <syncstream> // Для std::osyncstream

//Определяем thread_local переменную
thread_local std::string s("hello from ");

void threadFunction(int threadNumber) {
//Модифицируем переменную s
	s += std::to_string(threadNumber);

	//Выводим строку и адрес переменной s
	{
		std::osyncstream sync_out(std::cout);
		sync_out << "Thread " << threadNumber
			<< " -> Value: " << s
			<< ", Address: " << &s << "\n";
	}
}