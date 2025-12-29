#pragma once
#include <iostream>
#include "option.hpp"
#include "../stats/pathstats.hpp"

// The class barriereOption represents a barrier option
// Its payoff depends on whether the underlying asset price hits a certain barrier level during the option's
class barriereOption : public Option {
public:
    // We define two kinds of barriers
    enum barriereType {
        UP_AND_OUT, //Upward barrier: the option is knocked out if the price reaches or exceeds the barrier
        DOWN_AND_OUT  // Downward barrier: the option is knocked out if the price falls to or below the barrier
    };

private:
    double _barrier; // Barrier level
    int _type; // Barrier type

public:
    // Constructor
    barriereOption(double K, double T, bool isCall, double barrier, int type): Option(K, T, isCall), _barrier(barrier), _type(type){}

    // Payoff function based on barrier condition and max/min price retrieved from path statistics
    double payoff(const Stats& pathStats) const override {

        // Distinguish between the two barrier types
        if (_type == UP_AND_OUT) {
            if (pathStats.max_S() >= _barrier) {
                return 0.0;
            }
            return std::max(pathStats.max_S() - _K, 0.0);
        } else {   
            if (pathStats.min_S() <= _barrier) {
                return 0.0;
            }         
            return std::max(_K - pathStats.min_S(), 0.0);
        }
    }

    // Return a copy of the option with a new maturity, it will be used for theta calculation in monte carlo pricer
    Option* cloneWithMaturity(double newMaturity) const override {
        return new barriereOption(_K, newMaturity, _isCall, _barrier, _type);
    }   

    // Type function
    std::string type() const override {
        return _isCall ? "EXBARICA" : "EXBARIPUT";
    }

    // Destructor
    ~barriereOption() override {}
};

