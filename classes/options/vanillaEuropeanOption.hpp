#include <iostream>

#include "option.hpp"
#include "../stats/pathstats.hpp"

class VanillaEuropeanOption : public Option {
public:
    VanillaEuropeanOption(double K, double T, bool isCall) : Option(K, T, isCall) {};
    double payoff(const Stats& pathStats) const override {

        double ST = pathStats.ST();

        return isCall ? std::max(ST - _K, 0.0) : std::max(_K - ST, 0.0);
    }
    std::string type() const override {
        return isCall ? "VANEUCA" : "VANEUPUT";
    }
    ~VanillaEuropeanOption() override {}
};