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

//Макрос для базового пути
#define BASE_PATH "C:/Users/nigilist/OneDrive/Desktop/C230/Lab_C230"

// Формирование полного пути к файлу
#define FULL_PATH(file) (std::string(BASE_PATH) + "/" + (file))

// Функция для преобразования текста в верхний регистр и записи в новый файл
void processFile(const std::string& inputFile, const std::string& outputFile);

// Функция для измерения времени выполнения
std::string formatDuration(const std::chrono::nanoseconds& duration);
