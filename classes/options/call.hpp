#pragma once

#include <iostream>
#include "option.hpp"

class CallEuropean : public Option {

public:
    CallEuropean(double _K, double _T) : Option(_K, _T) {}

    double payoff(double ST) const override {
        return std::max(ST - _K, 0.0);
    }

    std::string type() const override {
        return "VANEUCA";
    }

    ~CallEuropean() override {}
};

