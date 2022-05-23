#include "benchmark.h"

Benchmark::Benchmark(int benchTimeSeconds)
{
	if (benchTimeSeconds > 0 && benchTimeSeconds <= 60)
		m_BenchTimeSeconds = benchTimeSeconds;
}

void Benchmark::RunSingleThread()
{
	unsigned long hashCounter = 0;
	bool finished = false;
	std::thread benchmarkThread = std::thread([&]() { while (!finished) { md5.HashPermutation("bench"); hashCounter++; } });
	std::this_thread::sleep_for(std::chrono::seconds(m_BenchTimeSeconds));
	finished = true;
	benchmarkThread.join();

	resultMHs = hashCounter / (double)m_BenchTimeSeconds / 1000000;
}

void Benchmark::RunMultiThread()
{
	unsigned int nThreads = std::thread::hardware_concurrency();
	unsigned long long hashCounter = 0;
	std::mutex hashCounterMutex;
	bool finished = false;
	
	std::vector<std::thread> threads;
	for (std::size_t i = 0; i < nThreads; i++)
		threads.emplace_back([&]() { while (!finished) { md5.HashPermutation("bench"); std::lock_guard<std::mutex> lock(hashCounterMutex);  hashCounter++; }});

	std::this_thread::sleep_for(std::chrono::seconds(m_BenchTimeSeconds));
	finished = true;

	for (auto& thread : threads)
		thread.join();
	
	resultMHs = hashCounter / (double)m_BenchTimeSeconds / 1000000;
}



double Benchmark::GetResults()
{
	return resultMHs;
}