#include "threadsafe.h"
// Флаг для остановки
std::atomic<bool> stopConsumer(false);

//Пример использования
void producer(threadsafe_stack<int>& stack, int start, int count, int delay_ms) {
	for (size_t i = 0; i < count; ++i)
	{
		stack.push(start + i);
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
	}
}
void consumer(threadsafe_stack<int>& stack, int delay_ms) {
	while (!stopConsumer)
	{
		std::optional<int>value = stack.pop_optional();
		if (value)
		{
			std::cout << "Consumer popped " << *value << std::endl;
		}
		else
		{
			std::cout << "Stack is empty, waiting..." << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

	}
	std::cout << "Consumer is stopping...\n" << std::endl;
}