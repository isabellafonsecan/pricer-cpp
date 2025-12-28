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

    // Return the option type code (EXLOOCA for call, EXLOOPUT for put)
    std::string type() const override {
        return _isCall ? "EXLOOCA" : "EXLOOPUT";
    }

    // Destructor
    ~LookbackOption() override {}

    // // ===== Main GREEKS (delta, vega, theta) =====

    // double delta(int n_simulations, double bump = 0.01) const {
    //     LookbackOption up(S0 * (1 + bump), r, sigma, T, isCall);
    //     LookbackOption down(S0 * (1 - bump), r, sigma, T, isCall);

    //     return (up.price(n_simulations) - down.price(n_simulations))
    //            / (2 * S0 * bump);
    // }

    // double vega(int n_simulations, double bump = 0.01) const {
    //     LookbackOption up(S0, r, sigma + bump, T, isCall);
    //     LookbackOption down(S0, r, sigma - bump, T, isCall);

    //     return (up.price(n_simulations) - down.price(n_simulations))
    //            / (2 * bump);
    // }

    // double theta(int n_simulations, double bump = 1.0 / 365.0) const {
    //     LookbackOption shorter(S0, r, sigma, T - bump, isCall);

    //     return (shorter.price(n_simulations) - price(n_simulations))
    //            / bump;
    // }
};
