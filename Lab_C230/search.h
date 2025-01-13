#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <thread>
//#include <jthread>
#include <atomic>
#include <algorithm>
#include <chrono>

//Функция поиска в заданном диапазоне
void searchInRange(const std::vector<int>& vec, size_t start, size_t end, int target, std::atomic<bool>& found);
