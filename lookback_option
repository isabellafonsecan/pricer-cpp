#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

class LookbackOption {
private:
    double S0;
    double r;
    double sigma;
    double T;
    int steps;
    bool isCall;

public:
    LookbackOption(double s0, double rate, double vol, double maturity, bool call)
        : S0(s0), r(rate), sigma(vol), T(maturity), isCall(call) {

        steps = static_cast<int>(std::round(T * 252));
        if (steps < 1) steps = 1;
    }

    // Monte-Carlo simulation
    double price(int n_simulations) const {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);

        double dt = T / steps;
        double drift = (r - 0.5 * sigma * sigma) * dt;
        double vol_sqrt_dt = sigma * std::sqrt(dt);

        double sum_payoffs = 0.0;

        for (int i = 0; i < n_simulations; ++i) {
            double S = S0;
            double minS = S0;
            double maxS = S0;

            for (int j = 0; j < steps; ++j) {
                double z = dist(gen);
                S *= std::exp(drift + vol_sqrt_dt * z);

                minS = std::min(minS, S);
                maxS = std::max(maxS, S);
            }

            double payoff;
            if (isCall)
                payoff = std::max(S - minS, 0.0);
            else
                payoff = std::max(maxS - S, 0.0);

            sum_payoffs += payoff;
        }

        return (sum_payoffs / n_simulations) * std::exp(-r * T);
    }

    // ===== Main GREEKS (delta, vega, theta) =====

    double delta(int n_simulations, double bump = 0.01) const {
        LookbackOption up(S0 * (1 + bump), r, sigma, T, isCall);
        LookbackOption down(S0 * (1 - bump), r, sigma, T, isCall);

        return (up.price(n_simulations) - down.price(n_simulations))
               / (2 * S0 * bump);
    }

    double vega(int n_simulations, double bump = 0.01) const {
        LookbackOption up(S0, r, sigma + bump, T, isCall);
        LookbackOption down(S0, r, sigma - bump, T, isCall);

        return (up.price(n_simulations) - down.price(n_simulations))
               / (2 * bump);
    }

    double theta(int n_simulations, double bump = 1.0 / 365.0) const {
        LookbackOption shorter(S0, r, sigma, T - bump, isCall);

        return (shorter.price(n_simulations) - price(n_simulations))
               / bump;
    }
};
