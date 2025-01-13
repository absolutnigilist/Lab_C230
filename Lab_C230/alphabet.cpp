#include "alphabet.h"

void printAlphabetWithIncreasingSpeed() {

	//��������� �������� ����� ��������� (� ��)
	int delay = 500;
	//����������� ��������
	const int minDelay = 10;
	
	//����������� ���������
	const double acceleration = .9;

	std::cout<< "Printing alphabet with increasing speed:\n";

	for (char letter='A'; letter <= 'Z'; ++letter)
	{
		std::cout << letter << " " << std::flush;
		
		// ���� �������� �����
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		
		// ��������� ��������, ����� ��������� ��������
		delay = static_cast<int>(delay * acceleration);
		if (delay < minDelay) {
			delay = minDelay; // ������������ ����������� ��������
		}
	}
	std::cout << "\nDone!" << std::endl;

}