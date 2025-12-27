#pragma once

#include "../stats/pathstats.hpp"

#include <iostream>

class Option {

protected:
    double _K;  // Strike price
    double _T;  // Maturity time
    bool isCall; // Type d'option Call ou Put true si call and false si put

public:
    Option(double K, double T, bool isCall) : _K(K), _T(T), isCall(isCall) {}

    virtual double payoff(const Stats& pathStats) const = 0;
    virtual std::string type() const = 0;
    virtual ~Option() {};

    double getStrike() const {
        return _K;
    }

    double getMaturity() const {
        return _T;
    }

    bool getIsCall() const {
        return isCall;
    }
};