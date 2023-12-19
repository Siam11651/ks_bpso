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
    size_t max_weight_sum = 0;

    for(size_t i = 0; i < 30; ++i)
    {
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

            particles[j] = new ks_particle(&problem, position, velocity);
        }

        ks_swarm swarm(particles);

        swarm.update_fitness();

        ks_bpso bpso(&swarm);

        bpso.run();

        max_weight_sum += swarm.const_best_fitness().value();

        std::cout << '\t' << swarm.const_best_fitness().value() << std::endl;

        for(size_t i = 0; i < swarm_size; ++i)
        {
            delete particles[i];
        }
    }

    std::cout << std::fixed << "average: " << (double)max_weight_sum / 30 << std::endl;

    return 0;
}