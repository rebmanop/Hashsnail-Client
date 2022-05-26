#pragma once
#include <thread>
#include <mutex>
#include "algorithms.h"

class Benchmark
{
public:
	Benchmark() = default;
	Benchmark(int benchTimeSeconds);

	void RunSingleThread();
	void RunMultiThread();

	int GetBenchmarkRunTime() const;

	double GetResults() const;

	~Benchmark() = default;
	
public:
	int m_BenchTimeSeconds = 5;

private:
	double resultMHs = 0;
	MD5Handler md5;
};