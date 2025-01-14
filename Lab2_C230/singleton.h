#pragma once

#include <iostream>
#include <mutex>

class Singleton {
	static Singleton* instance;
	static std::once_flag initInstanceFlag;
	
	//��������� �����������
	Singleton();
	//��������� ����������
	~Singleton();

public:
	//����� ��� ��������� ������������� ����������
	static Singleton& getInstance();

	//�������� ����������� ����������� � �����������
	Singleton(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator=(Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	void doSomething();

	//����� ��� ������������ ������
	static void destroyInstance();

};

class ThreadLocalSingleton {
private:
	//��������� �����������
	ThreadLocalSingleton();
	//��������� ����������
	~ThreadLocalSingleton();

public:
	// ���������� ����� ��� ��������� ����������
	static ThreadLocalSingleton& getInstance();

	// �������� ����������� � �����������
	ThreadLocalSingleton(const ThreadLocalSingleton&) = delete;
	ThreadLocalSingleton& operator=(const ThreadLocalSingleton&) = delete;
	ThreadLocalSingleton(ThreadLocalSingleton&&) = delete;
	ThreadLocalSingleton& operator=(ThreadLocalSingleton&&) = delete;
	
	void doSomething();
};
//������� ��� ������������� ���������
void useSingleton();

//������� ��� ������������� TLS
void useThreadLocalSingleton();