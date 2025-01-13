#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <cctype>
#include <string>
#include <atomic>
#include <algorithm>

//������ ��� �������� ����
#define BASE_PATH "C:/Users/nigilist/OneDrive/Desktop/C230/Lab_C230"

// ������������ ������� ���� � �����
#define FULL_PATH(file) (std::string(BASE_PATH) + "/" + (file))

// ������� ��� �������������� ������ � ������� ������� � ������ � ����� ����
void processFile(const std::string& inputFile, const std::string& outputFile);

// ������� ��� ��������� ������� ����������
std::string formatDuration(const std::chrono::nanoseconds& duration);
