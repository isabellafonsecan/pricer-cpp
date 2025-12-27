#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

#include "option.hpp"
#include "../stats/pathstats.hpp"

class LookbackOption : public Option {

public:
    LookbackOption(double K, double T, bool isCall) : Option(K, T, isCall) {}

    double payoff(const Stats& pathStats) const override {
        if (isCall) {
            return std::max(pathStats.ST() - pathStats.min_S(), 0.0);
        } else {
            return std::max(pathStats.max_S() - pathStats.ST(), 0.0);
        }
    }

    std::string type() const override {
        return isCall ? "EXLOOCA" : "EXLOOPUT";
    }

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
