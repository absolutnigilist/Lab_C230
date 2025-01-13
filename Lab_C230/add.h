#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <random>

// Функция для добавления элементов в вектор
void addValues(int threadId, int limit);