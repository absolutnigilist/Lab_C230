#include"pingpong.h"

//Глобальные семафоры
//ПИНГ начинает первым
std::binary_semaphore pingSemaphore(1);
//ПОНГ ждет ПИНГ
std::binary_semaphore pongSemaphore(0);

//Флаг для остановки игры
std::atomic<bool> stopGame(false);


void playPing() {
	while (!stopGame)
	{
		//Ждем разрешения от ПОНГ
		pingSemaphore.acquire();
		if (stopGame)
		{
			//Проверям, не остановлена ли игра
			break;
		}
		std::cout << "PING\n" << std::endl;
		//Имитация задержки
		std::this_thread::sleep_for(std::chrono::milliseconds(750));
		//Разрешаем ПОНГ
		pongSemaphore.release();
	}
}

void playPong() {
	while (!stopGame)
	{
		//Ждем разрешения от ПИНГ
		if (stopGame)
		{
			//Проверяем, не остановлена ли игра
			break;
		}
		std::cout << "PONG\n"<<std::endl;
		//Имитация задержки
		std::this_thread::sleep_for(std::chrono::milliseconds(750));
		//Разрешаем ПИНГ
		pingSemaphore.release();

	}
}
