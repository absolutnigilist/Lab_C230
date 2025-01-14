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
	//��������� ������
	std::vector<T> stack_data;
	//������� ��� �������������
	mutable std::mutex mtx;
public:
	threadsafe_stack() = default;
	~threadsafe_stack() = default;

	//����������� �����������
	threadsafe_stack(const threadsafe_stack& other) {
		std::lock_guard<std::mutex> lock(mtx, other.mtx);
		stack_data = other.stack_data;
	}
	//�������� ������������
	threadsafe_stack& operator=(const threadsafe_stack& other) {
		if (this!=&other)
		{
			std::scoped_lock(mtx, other.mtx);
			stack_data = other.stack_data;
		}
		return *this;
	}
	//���������� ��������
	void push(T value) {
		std::lock_guard<std::mutex>lock(mtx);
		stack_data.push_back(std::move(value));
	}
	//�������� � ������� �������� ����� ������
	void pop(T& val) {
		std::lock_guard<std::mutex> lock(mtx);
		if (stack_data.empty())
		{
			throw std::runtime_error("������� ���������� �� ������� �����\n");
		}
		val = std::move(stack_data.back());
		stack_data.pop_back();

	}
	//�������� � ������� �������� ����� shared_ptr
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
	//�������� � ������� �������� ����� std::optional
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
		//�������� �� �������
		bool empty()const {
			std::lock_guard<std::mutex>lock(mtx);
			return stack_data.empty();
		}
		//��������� ������� �����
		size_t size()const {
			std::lock_guard<std::mutex>lock(mtx);
			return stack_data.size();
		}
};
//������� �������������
void producer(threadsafe_stack<int>& stack, int start, int count, int delay_ms);

//������� �����������
void consumer(threadsafe_stack<int>& stack, int delay_ms);
