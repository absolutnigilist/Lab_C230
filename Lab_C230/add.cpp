#include "add.h"

//Глобальные переменные
//Общий вектор для потоков
std::vector<int> sharedVector;
//Мьютекс для защиты доступа к секции
std::mutex vectorMutex;
//Сумма элементов в векторе
std::atomic<int> totalSum(0);
//Флаг завершения работы потоков
std::atomic<bool>stopFlag(false);

void addValues(int threadId, int limit) {
	std::mt19937 rng(std::random_device{}());
	//Генерируем числа от 1 до 10
	std::uniform_int_distribution<int>dist(1, 10);
	while (!stopFlag)
	{
		int value = dist(rng);
		{
			//Захватываем мьютекс для добавления
			std::lock_guard<std::mutex>lock(vectorMutex);
			//Проверяем флаг после захвата мьютекса
			if (stopFlag)
			{
				break;
			}
			sharedVector.push_back(value);
			totalSum += value;
			std::cout << "Thread " << threadId << " added: " << value
				<< ", Total Sum: " << totalSum << "\n";
		}
		//Проверяем условие завершения
		if (totalSum>=limit)
		{
			stopFlag = true;
		}
		//Небольшая задержка, чтобы потоки чередовались
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}