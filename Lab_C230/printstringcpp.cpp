#include "printstring.h"

//������� ��� ������������� ������
std::mutex outputMutex;

void threadFunction(const std::string& str, std::atomic<bool>& running) {
	while (running)
	{
		{
			//������������� ������ � ������� ��������
			std::lock_guard<std::mutex> lock(outputMutex);
			std::cout << "Id" << std::this_thread::get_id() << " string " << str << std::endl;
		}

	//�������� ��� �������� ������������� ������ ������
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
