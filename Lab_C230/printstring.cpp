#include "printstring.h"

//Мьютекс для синхронизации потока
std::mutex outputMutex;

void threadFunction(const std::string& str, std::atomic<bool>& running) {
	while (running)
	{
		{
			//Синхронизация вывода с помощью мьютекса
			std::lock_guard<std::mutex> lock(outputMutex);
			std::cout << "Id" << std::this_thread::get_id() << " string " << str << std::endl;
		}

	//Задержка для снижения интенсивности работы потока
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
