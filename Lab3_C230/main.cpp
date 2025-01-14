#include "header.h"

int main() {

//--------------------------------------------------------------------------------------------
//		ЗАДАНИЕ 1
//--------------------------------------------------------------------------------------------
/*
Асинхронный запуск потоков.  Получение отложенного результата. Параллельная реализация std::accumulate()
Задача: сформировать среднее значение для контейнеров (vector, deque) с элементами любого типа (int, float,…)
    1. Создайте и заполните значениями контейнер с элементами типа int (количество элементов и значения можно формировать случайным образом).
Замечание: количество элементов должно быть достаточно большим, чтобы оценить результаты распараллеливания задачи
    2. Измерьте время работы std::accumulate() для суммирования элементов от begin() до end()
    3. Решите ту же задачу посредством параллельного суммирования:
        a. Исходя из количества вычислительных ядер, запустите посредством std::async() разумное количество потоков, каждый из которых должен получать сумму заданного диапазона
        b. Полученные частичные результаты по мере готовности требуется тоже просуммировать
        c. Измерьте общее время
        d. Попробуйте задать количество потоков
меньше, чем количество вычислительных ядер,
больше чем количество вычислительных ядер
    4. Сравните полученные результаты

*/
	{
		//Количество элементов в контейнере
		const size_t numElements = 1'000'000;
		//Диапазон значений
		const size_t minValue = 1, maxValue = 100;

		//Создание и заполнение контейнера
		std::vector<int> data;
		fillContainer(data, numElements, minValue, maxValue);

		//1. Использование std::accumulate для суммирования
		auto singleThreadTime = measureTime([&]() {
			auto sum = std::accumulate(data.begin(), data.end(), 0);
			std::cout << "Sum (single-thread): " << sum << "\n";
			});
		std::cout << "Time (std::accumulate): " << singleThreadTime << " ms\n";
		
		//2. Параллельное суммирование с количеством потоков = количеству ядер
		size_t numCores = std::thread::hardware_concurrency();
		auto parallelTime = measureTime([&]() {
			auto sum = parallelAccumulate(data.begin(), data.end(), 0, numCores);
			std::cout << "Sum (parallel, " << numCores << " threads): " << sum << "\n";
			});
		std::cout << "Time (parallel, " << numCores << " threads): " << parallelTime << " ms\n";
		// 3. Параллельное суммирование с количеством потоков меньше, чем ядер
		size_t fewerThreads = numCores / 2;
		auto fewerThreadsTime = measureTime([&]() {
			auto sum = parallelAccumulate(data.begin(), data.end(), 0, fewerThreads);
			std::cout << "Sum (parallel, " << fewerThreads << " threads): " << sum << "\n";
			});
		std::cout << "Time (parallel, " << fewerThreads << " threads): " << fewerThreadsTime << " ms\n";

		// 4. Параллельное суммирование с количеством потоков больше, чем ядер
		size_t moreThreads = numCores * 2;
		auto moreThreadsTime = measureTime([&]() {
			auto sum = parallelAccumulate(data.begin(), data.end(), 0, moreThreads);
			std::cout << "Sum (parallel, " << moreThreads << " threads): " << sum << "\n";
			});
		std::cout << "Time (parallel, " << moreThreads << " threads): " << moreThreadsTime << " ms\n";

	}
//--------------------------------------------------------------------------------------------
//		ЗАДАНИЕ 1
//--------------------------------------------------------------------------------------------
/*
Обработка исключений посредством future
Реализуйте в отдельном дочернем потоке  прием целых значений от пользователя. 
Если пользователь вводит НЕ целое, должно быть сгенерировано исключение.  
Обработку исключения требуется реализовать в родительском потоке.
Подсказка: можно использовать функцию std::stoi() – С++11, которая генерирует исключения:
• std::invalid_argument, если преобразование не может быть выполнено
• std::out_of_range, если преобразованное значение будет выходить за границы диапазона значений типа результата
*/
	{
		//Создание future для работы с дочерним потоком
		std::future<int> futureResult = std::async(std::launch::async, getIntegerFromUser);
		try
		{
			//Получение результата из дочернего потока
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