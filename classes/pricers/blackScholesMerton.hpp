#pragma once
#include <iostream>
#include <cassert>

#include "pricer.hpp"
#include "../options/option.hpp"

class BlackScholesMerton : public Pricer {
    private:
        double _S0;    // Spot price
        double _K;     // Strike price
        double _r;     // Risk-free interest rate
        double _sigma; // Volatility

    public:
        BlackScholesMerton(double S0, double K, double r, double sigma) {
            _S0 = S0;
            _K = K;
            _r = r;
            _sigma = sigma;
        };

        double price(const Option& option) const override {

            std::string type = option.type();
            assert(type == "VANEUCA" || type == "VANEUPUT");

            double d1 = (log(_S0 / _K) + (_r + 0.5 * _sigma * _sigma) * option.maturity()) / (_sigma * sqrt(option.maturity()));
            double d2 = d1 - _sigma * sqrt(option.maturity());

            // Fonction de répartition cumulative de la loi normale
            auto N = [](double x) {
                return 0.5 * erfc(-x * M_SQRT1_2);
            };

            if (type == "VANEUCA") {
                return _S0 * N(d1) - _K * exp(-_r * option.maturity()) * N(d2);
            } else if (type == "VANEUPUT") {
                return _K * exp(-_r * option.maturity()) * N(-d2) - _S0 * N(-d1);
            }
        }

        double delta() const override;
};