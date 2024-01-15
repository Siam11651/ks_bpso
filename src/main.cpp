#include <iostream>
#include <fstream>
#include <chrono>
#include <array>
#include <filesystem>
#include <knapsack.hpp>
#include <random.hpp>

const std::string DATASET_NAME = "ks_16a";
constexpr size_t SWARM_SIZE = 40;
constexpr size_t MAX_GENERATIONS = 1;
constexpr size_t MAX_ITERATIONS = 500;
constexpr double PARAM = 10.0;
constexpr size_t TRIALS = 30;
constexpr size_t RUNS = 10;

void allocate_particles(std::vector<ks_particle> &_start_particles, std::vector<ks_particle *> &_particle_ptrs)
{
    for(size_t i = 0; i < _particle_ptrs.size(); ++i)
    {
        _particle_ptrs[i] = new ks_particle(_start_particles[i]);
    }
}

void deallocate_particles(std::vector<ks_particle *> &_particle_ptrs)
{
    for(size_t i = 0; i < _particle_ptrs.size(); ++i)
    {
        delete _particle_ptrs[i];
    }
}

int main()
{
    std::ifstream data_ifstream("datasets/" + DATASET_NAME + ".dat");
    ks_problem problem(data_ifstream);

    data_ifstream.close();

    const size_t &capacity = problem.get_capacity();
    const std::vector<ks_pair> &pairs = problem.get_ks_pairs();
    const size_t dimension = pairs.size();
    std::array<double, RUNS> sbpso_data;
    std::array<double, RUNS> tvsbpso_data;
    std::array<double, RUNS> ntvsbpso_data;

    for(size_t i = 0; i < RUNS; ++i)
    {
        std::clog << "=== Start run " << i + 1 << " ===" << std::endl;

        std::vector<ks_particle> start_particles;

        for(size_t j = 0; j < SWARM_SIZE; ++j)
        {
            std::vector<bool> position(dimension);
            std::vector<double> velocity(dimension);

            while(true)
            {
                size_t weight_sum = 0;

                for(size_t k = 0; k < dimension; ++k)
                {
                    position[k] = random::get_bool();

                    if(position[k])
                    {
                        weight_sum += pairs[k].weight;
                    }
                }

                if(weight_sum <= capacity)
                {
                    break;
                }
            }

            for(size_t k = 0; k < dimension; ++k)
            {
                velocity[k] = random::get_bool();
            }

            start_particles.emplace_back(&problem, position, velocity);
        }

        std::clog << "Starting SBPSO..." << std::endl;

        size_t max_weight_sum = 0;
        std::chrono::steady_clock::time_point start_point = std::chrono::steady_clock::now();

        for(size_t j = 0; j < TRIALS; ++j)
        {
            std::vector<ks_particle *> particle_ptrs(SWARM_SIZE);

            allocate_particles(start_particles, particle_ptrs);

            ks_swarm swarm(particle_ptrs);
            swarm.vmax = 4.0;

            swarm.update_fitness();

            ks_bpso bpso(&swarm);
            bpso.generation_count = MAX_ITERATIONS * MAX_GENERATIONS;

            bpso.run();

            max_weight_sum += swarm.best_fitness.value();

            deallocate_particles(particle_ptrs);
        }

        std::chrono::steady_clock::time_point end_point = std::chrono::steady_clock::now();
        sbpso_data[i] = (double)max_weight_sum / TRIALS;

        std::clog << '\t' << std::fixed << "Average: " << sbpso_data[i] << std::endl;
        std::clog << '\t' << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_point - start_point).count() / 1e9 << "s" << std::endl;
        std::clog << "Starting TVSBPSO..." << std::endl;

        max_weight_sum = 0;
        start_point = std::chrono::steady_clock::now();

        for(size_t j = 0; j < TRIALS; ++j)
        {
            std::vector<ks_particle *> particle_ptrs(SWARM_SIZE);

            allocate_particles(start_particles, particle_ptrs);

            ks_swarm swarm(particle_ptrs);

            swarm.update_fitness();

            ks_tvbpso tvbpso(&swarm);
            tvbpso.generation_count = MAX_GENERATIONS;
            tvbpso.max_iteration = MAX_ITERATIONS;

            tvbpso.run();

            max_weight_sum += swarm.best_fitness.value();

            deallocate_particles(particle_ptrs);
        }

        end_point = std::chrono::steady_clock::now();
        tvsbpso_data[i] = (double)max_weight_sum / TRIALS;

        std::clog << '\t' << std::fixed << "Average: " << tvsbpso_data[i] << std::endl;
        std::clog << '\t' << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_point - start_point).count() / 1e9 << "s" << std::endl;
        std::clog << "Starting NTVSBPSO..." << std::endl;

        max_weight_sum = 0;
        start_point = std::chrono::steady_clock::now();

        for(size_t j = 0; j < TRIALS; ++j)
        {
            std::vector<ks_particle *> particle_ptrs(SWARM_SIZE);

            allocate_particles(start_particles, particle_ptrs);

            ks_swarm swarm(particle_ptrs);

            swarm.update_fitness();

            ks_ntvbpso ntvbpso(&swarm);
            ntvbpso.generation_count = MAX_GENERATIONS;
            ntvbpso.max_iteration = MAX_ITERATIONS;
            ntvbpso.param = PARAM;

            ntvbpso.run();

            max_weight_sum += swarm.best_fitness.value();

            deallocate_particles(particle_ptrs);
        }

        end_point = std::chrono::steady_clock::now();
        ntvsbpso_data[i] = (double)max_weight_sum / TRIALS;

        std::clog << '\t' << std::fixed << "Average: " << ntvsbpso_data[i] << std::endl;
        std::clog << '\t' << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_point - start_point).count() / 1e9 << "s" << std::endl;

        std::clog << "=== End run " << i + 1 << " ===" << std::endl;
        std::clog << std::endl;
    }

    std::filesystem::create_directory("report");

    std::ofstream report_ofstream("report/" + DATASET_NAME + ".csv");

    report_ofstream << "Dataset Name: " << DATASET_NAME << std::endl;
    report_ofstream << "Swarm Size: " << SWARM_SIZE << std::endl;
    report_ofstream << "Max Generations: " << MAX_GENERATIONS << std::endl;
    report_ofstream << "Max Iterations: " << MAX_ITERATIONS << std::endl;
    report_ofstream << "Param: " << std::fixed << PARAM << std::endl;
    report_ofstream << "Trial: " << TRIALS << std::endl;
    report_ofstream << "Runs: " << RUNS << std::endl;
    report_ofstream << std::endl;
    report_ofstream << "SBPSO,TVSBPSO,NTVSBPSO" << std::endl;

    for(size_t i = 0; i < RUNS; ++i)
    {
        report_ofstream << std::fixed << sbpso_data[i] << ',' << tvsbpso_data[i] << ',' << ntvsbpso_data[i] << std::endl;
    }

    report_ofstream.close();

    return 0;
}