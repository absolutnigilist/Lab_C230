#include "txt.h"
#include "beep.h"
#include "alphabet.h"
#include "transform.h"
#include "printstring.h"
#include "search.h"
#include "add.h"
#include "money.h"



int main() {


//--------------------------------------------------------------------------------------------
//		������� 1
//--------------------------------------------------------------------------------------------
/*
������ ������� � ��������� ����������
������������� ������, ��������, �� http://do-you-speak.ru/elementary-pre-intermediate � �������� ��� ������� ������ ���� - *.txt. 
������� ������������ ������������ �� ����� (� ������� ���������� ��� � ��������� �������������) ��������� ������, ��������:
const char* filespec[] = {"1.txt", "2.txt", "3.txt", "4.txt"�}; ��� std::string filespec[] = {"1.txt", "2.txt", "3.txt", "4.txt"�};
��������� ������� ����� ��������� ������� � ��������� ������. ������ ����� ������ ��������� ���������� �����, ��������� ��� ����� � 
������� ������� (� ����� � ������ ������� �������� ��������) � �������� � ����� ����. ����� ����� ������ ����� ����������� � ������� 
��������� �������� ��� ��������, ��������: 1.txt -> _1.txt/ ������������������� � ����������� ������������ ����������� �������, ������
��� ������� ����� ������������ ���������. ����� ���������� �������� �� �������� � ��������� ��� ������������ ����, ��������:
2s::10ms::150mks/ ���������, ����� ���� �� ������ ������������ ��� ��������� �������? � ����� ����, ����� �������� ������������� ������ ����� (�����?)
� ����� ������ ������ ���� �� ����� ����������� �������� ���������� ����������� �������.

1.� ��������������� ��������� ������. ��������� ����� ��������� ������ ����� ���������� ��������
�������� ����� ������ ���������.
1.� ��������� ������� �������, ������� ������ ��������� ����������
�������� ����� ������ ���������.
1.� ��������� ������� �������, ������� ���� � �������
�������� ����� ������ ���������.
*/
//������ ������
{
	std::vector<std::string>  filespec = {
		FULL_PATH("1.txt"),
		FULL_PATH("2.txt"),
		FULL_PATH("3.txt"),
		FULL_PATH("4.txt")
	};
	{
		//������� 1: ���������������� ���������� �������
		std::cout << "Sequential execution:\n";

		auto start = std::chrono::high_resolution_clock::now();

		for (const auto& file : filespec) {
			// ��������� ����� ����� �� ����
			std::string fileName = file.substr(file.find_last_of('/') + 1);
			std::string outputFile = std::string(BASE_PATH) + "/_" + fileName;
			//��������� ����� ��� ��������� ������ �����
			std::thread t(processFile, file, outputFile);
			//���� ���������� ������
			if (t.joinable())
			{
				t.join();
			}

		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Time:" << formatDuration(end - start) << std::endl;
	}

	{
		//������� 2:������������ ���������� �������
		std::cout << "Concurrent execution with threads for each file:\n";
		//������� ������ �������
		std::vector<std::thread>threads;
		threads.reserve(filespec.size());

		auto start = std::chrono::high_resolution_clock::now();

		for (const auto& file : filespec) {
			// ��������� ����� ����� �� ����
			std::string fileName = file.substr(file.find_last_of('/') + 1);
			std::string outputFile = std::string(BASE_PATH) + "/_" + fileName;
			//��������� �����b ��� ������� �����
			threads.emplace_back(processFile, file, outputFile);
		}
		// ���������� ���������� ���� �������
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Time:" << formatDuration(end - start) << std::endl;
	}
	{
		//������� 3: ������������ ���������� ������� ������ ����
		std::cout << "Concurrent execution with core limitation:\n";

		//������� ������ �������
		std::vector<std::thread>threads;
		threads.reserve(filespec.size());

		//������� �������
		unsigned int activeThreads = 0;

		// ��������� ����� ����
		unsigned int hardwareConcurrency = std::thread::hardware_concurrency();
		std::cout << "Number of available hardware threads: " << hardwareConcurrency << std::endl;


		auto start = std::chrono::high_resolution_clock::now();


		for (const auto& file : filespec) {
			// ��������� ����� ����� �� ����
			std::string fileName = file.substr(file.find_last_of('/') + 1);
			std::string outputFile = std::string(BASE_PATH) + "/_" + fileName;

			//��������� �����b ��� ������� �����, �� ������������ ���������� ������� ����������� ����
			if (activeThreads < hardwareConcurrency)
			{
				threads.emplace_back(processFile, file, outputFile);
				++activeThreads;
			}
			else
			{
				// ������� ���������� ���� �� ������ ������ ����� �������� ������
				for (auto& t : threads) {
					if (t.joinable())
					{
						t.join();
						--activeThreads;
					}
				}
				//����� ������������ ������, ��������� �����
				threads.emplace_back(processFile, file, outputFile);
				++activeThreads;
			}
		}
		// ����� ���������� ������ ���� �������, ���� ���������� ����������
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Time:" << formatDuration(end - start) << std::endl;
	}

}
		
//--------------------------------------------------------------------------------------------
//		������� 2
//--------------------------------------------------------------------------------------------
/*
�� 2. ������ ������������ �������. ���������� ������ �� �������� �������� �������
������ ����� ������ �������� �������� ���������� ������ � �������� ��������.
� �� Windows  ��� ����� ������������ ��������� ������� Beep(), ������ �� ������������� ���� ��������� ��������.
������������:
  Beep(261, <��������_�_��>); //  ������������� ��
  Beep(293, ��������_�_��); //  ��
  Beep(329, ��������_�_��); //  ��
  Beep(349, ��������_�_��); //  ��
  Beep(392, ��������_�_��); //  ����
  Beep(440, ��������_�_��); //  ��
  Beep(493, ��������_�_��); //  ��
�� ������� ������ ����� ���������� � �������
std::cout<<�\a�;
� ���������, �������� ������� ����� � ���� ������ ����������.
����� ������� ������� ��������� ��������� ������ �� ��������������� �������� thread.
��! ��� ���������� ���������� ������ � �� Windows ��� ��������� ������ ������������� ����������� => ����� �����-��
������� ��������� ��������� ����� ����������� ����� ����, ��� ����������� ��� ���������. ��������, ��������� ��� �� 
���������� ����� �� ������ - std::this_thread::sleep_for()
2.a
�������� �����
2.�
���������� ������� ������??? ���������: ������� Beep() ���������� ��������� ������� ��� ��������� ����� � �� ����������
����������, ���� �� ����� �������� ��������
*/
{
	//��������� � ������������� (������������ ������ ����)
	int interval = 500;
	// ���������� �������� ������ ����
	int count = 2;

	//������ �����
	playScale(interval,count);
		
	// ����� ��� ���������� ���� �������, ������ ��� �������� ����� ����������
	std::this_thread::sleep_for(std::chrono::seconds(2));
		
		
	count = 1;
	//������ ������
	playChord(interval,count);
}
//--------------------------------------------------------------------------------------------
//		������� 3
//--------------------------------------------------------------------------------------------
/*
this_thread
� ��������� ������ �������� �� ����� ������� (�A� � �Z�) ����� �������, 
����� �������� ������ ������ �������������.
*/
{
	printAlphabetWithIncreasingSpeed();
}
//--------------------------------------------------------------------------------------------
//		������� 4
//--------------------------------------------------------------------------------------------
/*
������������ ���������� std::transform(). ����������� ����� �������������� ����. ��������� �������.
��������� ����������� ��������� std::transform() ������������� �������� ������  �������� ���������� 
(vector, list, deque) � ���������� ������ ��������������� ���� (short, int, long long, float, double)
� �������� ������� �� �������: cont2[i] = abs(cont1[i]); �������� ������������ ���������� � ����������
�������������� ���� � ������������ ������������ ����������� �������� ���������� �������.
���������: ���������� ��������� ������� ����� ���� �� ������� ������ ��������� ������������� ��������, 
��� ��� ��������� ����� ���� ����� ��������� ���� ����� ������, ���� ��������� ������ ��������� ����.
���������� ������ ��������� ��������� ���������� ��������� ��� ������� ���������� ������. ������� 
��������� ������ ���������������� ��������� �����. �������� ����� ���������� ������ ������������� �����
���������� ��� ������� ������� ���������� �������. �����! ��� ���� ����� ��������, ��� ������ �������� 
������ ������������, ��� ��������������, � � �������� ������ ������, ��� ��� ��� ����������� ������ �����
��������� ������� ���������� ������ �������� (� ������� ����������� ����������� ������� ���������� ���������
�������, ���������� ����������� ����� ���� ������, ����������� ��� ������� ��������������� �����)
*/
{
	//������ ����������
	size_t containerSize = 1'000'000;
	std::vector<int> source(containerSize);
	//��������� ������� �� -5'000'000;
	std::iota(source.begin(), source.end(), -5'000'000);

	std::vector<int> result(containerSize);

	//���������� ���������� ����
	unsigned int numCores = std::thread::hardware_concurrency();
	std::cout << "Number of hardware cores: " << numCores << std::endl;

	//���� ���������� �������
	int numThreads;
	std::cout << "Enter number of threads to use (including main thread): ";
	std::cin >> numThreads;

	if (numThreads <= 0 || numThreads > static_cast<int>(numCores))
	{
		std::cerr << "Invalid number of threads. Using 1 thread (sequental execution)." << std::endl;
		numThreads = 1;
	}
	//��������� ������� ����������
	auto startTime = std::chrono::high_resolution_clock::now();
	parallelTransform(
		source.begin(),
		source.end(),
		result.begin(),
		[](int x) {return std::abs(x); },
		numThreads
	);

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	//������� ����� ����������
	std::cout << "Transformation completed in " << duration << " ms with " << numThreads << " threads. " << std::endl;
}
//--------------------------------------------------------------------------------------------
//		������� 5
//--------------------------------------------------------------------------------------------
/*
����� jthread, namespace this_thread, std::mutex
����� ������, ��������:
 const char* strings[] = { "Long-long ago..." ,"I am very sorry", "My Dog", �};

��������� ��������� ���������� �������, ������ ����� ����� � �������.
������ ����� ������ �������� ���� ������������� � ����������� ������, ��������:
Id: <��������_��������������> string: <������>
, ���� ������������ �� ������ ����� �������.
*/
{
//�������� ������ �����
	const char* strings[] = { "Long-long ago...", "I am very sorry", "My Dog", "Another string" };
	size_t numStrings = sizeof(strings) / sizeof(strings[0]);

	//���� ��� ���������� �������
	std::atomic<bool>running(true);

	//������ �������
	std::vector<std::jthread>threads;

	//������� � ��������� ������
	for (size_t i = 0; i < numStrings; i++)
	{
		threads.emplace_back(threadFunction, strings[i], std::ref(running));
	}
//������� ������� ����� �������
	std::cout << "Press any key to stop..." << std::endl;
	while (!_kbhit())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	//������������� ���� ����������
	running = false;
	//������ ������������� ����������� �� ������ �� ������� ���������, ��� ��� ������������ 
	// std::jthread
	std::cout << "All threads stopped." << std::endl;
	
}
//--------------------------------------------------------------------------------------------
//		������� 6
//--------------------------------------------------------------------------------------------
/*
����� jthread
����� ������� ������ ����� ����� (������������ ��������). ��������� ������, ���� �� � ������� 
�������� ��������. ����������� ������������ ����� �� ��������� ���������� �������. ��� 
����������� ������� ���������� ��� ������ ������ ���������� ������.
*/
{
//������ ������� � ��������� ��������� �����
	const size_t vectorSize = 1'000'000;
	std::vector<int> numbers(vectorSize);

	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(1, 1'000'000);
	std::generate(numbers.begin(), numbers.end(), [&]() {return dist(rng); });

	//�������� �������� ��� ������
	int target = dist(rng);
	std::cout << "Target: " << target << std::endl;

	//���������� �������
	const size_t threadCount = std::thread::hardware_concurrency();
	std::cout << "Number of threads: " << threadCount << std::endl;
	
	//���������� ��������� ��� �������
	size_t blockSize = vectorSize / threadCount;

	//���� ���������� ������
	std::atomic<bool>found(false);

	//������ �������
	std::vector<std::jthread> threads;

	//�������� ����� ����������
	auto startTime = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < threadCount; ++i)
	{
		size_t start = i * blockSize;
		size_t end = (i == threadCount - 1) ? vectorSize : start + blockSize;

		//��������� ����� ��� ������ � ���������
		threads.emplace_back(searchInRange, std::cref(numbers), start, end, target, std::ref(found));
	}
	//������ ������������� ���������� ��� ������ �� ������� ���������
	auto endTime = std::chrono::high_resolution_clock::now();
	std::cout << "Search completed in "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)
		<< " \n";
	if (found!=0)
	{
		std::cout << "Target value not found in the vector,\n";
	}
}
//--------------------------------------------------------------------------------------------
//		������� 7
//--------------------------------------------------------------------------------------------
/*
����� jthread. mutex
������ ������. ������ ����������� ��������� ������ ���� ��������. ��� ������ ����� �������� 
�������� ��������, ��� ������ ������ �����������!
*/
{
	//���������� �������
	const int numThreads = 5;
	//��������� �����
	const int sumLimit = 100;

	std::vector<std::jthread>threads;

	//��������� ������
	std::cout << "All threads started. Waiting for complection...\n";
	for (size_t i = 0; i < numThreads; ++i)
	{
		threads.emplace_back(addValues, i+1, sumLimit);
	}
	//������ �������� ������ ������������� ��������� std::jthread
	

	//�������� ���������� ���� �������
	for (auto& t : threads) {
		if (t.joinable())
		{
			t.join();
		}
	}
	extern std::vector<int> sharedVector;
	extern std::atomic<int> totalSum;
	std::cout << "Final vector size: " << sharedVector.size() << "\n";
	std::cout << "Final total sum: " << totalSum << "\n";
}

