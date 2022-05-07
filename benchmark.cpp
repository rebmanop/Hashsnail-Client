#include "benchmark.h"

Benchmark::Benchmark(int benchTimeSeconds)
{
	if (benchTimeSeconds > 0 && benchTimeSeconds <= 60)
		m_BenchTimeSeconds = benchTimeSeconds;
}

void Benchmark::RunBenchmark()
{
	unsigned long hashCounter = 0;
	bool finished = false;
	std::thread benchmarkThread = std::thread([&]() { while (!finished) { md5.HashPermutation("bench"); hashCounter++; } });
	std::this_thread::sleep_for(std::chrono::seconds(m_BenchTimeSeconds));
	finished = true;
	benchmarkThread.join();

	resultHs = hashCounter / (double)m_BenchTimeSeconds;
}

double Benchmark::GetResults()
{
	return resultHs;
}