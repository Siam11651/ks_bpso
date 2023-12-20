#ifndef KNAPSACK_H
#define KNAPSACK_H

#include <iostream>
#include <pso.hpp>

class ks_pair
{
private:
    uint64_t m_weight;
    uint64_t m_profit;

public:
    ks_pair();
    ks_pair(const uint64_t &_weight, const uint64_t &_profit);
    uint64_t &weight();
    const uint64_t &const_weight() const;
    uint64_t &profit();
    const uint64_t &const_profit() const;
};

class ks_problem
{
private:
    std::vector<ks_pair> m_ks_pairs;
    size_t m_capacity;

public:
    ks_problem(std::istream &_istream);
    const std::vector<ks_pair> &const_ks_pairs() const;
    const size_t &const_capacity() const;
};

class ks_particle : public particle<bool, double, uint64_t>
{
private:
    const ks_problem *m_ks_problem_ptr;

public:
    ks_particle(const ks_problem *_ks_problem_ptr);
    ks_particle(const ks_problem *_ks_problem_ptr, const std::vector<bool> &_position,
        const std::vector<double> &_velocity);
    void update_fitness();
};

class ks_swarm : public swarm<bool, double, uint64_t>
{
private:
    double m_vmax;
    double sigmoid(const double &in);

public:
    ks_swarm();
    ks_swarm(const std::vector<ks_particle *> &_particle_ptrs);
    double &vmax();
    const double &vmax() const;
    void update_position();
};

class ks_bpso : public pso<bool, double, uint64_t>
{
protected:
    size_t m_generation_count;

public:
    ks_bpso(ks_swarm *_swarm_ptr);
    ks_bpso(const size_t &_generation_count, ks_swarm *_swarm_ptr);
    size_t &generation_count();
    const size_t &const_generation_count() const;
    virtual void run();
};

class ks_tvbpso : public ks_bpso
{
private:
    size_t m_max_iteration;
    double m_vlow;
    double m_vhigh;

public:
    ks_tvbpso(ks_swarm *_swarm_ptr);
    ks_tvbpso(const size_t &_generation_count, const size_t &_max_iteration, ks_swarm *_swarm_ptr);
    size_t &max_iteration();
    const size_t &const_max_iteration() const;
    double &vlow();
    const double &const_vlow() const;
    double &vhigh();
    const double &const_vhigh() const;
    void run();
};

#endif