#include "header.h"

int main() {

//--------------------------------------------------------------------------------------------
//		������� 1
//--------------------------------------------------------------------------------------------
/*
����������� ������ �������.  ��������� ����������� ����������. ������������ ���������� std::accumulate()
������: ������������ ������� �������� ��� ����������� (vector, deque) � ���������� ������ ���� (int, float,�)
    1. �������� � ��������� ���������� ��������� � ���������� ���� int (���������� ��������� � �������� ����� ����������� ��������� �������).
���������: ���������� ��������� ������ ���� ���������� �������, ����� ������� ���������� ����������������� ������
    2. �������� ����� ������ std::accumulate() ��� ������������ ��������� �� begin() �� end()
    3. ������ �� �� ������ ����������� ������������� ������������:
        a. ������ �� ���������� �������������� ����, ��������� ����������� std::async() �������� ���������� �������, ������ �� ������� ������ �������� ����� ��������� ���������
        b. ���������� ��������� ���������� �� ���� ���������� ��������� ���� ��������������
        c. �������� ����� �����
        d. ���������� ������ ���������� �������
������, ��� ���������� �������������� ����,
������ ��� ���������� �������������� ����
    4. �������� ���������� ����������

*/
	{
		//���������� ��������� � ����������
		const size_t numElements = 1'000'000;
		//�������� ��������
		const size_t minValue = 1, maxValue = 100;

		//�������� � ���������� ����������
		std::vector<int> data;
		fillContainer(data, numElements, minValue, maxValue);

		//1. ������������� std::accumulate ��� ������������
		auto singleThreadTime = measureTime([&]() {
			auto sum = std::accumulate(data.begin(), data.end(), 0);
			std::cout << "Sum (single-thread): " << sum << "\n";
			});
		std::cout << "Time (std::accumulate): " << singleThreadTime << " ms\n";
		
		//2. ������������ ������������ � ����������� ������� = ���������� ����
		size_t numCores = std::thread::hardware_concurrency();
		auto parallelTime = measureTime([&]() {
			auto sum = parallelAccumulate(data.begin(), data.end(), 0, numCores);
			std::cout << "Sum (parallel, " << numCores << " threads): " << sum << "\n";
			});
		std::cout << "Time (parallel, " << numCores << " threads): " << parallelTime << " ms\n";
		// 3. ������������ ������������ � ����������� ������� ������, ��� ����
		size_t fewerThreads = numCores / 2;
		auto fewerThreadsTime = measureTime([&]() {
			auto sum = parallelAccumulate(data.begin(), data.end(), 0, fewerThreads);
			std::cout << "Sum (parallel, " << fewerThreads << " threads): " << sum << "\n";
			});
		std::cout << "Time (parallel, " << fewerThreads << " threads): " << fewerThreadsTime << " ms\n";

		// 4. ������������ ������������ � ����������� ������� ������, ��� ����
		size_t moreThreads = numCores * 2;
		auto moreThreadsTime = measureTime([&]() {
			auto sum = parallelAccumulate(data.begin(), data.end(), 0, moreThreads);
			std::cout << "Sum (parallel, " << moreThreads << " threads): " << sum << "\n";
			});
		std::cout << "Time (parallel, " << moreThreads << " threads): " << moreThreadsTime << " ms\n";

	}
//--------------------------------------------------------------------------------------------
//		������� 1
//--------------------------------------------------------------------------------------------
/*
��������� ���������� ����������� future
���������� � ��������� �������� ������  ����� ����� �������� �� ������������. 
���� ������������ ������ �� �����, ������ ���� ������������� ����������.  
��������� ���������� ��������� ����������� � ������������ ������.
���������: ����� ������������ ������� std::stoi() � �++11, ������� ���������� ����������:
� std::invalid_argument, ���� �������������� �� ����� ���� ���������
� std::out_of_range, ���� ��������������� �������� ����� �������� �� ������� ��������� �������� ���� ����������
*/
	{
		//�������� future ��� ������ � �������� �������
		std::future<int> futureResult = std::async(std::launch::async, getIntegerFromUser);
		try
		{
			//��������� ���������� �� ��������� ������
			int result = futureResult.get();
			std::cout << "You have entered a number: " << result << std::endl;
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << "Error: " << e.what() << std::endl;
		}
		catch(const std::out_of_range& e) 
		{
			std::cout << "Error: " << e.what() << std::endl;
		}
		catch (...) 
		{
			std::cout << "Unknown error." << std::endl;
		}

	}
	return 0;
}