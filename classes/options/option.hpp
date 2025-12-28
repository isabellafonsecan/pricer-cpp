#pragma once
#include "../stats/pathstats.hpp"

// The class Option is an abstract base class for different types of financial options both vanilla and exotic.
class Option {
protected:
    double _K;  // Strike price
    double _T;  // Maturity time
    bool _isCall; // Type d'option Call ou Put true si call and false si put

public:
    // Constructor
    Option(double K, double T, bool isCall) : _K(K), _T(T), _isCall(isCall) {}
    // Methods to be implemented by derived classes
    // Function to calculate the payoff given the path statistics, will be used in the Monte Carlo pricer
    virtual double payoff(const Stats& pathStats) const = 0;

    // Function to return the option type as a string, will be used in the black scholes pricer
    virtual std::string type() const = 0;

    // Destructor
    virtual ~Option() {};


    // Methods to access protected members
    double getStrike() const {
        return _K;
    }

    double getMaturity() const {
        return _T;
    }

    bool getIsCall() const {
        return _isCall;
    }
};