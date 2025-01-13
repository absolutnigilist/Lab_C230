#include "beep.h"

void playNote(int frequency, int interval, int count) {
	
	for (size_t i = 0; i < count; i++)
	{
		//������������� ����
		Beep(frequency, interval);
	}
}

void playScale(int interval, int count) {
	//������� ��� ��� � ����� �� �� �� ��
	int frequencies[] = {261, 293, 329, 349, 392, 440, 493};

	
	for (size_t i = 0; i < 7; i++)
	{
		std::thread t(playNote, frequencies[i], interval,count);
		//���� ���������� ���� �������

		if (t.joinable())
		{
			t.join();
		}
	}
}
void playChord(int interval, int count) {
	// ������� ��� ��� ������� (��������, C �����)
	int frequencies[] = { 261, 329, 392 }; // ��, ��, ���� (������ �� �����)

	// ��������� ������ ���� � ����������� ���������
	std::vector<std::thread> threads;
	for (int frequency:frequencies) {
		threads.emplace_back(playNote, frequency, interval,count);
	}

	// ���� ���������� ���� �������
	for (auto& t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}

}