#include <Windows.h>

#include <assert.h>
#include <atomic>
#include <exception>
#include <list>
#include <mutex>
#include <stdlib.h>
#include <string>
#include <thread>

#define NUM_THREADS 4
#define COUNT_PER_THREAD 1000000

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::list<std::thread> threads;

	std::atomic<int> i_counter(0);

	std::string s_counter = "0";
	std::mutex s_mutex;

	for(int j = 0; j < NUM_THREADS; ++j)
	{
		int thread_id = j;

		threads.emplace_back([&i_counter, &s_counter, &s_mutex, thread_id]()
		{
			try {
				for(int i = 0; i < COUNT_PER_THREAD; ++i)
				{
					++i_counter;

					std::lock_guard<std::mutex> lock(s_mutex);
					s_counter = std::to_string(atoi(s_counter.c_str()) + 1);
				}

				throw std::runtime_error("thread " + std::to_string(thread_id));
			}
			catch(...) {}
		});
	}

	while(!threads.empty())
	{
		threads.front().join();
		threads.pop_front();
	}

	assert(i_counter == (NUM_THREADS * COUNT_PER_THREAD));
	assert(s_counter == std::to_string(NUM_THREADS * COUNT_PER_THREAD));

	MessageBox(NULL, L"Hello from 2024!", L"Hello", MB_OK);
	return 0;
}
