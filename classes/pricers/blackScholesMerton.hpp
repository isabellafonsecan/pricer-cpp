#pragma once
#include <iostream>
#include <cassert>

#include "pricer.hpp"
#include "../options/option.hpp"

// The class BlackScholesMerton implements the Black-Scholes-Merton pricing model for vanilla European options.
// We use the closed-form solution to calculate the option price.
class BlackScholesMerton : public Pricer {
    private:
        double _S0;    // Spot price
        double _K;     // Strike price
        double _r;     // Risk-free interest rate
        double _sigma; // Volatility

    public:
        // Constructor
        BlackScholesMerton(double S0, double K, double r, double sigma) : _S0(S0), _K(K), _r(r), _sigma(sigma) {};

        // Method to calculate the price of the option
        double price(const Option& option) const override {

            // Ensure that the option is a vanilla European option, we'll use monte carlo for exotics
            std::string type = option.type();
            assert(type == "VANEUCA" || type == "VANEUPUT");

            // Black-Scholes-Merton formula
            double d1 = (log(_S0 / _K) + (_r + 0.5 * _sigma * _sigma) * option.getMaturity()) / (_sigma * sqrt(option.getMaturity()));
            double d2 = d1 - _sigma * sqrt(option.getMaturity());

            // Cumulative normal distribution function
            auto N = [](double x) {
                return 0.5 * erfc(-x * M_SQRT1_2);
            };

            // Calculate the option price based on its type using the Black-Scholes-Merton formula
            if (type == "VANEUCA") {
                return _S0 * N(d1) - _K * exp(-_r * option.getMaturity()) * N(d2);
            } else if (type == "VANEUPUT") {
                return _K * exp(-_r * option.getMaturity()) * N(-d2) - _S0 * N(-d1);
            }
            return 0.0; // Should never reach here
        }

        // Destructor
        ~BlackScholesMerton() override {};
};