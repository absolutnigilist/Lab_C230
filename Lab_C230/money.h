#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <stop_token>
#include <chrono>

//Функци для выполнения работы потоком
void withDrawMoney(int threadId, int withdrawAmount, int timeoutMs, std::stop_token stopToken);