#pragma once
#include <iostream>

#include "option.hpp"

// The class AsianOption represents an Asian option
//Its payoff is based on the average price of the underlying asset over a certain period.
class AsianOption : public Option {
public:
    // Constructor
    AsianOption(double K, double T, bool call) : Option(K, T, call) {}

    // Payoff function based on average price that we retrieve from path statistics
    double payoff(const Stats& pathStats) const override {
        double average_S = pathStats.average_S();
        return _isCall ? std::max(average_S - _K, 0.0) : std::max(_K - average_S, 0.0);
    }

    // Return a copy of the option with a new maturity, it will be used for theta calculation in monte carlo pricer
    Option* cloneWithMaturity(double newMaturity) const override {
        return new AsianOption(_K, newMaturity, _isCall);
    }
    
    // Return the option type code (EXASICA for call, EXASIPUT for put)
    std::string type() const override {
        return _isCall ? "EXASICA" : "EXASIPUT";
    }

    // Destructor
    ~AsianOption() override {};
};

