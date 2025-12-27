#pragma once

#include <iostream>
#include <vector>

class Stats {
private:
    double _before_ST;  // Before last value
    double _ST;           // Last value
    double _sum_S;       // Sum of values
    double _min_S;       // Minimum value
    double _max_S;       // Maximum value
    int _n_steps;      // Number of steps
    std::vector<double> _returns; // Vector of returns

public:
    Stats() : _before_ST(0.0), _ST(0.0), _sum_S(0.0), _min_S(INFINITY), _max_S(-INFINITY), _n_steps(0) {}

    // Function to update statistics with a new price S
    void update(double S) {
        if (_n_steps > 1) {
            double ret = S - _before_ST;
            _returns.push_back(ret);
        }

        _before_ST = _ST;
        _ST = S;
        _sum_S += S;
        _min_S = std::min(_min_S, S);
        _max_S = std::max(_max_S, S);
        _n_steps++;
    }

    // Getters for statistics
    double ST() const { return _ST; }
    double average_S() const { return _n_steps > 0 ? _sum_S / _n_steps : 0.0; }
    double min_S() const { return _min_S; }
    double max_S() const { return _max_S; }
    const std::vector<double>& returns() const { return _returns; } 
};