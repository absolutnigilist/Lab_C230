#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <random>

// ������� ��� ���������� ��������� � ������
void addValues(int threadId, int limit);