#ifndef NET_FRAME_THREAD_POOL_H
#define NET_FRAME_THREAD_POOL_H

#include "./msgQueue.h"

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#define MIN_THREADS 10


template<class Type>
class ThreadPool {
	ThreadPool& operator=(const ThreadPool&) = delete;

	ThreadPool(const ThreadPool& other) = delete;

public:
	ThreadPool(int32_t threads, std::function<void(Type& record)> handler);

	virtual ~ThreadPool();

	void Submit(Type record);

private:

private:
	bool _shutdown;
	int32_t _threads;
	std::function<void(Type& record)> _handler;
	std::vector <std::thread> _workers;
	ConcurrentQueue <Type> _tasks;
};




template<class Type>
ThreadPool<Type>::ThreadPool(int32_t threads, std::function<void(Type &record)> handler)
	: _shutdown(false),
	_threads(threads),
	_handler(handler),
	_workers(),
	_tasks() {
	if (_threads < MIN_THREADS)
		_threads = MIN_THREADS;


	for (int32_t i = 0; i < _threads; ++i)
		_workers.emplace_back(
			[this] {
		while (!_shutdown) {
			Type record;
			_tasks.Pop(record, true);
			_handler(record);
		}
	}
	);
}


template<class Type>
ThreadPool<Type>::~ThreadPool() {
	for (std::thread &worker : _workers)
		worker.join();
}


template<class Type>
void ThreadPool<Type>::Submit(Type record) {
	_tasks.Push(record);
}




#endif //NET_FRAME_THREAD_POOL_H