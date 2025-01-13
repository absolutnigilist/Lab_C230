#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

// Функция, издающая звукb на указанной частоте и интервале времени
void playNote(int frequency, int interval, int count);

// Функция для проигрывания гаммы (от До до Си)
void playScale(int interval, int count);

// Функция для проигрывания аккорда
void playChord(int interval, int count);
