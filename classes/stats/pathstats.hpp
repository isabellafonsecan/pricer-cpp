#pragma once
#include <iostream>
#include <vector>

// The class Stats is used to collect statistics about the path of the underlying asset price in Monte Carlo simulations.
class Stats {
private:
    double _before_ST;  // Before last value, used to calculate returns
    double _ST;           // Last value
    double _sum_S;       // Sum of values
    double _min_S;       // Minimum value
    double _max_S;       // Maximum value
    int _n_steps;      // Number of steps
    std::vector<double> _gains; // Vector of returns

public:
    Stats() : _before_ST(0.0), _ST(0.0), _sum_S(0.0), _min_S(INFINITY), _max_S(-INFINITY), _n_steps(0) {}

    // Function to update statistics with a new price S
    void update(double S) {

        // Update gains vector
        if (_n_steps > 1) {
            double ret = S - _before_ST;
            _gains.push_back(ret);
        }

        // Update other statistics
        _before_ST = _ST;
        _ST = S;       
        _sum_S += S;
        _min_S = std::min(_min_S, S);
        _max_S = std::max(_max_S, S);
        _n_steps++;
    }

    // Getters for statistics, will be used in option payoff calculations
    double ST() const { return _ST; }
    double average_S() const { return _n_steps > 0 ? _sum_S / _n_steps : 0.0; }
    double min_S() const { return _min_S; }
    double max_S() const { return _max_S; }
    const std::vector<double>& gains() const { return _gains; } 
};