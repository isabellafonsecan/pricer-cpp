#pragma once
#include <iostream>
#include <vector>

#include "option.hpp"
#include "../stats/pathstats.hpp"

// The class CliquetOption represents a cliquet option
// Its payoff is based on the sum of positive returns over the option's life, minus a strike price.
// In this case the gains are checked everyday, however in real life it is often monthly or quarterly.
class CliquetOption : public Option {
public:
    // Constructor
    CliquetOption(double K, double T, bool call) : Option(K, T, call) {}

    // Payoff function based on sum of positive gains retrieved from path statistics
    double payoff(const Stats& stats) const override {
        double payoff = 0.0;

        // We only sum positive returns
        for (double r : stats.gains()) payoff += std::max(r, 0.0);
        return std::max(payoff - _K, 0.0);
    }

    // Return a copy of the option with a new maturity, it will be used for theta calculation in monte carlo pricer
    Option* cloneWithMaturity(double newMaturity) const override {
        return new CliquetOption(_K, newMaturity, _isCall);
    }

    // Return the option type code (EXCLICA for call, EXCLIPUT for put)
    std::string type() const override {
        return _isCall ? "EXCLICA" : "EXCLIPUT";
    }

    // Destructor
    ~CliquetOption() override {}
};