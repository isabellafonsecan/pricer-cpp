#include <iostream>

#include "option.hpp"
#include "../stats/pathstats.hpp"

// The class VanillaEuropeanOption represents a vanilla European option
// Its payoff is based on the underlying asset price at maturity compared to the strike price.
class VanillaEuropeanOption : public Option {
public:
    // Constructor
    VanillaEuropeanOption(double K, double T, bool isCall) : Option(K, T, isCall) {};
    
    // Payoff function based on the underlying asset price at maturity retrieved from path statistics
    double payoff(const Stats& pathStats) const override {
        double ST = pathStats.ST();
        return _isCall ? std::max(ST - _K, 0.0) : std::max(_K - ST, 0.0);
    }

    // Return a copy of the option with a new maturity, it will be used for theta calculation in monte carlo pricer
    Option* cloneWithMaturity(double newMaturity) const override {
        return new VanillaEuropeanOption(_K, newMaturity, _isCall);
    }

    // Return the option type code (VANEUCA for call, VANEUPUT for put)
    std::string type() const override {
        return _isCall ? "VANEUCA" : "VANEUPUT";
    }

    // Destructor
    ~VanillaEuropeanOption() override {}
};