//--------------------------------------------------------------------------------------------
//		������� 8
//--------------------------------------------------------------------------------------------
/*
std::mutex, std::lock_guard, std::stop_source, std::stop_token, std::stop_callback
������ ����� (��������, 1000). ��������� ��������� ��������� �������, ������ �� ������� ������ 
� �������� ����-����� (��������, 1 ms, 2 ms�) ���������� �� ����� ����� ���� ����, �������� 
������� �������� � �������� ��������� (��������, 1,2,�). ��� ������ ����� ����� ������������,
��� ������� ������������, �� ��������� ����������. ��� ���� ������ ����� ������ ������� 
�����������: ������������� ������ � ������� ����� ����� ���� ������� �� ����� �����.
*/
{
	//���������� �������
	int numThreads = 5;
	//�����, ������� ������ ��������
	int withdrawAmounts[] = { 10,20,30,40,50 };
	//����-���� ��� ������� � �������������
	int timeouts[] = { 1,2,3,4,5 };

	//�������� ��� ��������� �������
	std::stop_source stopSource;
	std::vector<std::jthread>threads;

	//������� � ��������� ������
	for (size_t i = 0; i < numThreads; ++i)
	{
		threads.emplace_back(
			withDrawMoney,
			i+1,
			withdrawAmounts[i],
			timeouts[i],
			stopSource.get_token()
		);
	}
	//������� ���������� �������
	for (auto& thread : threads) {
		if (thread.joinable())
		{
			thread.join();
		}
	}
	extern int totalBalance;
	std::cout << "All threads finished. Final Balance: " << totalBalance << "\n";
}
return 0;
}