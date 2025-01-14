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
		cv.notify_one();
	}
}

//Функция ПОНГ
void pong();