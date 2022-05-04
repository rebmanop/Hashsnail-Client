#include "benchmark.h"

Benchmark::Benchmark(int benchTimeSeconds)
{
	if (benchTimeSeconds > 0 && benchTimeSeconds <= 60)
		m_BenchTimeSeconds = benchTimeSeconds;
}

void Benchmark::RunBenchmark()
{
	unsigned long hashCounter = 0;
	bool Finished = false;
	std::thread benchmarkThread = std::thread([&]() { while (!Finished) { md5.HashPermutation("1234567"); hashCounter++; } });
	std::this_thread::sleep_for(std::chrono::seconds(m_BenchTimeSeconds));
	Finished = true;
	benchmarkThread.join();

	resultMhs = (hashCounter / (double)m_BenchTimeSeconds) / 1000000;
}

double Benchmark::GetResults()
{
	return resultMhs;
}

