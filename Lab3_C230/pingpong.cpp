#include "pingpong.h"

std::mutex mtx;
std::condition_variable cv;
bool turn = true;  // ����������, ��� ���: true � "����", false � "����"
std::atomic<bool> running(true); // ���� ��� ��������� ����

//������� ����
void ping() {
	while (running)
	{
		std::unique_lock<std::mutex> lock(mtx);
		// ����, ���� �������� ������� "�����" ��� ���� �� �����������
		cv.wait(lock, []() {return turn || !running; });
		if (running)
		{
			break;
		}
		std::cout << "PING" << std::endl;
		//�������� ��� "�����"
		turn = false;
		//���������� "����"
		cv.notify_one();
	}
}

//������� ����
void pong() {
	while (running)
	{
		std::unique_lock<std::mutex>lock(mtx);
		//����, �� �� �������� ������� "�����" ��� ���� �� �����������
		cv.wait(lock, [] {return !turn || !running; });
		if (!running)
		{
			break;
		}
		std::cout << "PONG" << std::endl;
		//�������� ��� "�����"
		turn = true;
		//���������� "����"
		cv.notify_one();
	}

}

//������� ��� ������������ ������� �������
void monitorInput() {
	while (running)
	{	
		//���������, ���� �� ����� �������
		if (_kbhit())
		{
			//������������� ���� ����������
			running = false;
			//������������� ��� ������
			cv.notify_all();
		}
	}
}