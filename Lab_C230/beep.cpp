#include "beep.h"

void playNote(int frequency, int interval, int count) {
	
	for (size_t i = 0; i < count; i++)
	{
		//Воспроизводим звук
		Beep(frequency, interval);
	}
}

void playScale(int interval, int count) {
	//Частоты для нот в гамме от До до Си
	int frequencies[] = {261, 293, 329, 349, 392, 440, 493};

	
	for (size_t i = 0; i < 7; i++)
	{
		std::thread t(playNote, frequencies[i], interval,count);
		//Ждем завершения всех потоков

		if (t.joinable())
		{
			t.join();
		}
	}
}
void playChord(int interval, int count) {
	// Частоты для нот аккорда (например, C мажор)
	int frequencies[] = { 261, 329, 392 }; // До, Ми, Соль (аккорд До мажор)

	// Запускаем каждую ноту с минимальной задержкой
	std::vector<std::thread> threads;
	for (int frequency:frequencies) {
		threads.emplace_back(playNote, frequency, interval,count);
	}

	// Ждем завершения всех потоков
	for (auto& t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}

}