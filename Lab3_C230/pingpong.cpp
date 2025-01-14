#include "pingpong.h"

std::mutex mtx;
std::condition_variable cv;
bool turn = true;  // Определяет, чей ход: true — "пинг", false — "понг"
std::atomic<bool> running(true); // Флаг для остановки игры

//Функция ПИНГ
void ping() {
	while (running)
	{
		std::unique_lock<std::mutex> lock(mtx);
		// Ждем, пока наступит очередь "пинга" или игра не остановится
		cv.wait(lock, []() {return turn || !running; });
		if (running)
		{
			break;
		}
		std::cout << "PING" << std::endl;
		//Передаем ход "ПОНГУ"
		turn = false;
		//Уведомляем "ПОНГ"
		cv.notify_one();
	}
}

//Функция ПОНГ
void pong() {
	while (running)
	{
		std::unique_lock<std::mutex>lock(mtx);
		//Ждем, по не наступит очередь "понга" или игра не остановится
		cv.wait(lock, [] {return !turn || !running; });
		if (!running)
		{
			break;
		}
		std::cout << "PONG" << std::endl;
		//Передаем ход "ПОНГУ"
		turn = true;
		//Уведомляем "ПОНГ"
		cv.notify_one();
	}

}

//Функция для отслеживания нажатия клавиши
void monitorInput() {
	while (running)
	{	
		//Проверяем, была ли нажат клавиша
		if (_kbhit())
		{
			//Устанавливаем флаг завершения
			running = false;
			//Устанавливаем все потоки
			cv.notify_all();
		}
	}
}