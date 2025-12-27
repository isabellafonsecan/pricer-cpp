#pragma once

#include <iostream>

#include "option.h"

class PutEuropean : public Option {
public:
    PutEuropean(double _K, double _T) : Option(_K, _T) {}

    double payoff(double ST) const override {
        return std::max(_K - ST, 0.0);
    }

    double price_bsm(double S0, double r, double sigma) const {
        double d1 = (log(S0 / _K) + (r + 0.5 * sigma * sigma) * _T) / (sigma * sqrt(_T));
        double d2 = d1 - sigma * sqrt(_T);

        // Fonction de répartition cumulative de la loi normale
        auto N = [](double x) {
            return 0.5 * erfc(-x * M_SQRT1_2);
        };

        return _K * exp(-r * _T) * N(-d2) - S0 * N(-d1);
    }

    ~PutEuropean() override {}
};