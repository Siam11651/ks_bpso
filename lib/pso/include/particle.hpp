#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <vector>
#include <optional>

template <typename P, typename V, typename F>
class particle
{
protected:
    std::vector<P> m_position;
    std::optional<std::vector<P>> m_best_position;
    std::vector<V> m_velocity;
    std::optional<F> m_best_fitness;

public:
    particle();
    particle(const std::vector<P> &_position, const std::vector<V> &_velocity);
    std::vector<P> &position();
    const std::vector<P> &const_position() const;
    std::optional<std::vector<P>> &best_position();
    const std::optional<std::vector<P>> &const_best_position() const;
    std::vector<V> &velocity();
    const std::vector<V> &const_velocity() const;
    std::optional<F> &best_fitness();
    const std::optional<F> &const_best_fitness() const;
    virtual void update_fitness() = 0;
};

#endif