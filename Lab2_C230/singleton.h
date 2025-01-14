#pragma once

#include <iostream>
#include <mutex>

class Singleton {
	static Singleton* instance;
	static std::once_flag initInstanceFlag;
	
	//Дефолтный конструктор
	Singleton();
	//Дефолтный деструктор
	~Singleton();

public:
	//Метод для получения единственного экземпляра
	static Singleton& getInstance();

	//Удаление возможности копирования и перемещения
	Singleton(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator=(Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	void doSomething();

	//Метод для освобождения памяти
	static void destroyInstance();

};

class ThreadLocalSingleton {
private:
	//Дефолтный конструктор
	ThreadLocalSingleton();
	//Дефолтный деструктор
	~ThreadLocalSingleton();

public:
	// Статически метод для получения экземпляра
	static ThreadLocalSingleton& getInstance();

	// Удаление копирования и перемещения
	ThreadLocalSingleton(const ThreadLocalSingleton&) = delete;
	ThreadLocalSingleton& operator=(const ThreadLocalSingleton&) = delete;
	ThreadLocalSingleton(ThreadLocalSingleton&&) = delete;
	ThreadLocalSingleton& operator=(ThreadLocalSingleton&&) = delete;
	
	void doSomething();
};
//Функция для использования синглтона
void useSingleton();

//Функция для использования TLS
void useThreadLocalSingleton();