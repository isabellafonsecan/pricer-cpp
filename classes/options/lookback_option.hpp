#pragma once
#include <iostream>

#include "option.hpp"
#include "../stats/pathstats.hpp"

// The class LookbackOption represents a floating strike lookback option
// Its payoff is based on the maximum or minimum price of the underlying asset over the option's life.
class LookbackOption : public Option {
public:
    // Constructor
    LookbackOption(double K, double T, bool isCall) : Option(K, T, isCall) {}

    // Payoff function based on max/min price retrieved from path statistics
    double payoff(const Stats& pathStats) const override {
        if (_isCall) {
            return std::max(pathStats.ST() - pathStats.min_S(), 0.0);
        } else {
            return std::max(pathStats.max_S() - pathStats.ST(), 0.0);
        }
    }

    // Return a copy of the option with a new maturity, it will be used for theta calculation in monte carlo pricer
    Option* cloneWithMaturity(double newMaturity) const override {
        return new LookbackOption(_K, newMaturity, _isCall);
    }

    // Return the option type code (EXLOOCA for call, EXLOOPUT for put)
    std::string type() const override {
        return _isCall ? "EXLOOCA" : "EXLOOPUT";
    }

    // Destructor
    ~LookbackOption() override {}
};
