#include <iostream>
#include <fstream>
#include <knapsack.hpp>
#include <random.hpp>

int main()
{
    std::ifstream data_ifstream("datasets/ks16a.txt");
    ks_problem problem(data_ifstream);
    const size_t &capacity = problem.const_capacity();
    const std::vector<ks_pair> &pairs = problem.const_ks_pairs();
    const size_t dimension = pairs.size();
    const size_t swarm_size = 40;
    std::vector<ks_particle *> particles(swarm_size);

    for(size_t i = 0; i < swarm_size; ++i)
    {
        std::vector<bool> position(dimension);
        std::vector<double> velocity(dimension);

        while(true)
        {
            size_t weight_sum = 0;

            for(size_t j = 0; j < dimension; ++j)
            {
                position[j] = random::get_bool();

                if(position[j])
                {
                    weight_sum += pairs[j].const_weight();
                }
            }

            if(weight_sum <= capacity)
            {
                break;
            }
        }

        for(size_t j = 0; j < dimension; ++j)
        {
            velocity[j] = random::get_bool();
        }

        particles[i] = new ks_particle(&problem, position, velocity);
    }

    ks_swarm swarm(particles);

    swarm.update_fitness();

    ks_bpso bpso(&swarm);

    for(size_t i = 0; i < 30; ++i)
    {
        bpso.run();

        std::cout << swarm.const_best_fitness().value() << std::endl;
    }

    for(size_t i = 0; i < swarm_size; ++i)
    {
        delete particles[i];
    }

    return 0;
}