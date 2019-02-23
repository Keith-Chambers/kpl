#include "testextras.h"

#include <kpl/chainedarray.h>
#include <vector>
#include <cstdlib>

#include <benchmark/benchmark.h>

struct Pixel
{
    Pixel()
    {
    }

    Pixel(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b)
    {
    }

    unsigned char r, g, b;
};

static void UseVector(benchmark::State& state)
{
    
    for (auto _ : state)
    {
        int dimension = 250;

        std::vector<Pixel> pixels;
        pixels.resize(dimension * dimension);

        for(int i = 0; i < dimension * dimension; ++i)
        {
            pixels[i].r = 255;
            pixels[i].g = 0;
            pixels[i].b = 0;
        }
    }
}

static void UseArray(benchmark::State& state)
{
    for (auto _ : state)
    {
        int dimension = 250;

        Pixel * pixels = (Pixel *)malloc(sizeof(Pixel) * dimension * dimension);

        for(int i = 0 ; i < dimension * dimension; ++i)
        {
            pixels[i].r = 255;
            pixels[i].g = 0;
            pixels[i].b = 0;
        }

        free(pixels);
    }
}

static void UseVectorPushBack(benchmark::State& state)
{
    for (auto _ : state)
    {
        int dimension = 250;

        std::vector<Pixel> pixels;
            pixels.reserve(dimension * dimension);

        for(int i = 0; i < dimension * dimension; ++i)
            pixels.push_back(Pixel(255, 0, 0));
    }
}

static void UseFullyStackAllocatedChainedArray(benchmark::State& state)
{
    for (auto _ : state)
    {
        constexpr int dimension = 250;
        
        kpl::ChainedArray<Pixel, 250 * 251, 0, 0> chainedArray;
        Pixel * tmp;

        for(int i = 0; i < dimension * dimension; ++i)
        {
            chainedArray.emplaceBack(255, 0, 0);
            
            /*
            tmp = chainedArray.setNext();
            tmp->r = 255;
            tmp->g = 0;
            tmp->b = 0; */
        }
        
        //chainedArray.emplaceBack(255, 0, 0);
    }
}

static void stdVector50(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<NotDefaultConstructable> testVec;
        testVec.reserve(20);
        for(std::size_t i = 0; i < 50; i++)
        {
            testVec.push_back( NotDefaultConstructable(i, 'a') );
        }
    }
}

static void chainedArray50(benchmark::State& state)
{
    for (auto _ : state)
    {
        kpl::ChainedArray<NotDefaultConstructable, 50, 40, 10> chainedArray;
        kpl::array<NotDefaultConstructable> * array = chainedArray.asArray();

        for(std::size_t x = 0; x < 50; x++)
        {
            array->pushBack( NotDefaultConstructable(x, 'a') );
        }
    }
}

static void chainedArray50MiddleInsertion(benchmark::State& state)
{
    for (auto _ : state)
    {
        kpl::ChainedArray<NotDefaultConstructable, 15, 30, 2> chainedArray;
        kpl::array<NotDefaultConstructable> * array = chainedArray.asArray();

        for(std::size_t x = 0; x < 25; x++)
        {
            array->pushBack( NotDefaultConstructable(x, 'a') );
        }
        
        //for(uint16_t i = 0; i < 20; i++)
        //    array->insertAt( NotDefaultConstructable(i, 'a'), i );
    }
}

static void stdVector50MiddleInsertion(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<NotDefaultConstructable> testVec;
        testVec.reserve(40);
        
        for(std::size_t i = 0; i < 25; i++)
        {
            testVec.push_back( NotDefaultConstructable(i, 'a') );
        }
        
        //for(uint16_t i = 0; i < 20; i++)
        //    testVec.insert( testVec.begin() + i, NotDefaultConstructable(0, 'a'));
    }
}

//BENCHMARK(UseArray);
//BENCHMARK(UseVector);
//BENCHMARK(UseVectorPushBack);
//BENCHMARK(UseFullyStackAllocatedChainedArray);

//BENCHMARK(stdVector50);
//BENCHMARK(chainedArray50);

BENCHMARK(stdVector50MiddleInsertion);
BENCHMARK(chainedArray50MiddleInsertion);

BENCHMARK_MAIN();
