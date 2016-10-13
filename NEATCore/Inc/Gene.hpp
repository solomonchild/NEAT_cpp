#pragma once

#include <memory>
#include <functional>

#include "RandomGenerator.hpp"

class Gene
{
public:

    Gene(const RandomGenerator&);

    Gene(const Gene&);
    Gene& operator=(const Gene&);

    Gene(Gene&&);
    Gene& operator=(Gene&&);
    ~Gene();

    bool is_enabled() const;
    void is_enabled(bool);

    float weight() const;
    void weight(float);

    unsigned innovation() const;
    void innovation(unsigned);

    unsigned in() const;
    void in(unsigned);

    unsigned out() const;
    void out(unsigned);



private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

};
