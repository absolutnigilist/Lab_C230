#include "singleton.h"

// Инициализация статических членов
Singleton* Singleton::instance = nullptr;
std::once_flag Singleton::initInstanceFlag;

Singleton::Singleton() {
    std::cout << "Singleton created.\n";
}

Singleton::~Singleton() {
    std::cout << "Singleton destroyed.\n";
}

Singleton& Singleton::getInstance() {
    std::call_once(initInstanceFlag, []() {
        instance = new Singleton();
        });
    return *instance;
}

void Singleton::doSomething() {
    std::cout << "Singleton instance: " << this << "\n";
}

void Singleton::destroyInstance() {
    delete instance;
    instance = nullptr;

}

ThreadLocalSingleton::ThreadLocalSingleton() {
    std::cout << "Thread-local Singleton created in thread: "
        << std::this_thread::get_id() << "\n";
}

ThreadLocalSingleton::~ThreadLocalSingleton() {
    std::cout << "Thread-local Singleton destroyed in thread: "
        << std::this_thread::get_id() << "\n";
}

ThreadLocalSingleton& ThreadLocalSingleton::getInstance() {
    thread_local ThreadLocalSingleton instance;
    return instance;
}
void ThreadLocalSingleton::doSomething() {
    std::cout << "Thread-local Singleton instance: " << this
        << " in thread: " << std::this_thread::get_id() << "\n";
}


void useSingleton() {
    Singleton::getInstance().doSomething();
}

void useThreadLocalSingleton() {
    ThreadLocalSingleton::getInstance().doSomething();
}
