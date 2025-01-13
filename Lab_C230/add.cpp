#include "add.h"

//���������� ����������
//����� ������ ��� �������
std::vector<int> sharedVector;
//������� ��� ������ ������� � ������
std::mutex vectorMutex;
//����� ��������� � �������
std::atomic<int> totalSum(0);
//���� ���������� ������ �������
std::atomic<bool>stopFlag(false);

void addValues(int threadId, int limit) {
	std::mt19937 rng(std::random_device{}());
	//���������� ����� �� 1 �� 10
	std::uniform_int_distribution<int>dist(1, 10);
	while (!stopFlag)
	{
		int value = dist(rng);
		{
			//����������� ������� ��� ����������
			std::lock_guard<std::mutex>lock(vectorMutex);
			//��������� ���� ����� ������� ��������
			if (stopFlag)
			{
				break;
			}
			sharedVector.push_back(value);
			totalSum += value;
			std::cout << "Thread " << threadId << " added: " << value
				<< ", Total Sum: " << totalSum << "\n";
		}
		//��������� ������� ����������
		if (totalSum>=limit)
		{
			stopFlag = true;
		}
		//��������� ��������, ����� ������ ������������
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}