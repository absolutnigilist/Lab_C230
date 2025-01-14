#include <vector>
#include <mutex>
#include <memory>
#include <optional>
#include <stdexcept>
#include <iostream>
#include <thread>
#include <chrono>

template<typename T>
class threadsafe_stack {
	//Хранилище данных
	std::vector<T> stack_data;
	//Мьютекс для синхронизации
	mutable std::mutex mtx;
public:
	threadsafe_stack() = default;
	~threadsafe_stack() = default;

	//Конструктор копирования
	threadsafe_stack(const threadsafe_stack& other) {
		std::lock_guard<std::mutex> lock(mtx, other.mtx);
		stack_data = other.stack_data;
	}
	//Оператор присваивания
	threadsafe_stack& operator=(const threadsafe_stack& other) {
		if (this!=&other)
		{
			std::scoped_lock(mtx, other.mtx);
			stack_data = other.stack_data;
		}
		return *this;
	}
	//Добавление элемента
	void push(T value) {
		std::lock_guard<std::mutex>lock(mtx);
		stack_data.push_back(std::move(value));
	}
	//Удаление и возврат элемента через ссылку
	void pop(T& val) {
		std::lock_guard<std::mutex> lock(mtx);
		if (stack_data.empty())
		{
			throw std::runtime_error("Попытка извлечения из пустого стека\n");
		}
		val = std::move(stack_data.back());
		stack_data.pop_back();

	}
	//Удаление и возврат элемента через shared_ptr
	std::shared_ptr<T> pop(){
		std::lock_guard<std::mutex> lock(mtx);
		if (stack_data.empty())
		{
			return nullptr;
		}
		std::shared_ptr<T> result = std::make_shared<T>(std::move(stack_data.back()));
		stack_data.pop_back();
		return result;
	}
	//Удаление и возврат элемента через std::optional
		std::optional<T> pop_optional() {
			std::lock_guard<std::mutex> lock(mtx);
			if (stack_data.empty())
			{
				return std::nullopt;
			}
			T result = std::move(stack_data.back());
			stack_data.pop_back();
			return result;
	}
		//Проверка на пустоту
		bool empty()const {
			std::lock_guard<std::mutex>lock(mtx);
			return stack_data.empty();
		}
		//Получение размера стека
		size_t size()const {
			std::lock_guard<std::mutex>lock(mtx);
			return stack_data.size();
		}
};
//Функция производитель
void producer(threadsafe_stack<int>& stack, int start, int count, int delay_ms);

//Функция потребитель
void consumer(threadsafe_stack<int>& stack, int delay_ms);
