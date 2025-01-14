#include "threadsafe.h"
#include "pingpong.h"
#include "singleton.h"
#include "syncstream.h"

int main() {

//--------------------------------------------------------------------------------------------
//		������� 1
//--------------------------------------------------------------------------------------------
/*
���������������� ����. std::mutex. std::lock_guard. std::shared_mutex. std::lock(). std::scoped_lock.
����������� ����� threadsafe_stack:
1.	����� �� ������� ����� �� �������� ������, � �������� ��������� ������ ���������� std::vector
2.	��� ���� ����� ������������� ��������� ����� ��� ����������/������� ������ �� �����, �������:
	�� ����� ����������� �������� ��� ��������� ������ ������� �� ����������� ������� � ������ 
	������������ �� ���� � ����� ������� => ������ ���� ����� �������, ������� ����� ������������ 
	����� �����, ������������ � ����� => ������ ������� ������ ������ threadsafe_stack, ��� ��� ��
	����� ������������ ������� � ����������� �������, ����� ��������� �������� ��� ������������� 
	������ =>???
3.	��� ������ �� ����� ������/������ ������ �������� ���� �������� ���������
4.	���������: ����� push() �� ����� (�� ������) ��������� � �������� ��������� ������, ��� ���
	������ ����� ����� �������� �������� �� ������
5.	��������: ������������ std::vector ��������� ��� ��������: ��������� �������� ���������� 
	��������  - T& back();  � �������� ���������� �������� � void pop_back(); 
	���� �� ����� ��� ����������: T pop_back (); (����������� ���� ��������) � � 
	�������� ������������ ������������� �������� (��� �����������) ������������ ����������, 
	�� ��������� ��������, ����� �� ����� ������ ��� �������, � ������������ �������� ��� 
	�� ������������ => ������ ��������!��! ��� ����������������� ����� ����� ���������� �������� 
	��������� �����: ������� �� �������� � �������� (�� ���� ��������� �����):
	if(!s.empty())
	{
	res = s.top();
	s.pop();
	do_smth(res);
	},
	��� ��� ����� ������� empty() � top() ������ ����� ����� ������� pop()���������� ����� ����� ���������� ����� ������� top() � pop()
_________________________________________
|	����� 1		   |	 ����� 2		|
-----------------------------------------
|if(!s.empty())    |					|
|{				   |	                |
|res = s.top();	   |					|
|				   |if(!s.empty())		|
|				   |{				    |
|				   |res = s.top();      |
|				   |s.pop();			|
|				   |do_smth(res);		|
|				   |}					|
|s.pop();		   |					|
|do_smth(res);     |					|
|}				   |				    |
-----------------------------------------
���� �������� ��������� ������ �� �����, � ������ ����� ���������� ������!������� � ����������� ��������� ���������� ��� ��� �������� 
(����� ����������� ��� ����������� ���� ��������, ����� � ������������ ��� �����):
������� 1: void pop(T& val); //������ ������������� ����� ��� ��������������� �� ����� �������� => ���� ��� ����������� ������������ 
����������, �� �������� �������� � �����.
������� 2: ���������� ��������� �� ������������ ����� �������������� ��������. ��������, 
�� ���������, ��� ��� ��� ����������� ���������� �� ����� ���� ������������� ����������. �������� �������� ���������� ������������ 
�������, ������� ����� ������ ����������� std::shared_ptr<T> - std::shared_ptr<T> pop();
������� 3: optional<T> pop();
6.	��� ���������� ������������ ����������� ����� �������� �� ����������� ������ ��������
7.	��� ���������� operator= ����� ������ ����������� �������� ���������� => ����� �������� ��� ��������, ��� � ��������.
8.	��������� ��������� �������, ������� ����� ����������� �������� � ����� � push() (����������) � ��������  �������� � ���� �����, ������� ����� � �������� �������� ��������� ������� � �������� � ������������
9.	��� ������� ������� ������� � �������� ������� �����
o	��� ������ ���� ������������� ����������. ��! ���������� ������������� � �������� ������, � ���������� ������� � ������������ ������
o	��� ��������� ������� optional
*/
	{
		extern std::atomic<bool>stopConsumer;
		threadsafe_stack<int>stack;

		//������ ��������������
		std::thread producer1(producer, std::ref(stack), 0, 10, 100);
		std::thread producer2(producer, std::ref(stack), 100, 10, 150);

		//����� �����������
		std::thread consumer_thread(consumer, std::ref(stack), 200);

		// ���� ������� � ������������� �����
		std::this_thread::sleep_for(std::chrono::seconds(5));
		// ������ ���������
		stopConsumer = true; 

		producer1.join();
		producer2.join();
		consumer_thread.join();
	}
//--------------------------------------------------------------------------------------------
//		������� 2
//--------------------------------------------------------------------------------------------
/*
���� - ���� �� ���������.semaphore, jthread
����� ��� ������ ������ � ���� - ����.���� ����� ���������� ������ �����(�������� ����� �������).��������� 
���������� ������, ����� ������� ����� ����(� ����� ����).������� ����� ������� : ����� ������ ����(pong), 
�����, ����� ������ ���� ���� ������(ping).� �� �������� ������ ���� ���, ��� �������� ������(ping).���, 
��� ����, ������� �ping�, ���, ��� �������� - �pong�.
��������� 1� ��� ����������� �������������� ���������� ������������ �������� ��������;
��������� 2: ���������, ��� ������� ���������������� �������� ?
*/
	{
		std::cout << "Press any key to stop game..." << std::endl;
		//������� ������
		std::jthread pingThread(playPing);
		std::jthread pongThread(playPong);

		
		//�������� ������� �������
		std::cin.get();
		//������ ���� ������� �����������
		extern std::atomic<bool> stopGame;
		stopGame = true;

		extern std::binary_semaphore pingSemaphore;
		extern std::binary_semaphore pongSemaphore;
		//���������� �������
		//����������� �����, ���� �� �������
		pingSemaphore.release();
		//����������� �����, ���� �� �������
		pongSemaphore.release();
	}
//--------------------------------------------------------------------------------------------
//		������� 3
//--------------------------------------------------------------------------------------------
/*
Thread-safe Singletonthread_local
�������� �����, ����������� ������� �������������� ��������.
����� �������� � ����� �������� ����� ���������? � ���?

1 ������� - ���� ������ � ������ �������������� ����������
2 ������� � ��� ������� ������ ���� ���������
*/
	{
	//���� ������ ��� ����� ����������
		std::thread t1(useSingleton);
		std::thread t2(useSingleton);
		t1.join();
		t2.join();

		//���� ������ �� �����
		std::thread t3(useThreadLocalSingleton);
		std::thread t4(useThreadLocalSingleton);
		t3.join();
		t4.join();
	}
//--------------------------------------------------------------------------------------------
//		������� 4
//--------------------------------------------------------------------------------------------
/*
Thread_local, osyncstream. ��������� ���������� thread_local � ����������.
����:thread_local std::string s("hello from "); //��� ������ {} ���������� ������
��������� ������, � ������� � �������� ��������� �������� ���������� ����� ������ (�������, � ������� ������ ���� ��������). � ��������� ������� ������������ s ��������� �������: ��������� (+=) ����� �������. ������� �������������� ������ � ����� s. ��������� ���������, ���� ��������� �� ����� ��������, ���������� ��� osyncstream.
*/
	{
		extern std::string s;

		const int numThreads = 5;

		//��������� ��������� �������
		std::thread threads[numThreads];
		for (size_t i = 0; i < numThreads; i++)
		{
			threads[i] = std::thread(threadFunction,i);
		}
		//���� ���������� ���� �������
		for(auto& t:threads) {
			t.join();
		}
	}
	return 0;
}