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

    // Return the option type code (EXCLICA for call, EXCLIPUT for put)
    std::string type() const override {
        return _isCall ? "EXCLICA" : "EXCLIPUT";
    }

    // Destructor
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