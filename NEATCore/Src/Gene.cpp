#include "Gene.hpp"
#include <iostream>

    Gene::Gene(std::shared_ptr<RandomGenerator> generator,
         float weight,
         unsigned innovation,
         unsigned in,
         unsigned out,
         bool is_enabled)

        :generator_(generator)
        ,weight_(weight)
        ,innovation_(innovation)
        ,in_(in)
        ,out_(out)
        ,is_enabled_(is_enabled)
    {
        if(weight < 0)
        {
            weight_ = generator_->get_next();
        }
    }

    Gene::Gene()
        :generator_(std::make_shared<RandomGenerator>())
    {
    }

    bool Gene::operator ==(const Gene& other) const
    {
        return weight_ == other.weight_
                && innovation_ == other.innovation_
                && in_ == other.in_
                && out_ == other.out_
                && is_enabled_ == other.is_enabled_;
    }

Gene::Gene(const Gene& other)
{
    this->generator_ = other.generator_;
    this->weight_ = other.weight_;
    this->innovation_ = other.innovation_;
    this->in_ = other.in_;
    this->out_ = other.out_;
    this->is_enabled_ = other.is_enabled_;
}

Gene& Gene::operator=(const Gene& other)
{
    this->generator_ = other.generator_;
    this->weight_ = other.weight_;
    this->innovation_ = other.innovation_;
    this->in_ = other.in_;
    this->out_ = other.out_;
    this->is_enabled_ = other.is_enabled_;
    return *this;
}

Gene::Gene(Gene&& other)
{
    this->generator_ = std::move(other.generator_);
    this->weight_ = other.weight_;
    this->innovation_ = other.innovation_;
    this->in_ = other.in_;
    this->out_ = other.out_;
    this->is_enabled_ = other.is_enabled_;
}

Gene& Gene::operator=(Gene&& other)
{
    this->generator_ = std::move(other.generator_);
    this->weight_ = other.weight_;
    this->innovation_ = other.innovation_;
    this->in_ = other.in_;
    this->out_ = other.out_;
    this->is_enabled_ = other.is_enabled_;
    return *this;
}

bool Gene::is_enabled() const
{
    return is_enabled_;
}

float Gene::weight() const
{
    return weight_;
}

unsigned Gene::innovation() const
{
    return innovation_;
}

unsigned Gene::in() const
{
    return in_;
}

unsigned Gene::out() const
{
    return out_;
}

void Gene::is_enabled(bool val)
{
   is_enabled_ = val;
}

void Gene::weight(float val)
{
    weight_ = val;
}

void Gene::innovation(unsigned val)
{
    innovation_ = val;
}

void Gene::in(unsigned val)
{
    in_ = val;
}

void Gene::out(unsigned val)
{
    out_ = val;
}

std::ostream& operator<<(std::ostream& stream, const Gene& g)
{
   stream << "[Is enabled: " << std::boolalpha << g.is_enabled_ << ", ";
   stream << "weight: "  << g.weight_ << ", ";
   stream << "innovation: "  << g.innovation_ << ", ";
   stream << "in: "  << g.in_ << ", ";
   stream << "out: "  << g.out_ << "]";
   stream << "\n";
   return stream;
}

