#include "alphabet.h"

void printAlphabetWithIncreasingSpeed() {

	//Начальная задержка между символами (в мс)
	int delay = 500;
	//Минимальная задержка
	const int minDelay = 10;
	
	//Коэффициент ускорения
	const double acceleration = .9;

	std::cout<< "Printing alphabet with increasing speed:\n";

	for (char letter='A'; letter <= 'Z'; ++letter)
	{
		std::cout << letter << " " << std::flush;
		
		// Ждем заданное время
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		
		// Уменьшаем задержку, чтобы увеличить скорость
		delay = static_cast<int>(delay * acceleration);
		if (delay < minDelay) {
			delay = minDelay; // Ограничиваем минимальную задержку
		}
	}
	std::cout << "\nDone!" << std::endl;

}