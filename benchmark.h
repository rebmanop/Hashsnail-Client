#ifndef _BENCHMARK_H
#define _BENCHMARK_H

#include <thread>
#include "algorithms.h"

class Benchmark
{
public:
	Benchmark() = default;
	Benchmark(int benchTimeSeconds);

	void RunBenchmark();
	double GetResults();

	~Benchmark() = default;

private:
	double resultHs = 0;
	int m_BenchTimeSeconds = 5;
	MD5Handler md5;
};


#endif