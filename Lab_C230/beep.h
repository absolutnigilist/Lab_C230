#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

// �������, �������� ����b �� ��������� ������� � ��������� �������
void playNote(int frequency, int interval, int count);

// ������� ��� ������������ ����� (�� �� �� ��)
void playScale(int interval, int count);

// ������� ��� ������������ �������
void playChord(int interval, int count);
