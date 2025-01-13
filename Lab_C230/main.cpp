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
//		ЗАДАНИЕ 1
//--------------------------------------------------------------------------------------------
/*
Запуск потоков с ожиданием завершения
Позаимствуйте тексты, например, на http://do-you-speak.ru/elementary-pre-intermediate и создайте для каждого текста файл - *.txt. 
Задайте спецификации существующих на диске (в текущей директории или в отдельной поддиректории) текстовых файлов, например:
const char* filespec[] = {"1.txt", "2.txt", "3.txt", "4.txt"…}; или std::string filespec[] = {"1.txt", "2.txt", "3.txt", "4.txt"…};
Обработку каждого файла требуется сделать в отдельном потоке. Каждый поток должен прочитать содержимое файла, перевести все буквы в 
верхний регистр (а цифры и другие символы оставить прежними) и записать в новый файл. Имена новых файлов можно формировать с помощью 
заданного префикса или суффикса, например: 1.txt -> _1.txt/ Поэкспериментируйте с количеством одновременно запускаемых потоков, каждый
раз измеряя время выполняемого фрагмента. Время выполнения хотелось бы выводить в привычном для пользователя виде, например:
2s::10ms::150mks/ Подумайте, какие часы Вы будете использовать для измерения времени? А может быть, стоит сравнить использование разных часов (каких?)
В общем случае удобно было бы иметь возможность задавать количество запускаемых потоков.

1.а Последовательно запускаем потоки. Очередной поток запускаем только после завершения текущего
Измерьте время работы фрагмента.
1.б Запускаем столько потоков, сколько файлов требуется обработать
Измерьте время работы фрагмента.
1.в Запускаем столько потоков, сколько ядер в системе
Измерьте время работы фрагмента.
*/
//Список файлов
{
	std::vector<std::string>  filespec = {
		FULL_PATH("1.txt"),
		FULL_PATH("2.txt"),
		FULL_PATH("3.txt"),
		FULL_PATH("4.txt")
	};
	{
		//Вариант 1: Последовательное выполнение потоков
		std::cout << "Sequential execution:\n";

		auto start = std::chrono::high_resolution_clock::now();

		for (const auto& file : filespec) {
			// Получение имени файла из пути
			std::string fileName = file.substr(file.find_last_of('/') + 1);
			std::string outputFile = std::string(BASE_PATH) + "/_" + fileName;
			//Запускаем поток для обработки одного файла
			std::thread t(processFile, file, outputFile);
			//Ждем завершения потока
			if (t.joinable())
			{
				t.join();
			}

		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Time:" << formatDuration(end - start) << std::endl;
	}

	{
		//Вариант 2:Параллельное выполнение потоков
		std::cout << "Concurrent execution with threads for each file:\n";
		//Создаем вектор потоков
		std::vector<std::thread>threads;
		threads.reserve(filespec.size());

		auto start = std::chrono::high_resolution_clock::now();

		for (const auto& file : filespec) {
			// Получение имени файла из пути
			std::string fileName = file.substr(file.find_last_of('/') + 1);
			std::string outputFile = std::string(BASE_PATH) + "/_" + fileName;
			//Запускаем потокb для каждого файла
			threads.emplace_back(processFile, file, outputFile);
		}
		// Дожидаемся завершения всех потоков
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Time:" << formatDuration(end - start) << std::endl;
	}
	{
		//Вариант 3: Ограничиваем количество потоков числом ядер
		std::cout << "Concurrent execution with core limitation:\n";

		//Создаем вектор потоков
		std::vector<std::thread>threads;
		threads.reserve(filespec.size());

		//Счетчик потоков
		unsigned int activeThreads = 0;

		// Получение числа ядер
		unsigned int hardwareConcurrency = std::thread::hardware_concurrency();
		std::cout << "Number of available hardware threads: " << hardwareConcurrency << std::endl;


		auto start = std::chrono::high_resolution_clock::now();


		for (const auto& file : filespec) {
			// Получение имени файла из пути
			std::string fileName = file.substr(file.find_last_of('/') + 1);
			std::string outputFile = std::string(BASE_PATH) + "/_" + fileName;

			//Запускаем потокb для каждого файла, но ограничиваем количество потоков количеством ядер
			if (activeThreads < hardwareConcurrency)
			{
				threads.emplace_back(processFile, file, outputFile);
				++activeThreads;
			}
			else
			{
				// Ожидаем завершения хотя бы одного потока перед запуском нового
				for (auto& t : threads) {
					if (t.joinable())
					{
						t.join();
						--activeThreads;
					}
				}
				//После освобождения потока, запускаем новый
				threads.emplace_back(processFile, file, outputFile);
				++activeThreads;
			}
		}
		// После завершения работы всех потоков, ждем завершения оставшихся
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
//		ЗАДАНИЕ 2
//--------------------------------------------------------------------------------------------
/*
ие 2. Запуск обособленных потоков. Блокировка потока на заданный интервал времени
Каждый поток должен «издать» заданное количество звуков с заданной частотой.
В ОС Windows  для этого используется системная функция Beep(), другие ОС предоставляют свои системные средства.
Соответствие:
  Beep(261, <интервал_в_мс>); //  соответствует ДО
  Beep(293, интервал_в_мс); //  РЕ
  Beep(329, интервал_в_мс); //  МИ
  Beep(349, интервал_в_мс); //  ФА
  Beep(392, интервал_в_мс); //  СОЛЬ
  Beep(440, интервал_в_мс); //  ЛЯ
  Beep(493, интервал_в_мс); //  СИ
На крайний случай можно «пискнуть» с помощью
std::cout<<’\a’;
к сожалению, изменить частоту звука в этом случае невозможно.
После запуска потоков требуется отключить потоки от соответствующих объектов thread.
Но! при завершении первичного потока в ОС Windows все остальные потоки принудительно завершаются => нужно каким-то
образом заставить первичный поток завершиться после того, как завершились все вторичные. Например, отправить его на 
длительное время «в спячку» - std::this_thread::sleep_for()
2.a
Сыграйте гамму
2.б
Попробуйте сыграть аккорд??? Подсказка: функция Beep() использует системный динамик для генерации звука и не возвращает
управление, пока не истек заданный интервал
*/
{
	//Интерывал в миллисекундах (длительность каждой ноты)
	int interval = 500;
	// количество повторов каждой ноты
	int count = 2;

	//Играем гамму
	playScale(interval,count);
		
	// Пауза для завершения всех потоков, прежде чем основной поток завершится
	std::this_thread::sleep_for(std::chrono::seconds(2));
		
		
	count = 1;
	//Играем аккорд
	playChord(interval,count);
}
//--------------------------------------------------------------------------------------------
//		ЗАДАНИЕ 3
//--------------------------------------------------------------------------------------------
/*
this_thread
В первичном потоке выведите на экран алфавит (‘A’ – ‘Z’) таким образом, 
чтобы скорость вывода плавно увеличивалась.
*/
{
	printAlphabetWithIncreasingSpeed();
}
//--------------------------------------------------------------------------------------------
//		ЗАДАНИЕ 4
//--------------------------------------------------------------------------------------------
/*
Параллельная реализация std::transform(). Определение числа вычислительных ядер. Измерение времени.
Требуется посредством алгоритма std::transform() преобразовать элементы любого  базового контейнера 
(vector, list, deque) с элементами любого арифметического типа (short, int, long long, float, double)
в элементы другого по правилу: cont2[i] = abs(cont1[i]); Выведите пользователю информацию о количестве
вычислительных ядер и предоставьте пользователю возможность задавать количество потоков.
Подсказка: количество вторичных потоков может быть на единицу меньше заданного пользователем значения, 
так как первичный поток тоже может выполнить свою часть работы, пока вторичные потоки выполняют свою.
Определите размер диапазона элементов контейнера источника для каждого вторичного потока. Остаток 
элементов должен трансформировать первичный поток. Измерьте время выполнения полной трансформации всего
контейнера при задании разного количества потоков. Важно! при этом нужно понимать, что оценка является 
скорее качественной, чем количественной, а о точности судить трудно, так как для достоверной оценки нужно
учитывать большое количество других факторов (в системе параллельно выполняется большое количество системных
потоков, пропускная способность общей шины данных, кеширование для каждого вычислительного ядра…)
*/
{
	//Размер контейнера
	size_t containerSize = 1'000'000;
	std::vector<int> source(containerSize);
	//Заполняем числами от -5'000'000;
	std::iota(source.begin(), source.end(), -5'000'000);

	std::vector<int> result(containerSize);

	//Определяем количество ядер
	unsigned int numCores = std::thread::hardware_concurrency();
	std::cout << "Number of hardware cores: " << numCores << std::endl;

	//Ввод количества потоков
	int numThreads;
	std::cout << "Enter number of threads to use (including main thread): ";
	std::cin >> numThreads;

	if (numThreads <= 0 || numThreads > static_cast<int>(numCores))
	{
		std::cerr << "Invalid number of threads. Using 1 thread (sequental execution)." << std::endl;
		numThreads = 1;
	}
	//Измерение времени выполнения
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

	//Выводим время выполнения
	std::cout << "Transformation completed in " << duration << " ms with " << numThreads << " threads. " << std::endl;
}
//--------------------------------------------------------------------------------------------
//		ЗАДАНИЕ 5
//--------------------------------------------------------------------------------------------
/*
Класс jthread, namespace this_thread, std::mutex
Задан массив, например:
 const char* strings[] = { "Long-long ago..." ,"I am very sorry", "My Dog", …};

Требуется запустить количество потоков, равное числу строк в массиве.
Каждый поток должен выводить свой идентификатор и принимаемую строку, например:
Id: <значение_идентификатора> string: <строка>
, пока пользователь не нажмет любую клавишу.
*/
{
//Исходный массив строк
	const char* strings[] = { "Long-long ago...", "I am very sorry", "My Dog", "Another string" };
	size_t numStrings = sizeof(strings) / sizeof(strings[0]);

	//Флаг для завершения потоков
	std::atomic<bool>running(true);

	//Вектор потоков
	std::vector<std::jthread>threads;

	//Создаем и запускаем потоки
	for (size_t i = 0; i < numStrings; i++)
	{
		threads.emplace_back(threadFunction, strings[i], std::ref(running));
	}
//Ожидаем нажатия любой клавиши
	std::cout << "Press any key to stop..." << std::endl;
	while (!_kbhit())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	//Устанавливаем флаг завершения
	running = false;
	//Потоки автоматически завершаются по выходу из области видимости, так как используется 
	// std::jthread
	std::cout << "All threads stopped." << std::endl;
	
}
//--------------------------------------------------------------------------------------------
//		ЗАДАНИЕ 6
//--------------------------------------------------------------------------------------------
/*
Класс jthread
Задан большой вектор целых чисел (сгенерируйте значения). Требуется узнать, есть ли в векторе 
заданное значение. Реализовать параллельный поиск по отдельным диапазонам вектора. При 
обнаружении первого совпадения все потоки должны прекратить работу.
*/
{
//Размер вектора и генерация случайных чисел
	const size_t vectorSize = 1'000'000;
	std::vector<int> numbers(vectorSize);

	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(1, 1'000'000);
	std::generate(numbers.begin(), numbers.end(), [&]() {return dist(rng); });

	//Заданное значение для поиска
	int target = dist(rng);
	std::cout << "Target: " << target << std::endl;

	//Количество потоков
	const size_t threadCount = std::thread::hardware_concurrency();
	std::cout << "Number of threads: " << threadCount << std::endl;
	
	//Определяем диапазоны для потоков
	size_t blockSize = vectorSize / threadCount;

	//Флаг завершения поиска
	std::atomic<bool>found(false);

	//Вектор потоков
	std::vector<std::jthread> threads;

	//Засекаем время выполнения
	auto startTime = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < threadCount; ++i)
	{
		size_t start = i * blockSize;
		size_t end = (i == threadCount - 1) ? vectorSize : start + blockSize;

		//Запускаем поток для поиска в диапазоне
		threads.emplace_back(searchInRange, std::cref(numbers), start, end, target, std::ref(found));
	}
	//Потоки автоматически завершатся при выходе из области видимости
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
//		ЗАДАНИЕ 7
//--------------------------------------------------------------------------------------------
/*
Класс jthread. mutex
Пустой вектор. Потоки параллельно добавляют каждый свое значение. Как только сумма превысит 
заданное значение, все потоки должны завершиться!
*/
{
	//Количество потоков
	const int numThreads = 5;
	//Пороговая сумма
	const int sumLimit = 100;

	std::vector<std::jthread>threads;

	//Запускаем потоки
	std::cout << "All threads started. Waiting for complection...\n";
	for (size_t i = 0; i < numThreads; ++i)
	{
		threads.emplace_back(addValues, i+1, sumLimit);
	}
	//Потоки завершат работу автоматически благодаря std::jthread
	

	//Ожидание завершения всех потоков
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
//		ЗАДАНИЕ 8
//--------------------------------------------------------------------------------------------
/*
std::mutex, std::lock_guard, std::stop_source, std::stop_token, std::stop_callback
Задана сумма (например, 1000). Требуется запустить несколько потоков, каждый из которых должен 
с заданным тайм-аутом (например, 1 ms, 2 ms…) «забирать» из общей суммы свою долю, значение 
которой задается в качестве параметра (например, 1,2,…). Как только любой поток обнаруживает,
что «деньги закончились», он завершает выполнение. При этом каждый поток должен вывести 
диагностику: идентификатор потока и сколько денег поток смог забрать из общей суммы.
*/
{
	//Количесвто потоков
	int numThreads = 5;
	//Суммы, которые потоки забирают
	int withdrawAmounts[] = { 10,20,30,40,50 };
	//Тайм-ауты для потоков в миллисекундах
	int timeouts[] = { 1,2,3,4,5 };

	//Источник для остановки потоков
	std::stop_source stopSource;
	std::vector<std::jthread>threads;

	//Создаем и запускаем потоки
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
	//Ожидаем завершения потоков
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