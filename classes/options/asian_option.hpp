#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

#include "option.hpp"

class AsianOption : public Option {
public:
    AsianOption(double K, double T, bool call) : Option(K, T, call) {}

    // Return the option type code (EXASICA for call, EXASIPUT for put)
    std::string type() const override {
        return isCall ? "EXASICA" : "EXASIPUT";
    }

    double payoff(const Stats& pathStats) const override {
        double average_S = pathStats.average_S();
        return isCall ? std::max(average_S - _K, 0.0) : std::max(_K - average_S, 0.0);
    }
    
    ~AsianOption() override {};
};

