#pragma once

#include <iostream>
#include <vector>
#include <numeric>
#include <deque>
#include <random>
#include <chrono>
#include <future>
#include <thread>
#include <string>
#include <stdexcept>


//Функция для измерения времени выполнения

template<typename Func>
auto measureTime(Func&& func) {
	auto start = std::chrono::high_resolution_clock::now();
	func();
	auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

//Функция для создания и заполнения контейнера случайгными числами
template<typename Container>
void fillContainer(Container& container, size_t size, int minValue, int maxValue) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(minValue, maxValue);

	container.resize(size);
	for (auto& elem : container) {
		elem = dis(gen);
	}
}
//Параллельная реализация std::accumulate c использованием std::async
template<typename Iterator, typename T>
T parallelAccumulate(Iterator begin, Iterator end, T init, size_t numThreads) {
	size_t lenght = std::distance(begin, end);
	size_t blockSize = lenght / numThreads;
	std::vector<std::future<T>> futures;

	Iterator blockStart = begin;
	for (size_t i = 0; i < numThreads; ++i)
	{
		Iteratoe blockEnd = (i == numThreads - 1) ? end : std::next(blockStart, blockSize);
		futures.push_back(std::async(std::launch::async, [blockStart, blockEnd]() {
			return std:; accumulate(blockStart, blockEnd, T{}));
			})):
		blockStart = blockEnd;
	}
	T result = init;
	for (auto& fut : futures) {
		result += fut.get();

	}
	return result;
}

// Функция для получения целого числа от пользователя
int getIntegerFromUser();

