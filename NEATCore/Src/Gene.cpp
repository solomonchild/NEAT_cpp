#include "Gene.hpp"
#include <iostream>

struct Gene::Impl
{
    Impl(std::shared_ptr<RandomGenerator> generator,
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

    Impl()
        :generator_(std::make_shared<RandomGenerator>())
    {
    }

    std::shared_ptr<RandomGenerator> generator_;
    float weight_ = 0.0f;
    unsigned innovation_ = 0;
    unsigned in_ = 0;
    unsigned out_ = 0;
    bool is_enabled_ = true;
};



Gene::Gene()
    :impl_(new Impl)
{
}

Gene::Gene(std::shared_ptr<RandomGenerator> generator,
         float weight,
         unsigned innovation,
         unsigned in,
         unsigned out,
         bool is_enabled)
:impl_(new Impl(generator, weight, innovation, in, out, is_enabled))
{
}

Gene::Gene(const Gene& other)
{
   this->impl_ = std::make_unique<Impl>(*other.impl_);
}

Gene& Gene::operator=(const Gene& other)
{
    this->impl_ = std::make_unique<Impl>(*other.impl_);
    return *this;
}

Gene::Gene(Gene&& other)
{
    this->impl_ = std::make_unique<Impl>(std::move(*other.impl_));
}

Gene& Gene::operator=(Gene&& other)
{
    this->impl_ = std::make_unique<Impl>(std::move(*other.impl_));
    return *this;
}

Gene::~Gene() = default;

bool Gene::is_enabled() const
{
    return impl_->is_enabled_;
}

float Gene::weight() const
{
    return impl_->weight_;
}

unsigned Gene::innovation() const
{
    return impl_->innovation_;
}

unsigned Gene::in() const
{
    return impl_->in_;
}

unsigned Gene::out() const
{
    return impl_->out_;
}

void Gene::is_enabled(bool val)
{
   impl_->is_enabled_ = val;
}

void Gene::weight(float val)
{
    impl_->weight_ = val;
}

void Gene::innovation(unsigned val)
{
    impl_->innovation_ = val;
}

void Gene::in(unsigned val)
{
    impl_->in_ = val;
}

void Gene::out(unsigned val)
{
    impl_->out_ = val;
}

std::ostream& operator<<(std::ostream& stream, const Gene& g)
{
   stream << "[Is enabled: " << std::boolalpha << g.impl_->is_enabled_ << ", ";
   stream << "weight: "  << g.impl_->weight_ << ", ";
   stream << "innovation: "  << g.impl_->innovation_ << ", ";
   stream << "in: "  << g.impl_->in_ << ", ";
   stream << "out: "  << g.impl_->out_ << "]";
   stream << "\n";
   return stream;
}
