#include "Benchmark.h"
#include <vector>
#include <chrono>
#include <algorithm>

struct Transform
{
    float matrix[16] =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

class GameObject3D
{
public:
    Transform transform;
    int ID{};
};

class GameObject3DAlt
{
public:
    std::vector<Transform> transforms; 
    std::vector<int> IDs;            

    explicit GameObject3DAlt(size_t count)
        : transforms(count), IDs(count, 1)
    {
    }
};


static float AverageWithoutMinMax(std::vector<float>& samples)
{
    if (samples.empty()) return 0.0f;

    if (samples.size() < 3)
    {
        float sum = 0.0f;
        for (float v : samples) sum += v;
        return sum / static_cast<float>(samples.size());
    }

    std::sort(samples.begin(), samples.end());
    samples.erase(samples.begin()); 

    samples.pop_back();            

    float sum = 0.0f;
    for (float v : samples) sum += v;

    return sum / static_cast<float>(samples.size());
}

std::vector<float> RunExercise1Benchmark(int runs)
{
    if (runs < 3) runs = 3;  

    const size_t elementCount = 1'000'000;
    std::vector<int> buffer(elementCount);

    std::vector<float> results;

    results.reserve(11);

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        std::vector<float> samples;
        samples.reserve(runs);

        for (int r = 0; r < runs; ++r)
        {
            const auto start = std::chrono::steady_clock::now();


            for (size_t i = 0; i < buffer.size(); i += stepsize)
            {
                buffer[i] *= 2;

            }

            const auto end = std::chrono::steady_clock::now();


            float ms = std::chrono::duration<float, std::milli>(end - start).count();
            samples.push_back(ms);

        }

        std::sort(samples.begin(), samples.end());
        samples.erase(samples.begin());     

        samples.pop_back();                

        float sum = 0.0f;
        for (float v : samples)
        {
            sum += v;
        }

        float average = sum / static_cast<float>(samples.size());

        results.push_back(average);

    }

    return results;

}

std::vector<float> RunExercise2Benchmark_GameObject3D(int runs)
{


    if (runs < 3) runs = 3;

    const size_t elementCount = 1'000'000;
    std::vector<GameObject3D> buffer(elementCount);
    for (auto& go : buffer) go.ID = 1;

    std::vector<float> results;
    results.reserve(11);

    for (int step = 1; step <= 1024; step *= 2)
    {

        std::vector<float> samples;
        samples.reserve(runs);

        for (int r = 0; r < runs; ++r)
        {
            const auto start = std::chrono::steady_clock::now();

            for (size_t i = 0; i < buffer.size(); i += step)
                buffer[i].ID *= 2;

            const auto end = std::chrono::steady_clock::now();
            samples.push_back(std::chrono::duration<float, std::milli>(end - start).count());
        }

        results.push_back(AverageWithoutMinMax(samples));
    }

    return results;


}


std::vector<float> RunExercise2Benchmark_GameObject3DAlt(int runs)
{
    if (runs < 3) runs = 3;

    const size_t elementCount = 1'000'000;
    GameObject3DAlt buffer(elementCount);

    std::vector<float> results;
    results.reserve(11);

    for (int step = 1; step <= 1024; step *= 2)
    {
        std::vector<float> samples;
        samples.reserve(runs);

        for (int r = 0; r < runs; ++r)
        {
            const auto start = std::chrono::steady_clock::now();

            for (size_t i = 0; i < buffer.IDs.size(); i += step)
                buffer.IDs[i] *= 2;

            const auto end = std::chrono::steady_clock::now();
            samples.push_back(std::chrono::duration<float, std::milli>(end - start).count());
        }

        results.push_back(AverageWithoutMinMax(samples));
    }

    return results;
}