#include "testextras.h"

#include <kpl/chainedarray.h>
#include <vector>
#include <cstdlib>

#include <benchmark/benchmark.h>

static void stdVector50(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<NotDefaultConstructable> testVec;
        //testVec.reserve(50);
        for(std::size_t i = 0; i < 10; i++)
        {
            testVec.push_back( NotDefaultConstructable(i, 'a') );
        }
    }
}

static void chainedArray50(benchmark::State& state)
{
    for (auto _ : state)
    {
        kpl::ChainedArray<NotDefaultConstructable, 10, 40, 10> chainedArray;

        for(std::size_t x = 0; x < 10; x++)
        {
            chainedArray.pushBack( NotDefaultConstructable(x, 'a') );
        }
    }
}

BENCHMARK(stdVector50);
BENCHMARK(chainedArray50);

BENCHMARK_MAIN();
