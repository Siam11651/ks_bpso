#ifndef SWARM_H
#define SWARM_H

#include <particle.hpp>

template <typename P, typename V, typename F>
class swarm
{
protected:
    std::vector<particle<P, V, F> *> m_particle_ptrs;
    std::optional<std::vector<P>> m_best_position;

public:
    std::optional<F> best_fitness;

    swarm() {}

    swarm(const std::vector<particle<P, V, F> *> &_particle_ptrs) :
    m_particle_ptrs(_particle_ptrs) {}

    virtual void update_position() = 0;

    void update_fitness()
    {
        for(particle<P, V, F> *item : m_particle_ptrs)
        {
            item->update_fitness();

            if(!best_fitness.has_value() || item->best_fitness > best_fitness)
            {
                best_fitness = std::max(best_fitness, item->best_fitness);
                m_best_position = item->best_position;
            }
        }
    }
};

#endif