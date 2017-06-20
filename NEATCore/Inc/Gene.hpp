#pragma once

#include <memory>
#include <functional>

#include "RandomGenerator.hpp"

class Gene
{
public:


    Gene();
    Gene(const Gene&);
    Gene& operator=(const Gene&);
    Gene(std::shared_ptr<RandomGenerator>,
         float weight = 0,
         unsigned innovation = 1,
         unsigned in = 0,
         unsigned out = 0,
         bool is_enabled = true);

    Gene(Gene&&);
    Gene& operator=(Gene&&);

    bool operator==(const Gene&) const;

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
    std::shared_ptr<RandomGenerator> generator_;
    float weight_ = 0.0f;
    unsigned innovation_ = 0;
    unsigned in_ = 0;
    unsigned out_ = 0;
    bool is_enabled_ = true;
    friend std::ostream& operator<<(std::ostream& stream, const Gene&);
};
