#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <conio.h> // ��� _kbhit()

//������� ��� ���������� ������
void threadFunction(const std::string& str, std::atomic<bool>& running);