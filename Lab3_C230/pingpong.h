#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <conio.h> // Для функции _kbhit()

//Функция ПИНГ
void ping();

//Функция ПОНГ
void pong();

//Функция для отслеживания нажатия клавиши
void monitorInput();