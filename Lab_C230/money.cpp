#include "money.h"

//Глобальные переменные
//Мьютекс для защиты общей суммы
std::mutex balanceMutex;
//Мьютекс для синхронизации вывода
std::mutex coutMutex;
//Общая сумма
int totalBalance = 1000;

void withDrawMoney(int threadId, int withdrawAmount, int timeoutMs, std::stop_token stopToken) {
	int totalWithDrawn = 0;
	while (!stopToken.stop_requested())
	{
		//Тайм-аут
		std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMs));
		{
		//Забираем деньги из общей суммы
			std::lock_guard<std::mutex>lock(balanceMutex);
			if (totalBalance<=0)
			{
				//Деньги закончились
				break;
			}
			if (totalBalance>=withdrawAmount)
			{
				totalBalance -= withdrawAmount;
				totalWithDrawn += withdrawAmount;
				//Синхронизированный вывод
				{
					std::lock_guard<std::mutex> coutLock(coutMutex);
					std::cout << "Thread " << threadId << " withdrew " << withdrawAmount
						<< ", Remaining Balance: " << totalBalance << "\n";
				}
			}
			else
			{
				totalWithDrawn += totalBalance;
				totalBalance = 0;
				//Синхронизированный вывод
				{
					std::lock_guard<std::mutex> coutLock(coutMutex);
					std::cout << "Thread" << threadId << " withdrew remaining balance, Balance: 0\n";
				}
				
				//Все деньги забраны
				break;
			}
		}
	}
	std::cout << "Thread " << threadId << " finished with total withdrawal: "
		<< totalWithDrawn << "\n";
}