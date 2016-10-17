#include "Gene.hpp"
#include <iostream>

struct Gene::Impl
{
    bool is_enabled_ = true;
    float weight_ = 0.0f;
    unsigned innovation_ = 0;
    unsigned in_ = 0;
    unsigned out_ = 0;

};



Gene::Gene(const RandomGenerator&)
:impl_(new Impl)
{
    // TODO: pass generator to impl
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
   stream << "[Is enabled: "  << g.impl_->is_enabled_ << ", ";
   stream << "weight: "  << g.impl_->weight_ << ", ";
   stream << "innovation: "  << g.impl_->innovation_ << ", ";
   stream << "in: "  << g.impl_->in_ << ", ";
   stream << "out: "  << g.impl_->out_ << "]";
   stream << "\n";
   return stream;
}
