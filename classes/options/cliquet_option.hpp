#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

#include "option.hpp"
#include "../stats/pathstats.hpp"

class CliquetOption : public Option {

// private:
//     double checkPoint; // intervalle de temps entre deux checks du cliquet

public:
    CliquetOption(double K, double T, bool call) : Option(K, T, call) {}

    double payoff(const Stats& stats) const override {
        double payoff = 0.0;
        for (double r : stats.returns()) payoff += std::max(r, 0.0);
        return payoff - _K;
    }

    std::string type() const override {
        return isCall ? "EXCLICA" : "EXCLIPUT";
    }

    ~CliquetOption() override {}

    // // ===== Main GREEKS (delta, vega, theta) =====

    // double delta(int n_simulations, double bump = 0.01) const {
    //     CliquetOption up(S0 * (1 + bump), r, sigma, T);
    //     CliquetOption down(S0 * (1 - bump), r, sigma, T);

    //     return (up.price(n_simulations) - down.price(n_simulations))
    //            / (2 * S0 * bump);
    // }

    // double vega(int n_simulations, double bump = 0.01) const {
    //     CliquetOption up(S0, r, sigma + bump, T);
    //     CliquetOption down(S0, r, sigma - bump, T);

    //     return (up.price(n_simulations) - down.price(n_simulations))
    //            / (2 * bump);
    // }

    // double theta(int n_simulations, double bump = 1.0 / 365.0) const {
    //     CliquetOption shorter(S0, r, sigma, T - bump, isCall);

    //     return (shorter.price(n_simulations) - price(n_simulations))
    //            / bump;
    // }
};