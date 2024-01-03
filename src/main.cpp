#include <iostream>
#include <fstream>
#include <chrono>
#include <knapsack.hpp>
#include <random.hpp>

int main()
{
    std::ifstream data_ifstream("datasets/ks_16a.dat");
    ks_problem problem(data_ifstream);

    data_ifstream.close();

    const size_t &capacity = problem.get_capacity();
    const std::vector<ks_pair> &pairs = problem.get_ks_pairs();
    const size_t dimension = pairs.size();
    const size_t swarm_size = 40;

    std::vector<ks_particle> start_particles;

    for(size_t j = 0; j < swarm_size; ++j)
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

    auto allocate_particles = [](std::vector<ks_particle> &_start_particles,
        std::vector<ks_particle *> &_particle_ptrs) -> void
    {
        for(size_t i = 0; i < _particle_ptrs.size(); ++i)
        {
            _particle_ptrs[i] = new ks_particle(_start_particles[i]);
        }
    };
    auto deallocate_particles = [](std::vector<ks_particle *> &_particle_ptrs) -> void
    {
        for(size_t i = 0; i < _particle_ptrs.size(); ++i)
        {
            delete _particle_ptrs[i];
        }
    };

    std::cout << "Starting SBPSO..." << std::endl;

    size_t max_weight_sum = 0;
    std::chrono::steady_clock::time_point start_point = std::chrono::steady_clock::now();

    for(size_t i = 0; i < 30; ++i)
    {
        std::vector<ks_particle *> particle_ptrs(swarm_size);

        allocate_particles(start_particles, particle_ptrs);

        ks_swarm swarm(particle_ptrs);
        swarm.vmax = 4.0;

        swarm.update_fitness();

        ks_bpso bpso(&swarm);
        bpso.generation_count = 500;

        bpso.run();

        max_weight_sum += swarm.best_fitness.value();

        deallocate_particles(particle_ptrs);
    }

    std::chrono::steady_clock::time_point end_point = std::chrono::steady_clock::now();

    std::cout << std::fixed << "SBPSO Average: " << (double)max_weight_sum / 30 << std::endl;
    std::cout << "SBPSO Time: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end_point - start_point).count() / 1e9
        << "s" << std::endl;
    std::cout << std::endl;
    std::cout << "Starting TVSBPSO..." << std::endl;

    max_weight_sum = 0;
    start_point = std::chrono::steady_clock::now();

    for(size_t i = 0; i < 30; ++i)
    {
        std::vector<ks_particle *> particle_ptrs(swarm_size);

        allocate_particles(start_particles, particle_ptrs);

        ks_swarm swarm(particle_ptrs);
        swarm.vmax = 4.0;

        swarm.update_fitness();

        ks_tvbpso tvbpso(&swarm);
        tvbpso.generation_count = 500;
        tvbpso.max_iteration = 2;

        tvbpso.run();

        max_weight_sum += swarm.best_fitness.value();

        deallocate_particles(particle_ptrs);
    }

    end_point = std::chrono::steady_clock::now();

    std::cout << std::fixed << "TVSPBSO Average: " << (double)max_weight_sum / 30 << std::endl;
    std::cout << "TVSBPSO Time: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end_point - start_point).count() / 1e9
        << "s" << std::endl;

    return 0;
}