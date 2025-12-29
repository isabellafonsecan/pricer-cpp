#pragma once
#include <iostream>
#include <cassert>
#include <cmath>

#include "pricer.hpp"
#include "../options/option.hpp"

// The class BlackScholesMerton implements the Black-Scholes-Merton pricing model
// for vanilla European options using closed-form formulas.
class BlackScholesMerton : public Pricer {
private:
    double _S0;    // Spot price
    double _K;     // Strike price
    double _r;     // Risk-free interest rate
    double _sigma; // Volatility

    // Cumulative distribution function of the standard normal distribution
    static double N(double x) {
        return 0.5 * std::erfc(-x * M_SQRT1_2);
    }

    // Probability density function of the standard normal distribution
    static double n(double x) {
        return std::exp(-0.5 * x * x) / std::sqrt(2.0 * M_PI);
    }

public:
    // Constructor
    BlackScholesMerton(double S0, double K, double r, double sigma)
        : _S0(S0), _K(K), _r(r), _sigma(sigma) {}

    // Price of a vanilla European option
    double price(const Option& option) const override {

        std::string type = option.type();
        assert(type == "VANEUCA" || type == "VANEUPUT");

        double T = option.getMaturity();
        double sqrtT = std::sqrt(T);

        double d1 = (std::log(_S0 / _K) + (_r + 0.5 * _sigma * _sigma) * T)
                    / (_sigma * sqrtT);
        double d2 = d1 - _sigma * sqrtT;

        if (type == "VANEUCA") {
            return _S0 * N(d1) - _K * std::exp(-_r * T) * N(d2);
        } else {
            return _K * std::exp(-_r * T) * N(-d2) - _S0 * N(-d1);
        }
    }

    // Delta of a vanilla European option
    double delta(const Option& option) const override {

        std::string type = option.type();
        assert(type == "VANEUCA" || type == "VANEUPUT");

        double T = option.getMaturity();
        double d1 = (std::log(_S0 / _K) + (_r + 0.5 * _sigma * _sigma) * T)
                    / (_sigma * std::sqrt(T));

        if (type == "VANEUCA") {
            return N(d1);
        } else {
            return N(d1) - 1.0;
        }
    }

    // Vega of a vanilla European option (per unit of volatility)
    double vega(const Option& option) const override {

        std::string type = option.type();
        assert(type == "VANEUCA" || type == "VANEUPUT");

        double T = option.getMaturity();
        double d1 = (std::log(_S0 / _K) + (_r + 0.5 * _sigma * _sigma) * T)
                    / (_sigma * std::sqrt(T));

        return _S0 * std::sqrt(T) * n(d1);
    }

    // Theta of a vanilla European option (per year)
    double theta(const Option& option) const override {

        std::string type = option.type();
        assert(type == "VANEUCA" || type == "VANEUPUT");

        double T = option.getMaturity();
        double sqrtT = std::sqrt(T);

        double d1 = (std::log(_S0 / _K) + (_r + 0.5 * _sigma * _sigma) * T)
                    / (_sigma * sqrtT);
        double d2 = d1 - _sigma * sqrtT;

        double firstTerm = -(_S0 * n(d1) * _sigma) / (2.0 * sqrtT);

        if (type == "VANEUCA") {
            return firstTerm - _r * _K * std::exp(-_r * T) * N(d2);
        } else {
            return firstTerm + _r * _K * std::exp(-_r * T) * N(-d2);
        }
    }

    // Destructor
    ~BlackScholesMerton() override {}
};
