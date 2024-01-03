#ifndef PARTICLE_H
#define PARTICLE_H

#include <cstdint>
#include <vector>
#include <optional>

template <typename P, typename V, typename F>
class particle
{
public:
    std::vector<P> position;
    std::optional<std::vector<P>> best_position;
    std::vector<V> velocity;
    std::optional<F> best_fitness;

    particle() {}
    
    particle(const std::vector<P> &_position, const std::vector<V> &_velocity) :
        position(_position),
        velocity(_velocity) {}

    virtual void update_fitness() = 0;
};

#endif