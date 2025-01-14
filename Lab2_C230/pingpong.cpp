#include"pingpong.h"

//���������� ��������
//���� �������� ������
std::binary_semaphore pingSemaphore(1);
//���� ���� ����
std::binary_semaphore pongSemaphore(0);

//���� ��� ��������� ����
std::atomic<bool> stopGame(false);


void playPing() {
	while (!stopGame)
	{
		//���� ���������� �� ����
		pingSemaphore.acquire();
		if (stopGame)
		{
			//��������, �� ����������� �� ����
			break;
		}
		std::cout << "PING\n" << std::endl;
		//�������� ��������
		std::this_thread::sleep_for(std::chrono::milliseconds(750));
		//��������� ����
		pongSemaphore.release();
	}
}

void playPong() {
	while (!stopGame)
	{
		//���� ���������� �� ����
		if (stopGame)
		{
			//���������, �� ����������� �� ����
			break;
		}
		std::cout << "PONG\n"<<std::endl;
		//�������� ��������
		std::this_thread::sleep_for(std::chrono::milliseconds(750));
		//��������� ����
		pingSemaphore.release();

	}
}
