#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <chrono>

//Функция для преобразования диапазона контейнера
template<typename Iterator, typename UnaryOperation>
void parallelTransform(Iterator begin, Iterator end, Iterator out, UnaryOperation op, int numThreads) {
	auto totalElements = std::distance(begin, end);
	int blockSize = totalElements / numThreads;
	int remainder = totalElements % numThreads;

	std::vector<std::thread>threads;
	//Создаем потоки для всех, кроме первичного
	threads.reserve(numThreads - 1);

	auto start = begin;
	for (size_t i = 0; i < numThreads; ++i)
	{
		auto finish = std::next(start, blockSize);
		threads.emplace_back([=]() {
			std::transform(start, finish, out, op);
			}
		);
		std::advance(out, blockSize);
		start = finish;
	}
	//Обрабатываем оставшиейся элменты (остаток + последний блок)
	std::transform(start, end, out, op);

	//Ждем завершения всех потоков
	for (auto& thread : threads) {
		if (thread.joinable())
		{
			thread.join();
		}
	}

}