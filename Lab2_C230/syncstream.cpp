#include "syncstream.h"

#include <iostream>
#include <thread>
#include <string>
#include <syncstream> // ��� std::osyncstream

//���������� thread_local ����������
thread_local std::string s("hello from ");

void threadFunction(int threadNumber) {
//������������ ���������� s
	s += std::to_string(threadNumber);

	//������� ������ � ����� ���������� s
	{
		std::osyncstream sync_out(std::cout);
		sync_out << "Thread " << threadNumber
			<< " -> Value: " << s
			<< ", Address: " << &s << "\n";
	}
}