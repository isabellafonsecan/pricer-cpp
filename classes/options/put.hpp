#pragma once

#include <iostream>
#include "option.hpp"

class PutEuropean : public Option {
public:
    PutEuropean(double _K, double _T) : Option(_K, _T) {}

    double payoff(double ST) const override {
        return std::max(_K - ST, 0.0);
    }
    
    std::string type() const override {
        return "VANEUPUT";
    }

    ~PutEuropean() override {}
};