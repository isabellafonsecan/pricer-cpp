#pragma once
#include <iostream>

class Option {

protected:
    double _K;  // Strike price
    double _T;  // Maturity time

public:
    Option(double K, double T) : _K(K), _T(T) {}
    virtual double payoff(double ST) const = 0;
    virtual std::string type() const = 0;
    virtual ~Option() {};

    double strike() const {
        return _K;
    }

    double maturity() const {
        return _T;
    }
};