#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <conio.h> // ��� ������� _kbhit()

//������� ����
void ping();

//������� ����
void pong();

//������� ��� ������������ ������� �������
void monitorInput();