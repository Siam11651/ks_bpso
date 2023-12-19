#ifndef SWARM_H
#define SWARM_H

#include <particle.hpp>

template <typename P, typename V, typename F>
class swarm
{
protected:
    std::vector<particle<P, V, F> *> m_particle_ptrs;
    std::optional<std::vector<P>> m_best_position;
    std::optional<F> m_best_fitness;

public:
    swarm();
    swarm(const std::vector<particle<P, V, F> *> &_particle_ptrs);
    std::optional<F> &best_fitness();
    const std::optional<F> &const_best_fitness() const;
    virtual void update_position() = 0;
    void update_fitness();
};

#endif