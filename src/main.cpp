#include <iostream>
#include <fstream>
#include <knapsack.hpp>
#include <random.hpp>

int main()
{
    std::ifstream data_ifstream("datasets/ks16a.txt");
    ks_problem problem(data_ifstream);

    data_ifstream.close();

    const size_t &capacity = problem.const_capacity();
    const std::vector<ks_pair> &pairs = problem.const_ks_pairs();
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
                    weight_sum += pairs[k].const_weight();
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

    size_t max_weight_sum = 0;

    for(size_t i = 0; i < 30; ++i)
    {
        std::vector<ks_particle *> particle_ptrs(swarm_size);

        allocate_particles(start_particles, particle_ptrs);

        ks_swarm swarm(particle_ptrs);
        swarm.vmax() = 4.0;

        swarm.update_fitness();

        ks_bpso bpso(&swarm);
        bpso.generation_count() = 500;

        bpso.run();

        max_weight_sum += swarm.const_best_fitness().value();

        deallocate_particles(particle_ptrs);
    }

    std::cout << std::fixed << "SBPSO Average: " << (double)max_weight_sum / 30 << std::endl;

    max_weight_sum = 0;

    for(size_t i = 0; i < 30; ++i)
    {
        std::vector<ks_particle *> particle_ptrs(swarm_size);

        allocate_particles(start_particles, particle_ptrs);

        ks_swarm swarm(particle_ptrs);
        swarm.vmax() = 4.0;

        swarm.update_fitness();

        ks_tvbpso tvbpso(&swarm);
        tvbpso.generation_count() = 500;
        tvbpso.max_iteration() = 10;

        tvbpso.run();

        max_weight_sum += swarm.const_best_fitness().value();

        deallocate_particles(particle_ptrs);
    }

    std::cout << std::fixed << "TVSPBSO Average: " << (double)max_weight_sum / 30 << std::endl;

    return 0;
}