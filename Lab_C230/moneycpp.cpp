#include "money.h"

//���������� ����������
//������� ��� ������ ����� �����
std::mutex balanceMutex;
//������� ��� ������������� ������
std::mutex coutMutex;
//����� �����
int totalBalance = 1000;

void withDrawMoney(int threadId, int withdrawAmount, int timeoutMs, std::stop_token stopToken) {
	int totalWithDrawn = 0;
	while (!stopToken.stop_requested())
	{
		//����-���
		std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMs));
		{
		//�������� ������ �� ����� �����
			std::lock_guard<std::mutex>lock(balanceMutex);
			if (totalBalance<=0)
			{
				//������ �����������
				break;
			}
			if (totalBalance>=withdrawAmount)
			{
				totalBalance -= withdrawAmount;
				totalWithDrawn += withdrawAmount;
				//������������������ �����
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
				//������������������ �����
				{
					std::lock_guard<std::mutex> coutLock(coutMutex);
					std::cout << "Thread" << threadId << " withdrew remaining balance, Balance: 0\n";
				}
				
				//��� ������ �������
				break;
			}
		}
	}
	std::cout << "Thread " << threadId << " finished with total withdrawal: "
		<< totalWithDrawn << "\n";
}