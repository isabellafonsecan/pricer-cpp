#include <iostream>
#include <cmath>
#include <random>

using namespace std;

class BlackScholesModel {
private:
    double S0, r, sigma;
    
public:
    BlackScholesModel(double S0_, double r_, double sigma_)
        : S0(S0_), r(r_), sigma(sigma_) {}

    double simulate_terminal_price(double T) const {
        static std::mt19937_64 gen(42);
        static std::normal_distribution<double> dist(0.0, 1.0);
        double Z = dist(gen);
        return S0 * exp((r - 0.5*sigma*sigma)*T + sigma*sqrt(T)*Z);
    }
};

class Option {
public:
    double K, T;
    Option(double K_, double T_) : K(K_), T(T_) {}
    virtual double payoff(double ST) const = 0;
};

class Call : public Option {
public:
    Call(double K_, double T_) : Option(K_, T_) {}
    double payoff(double ST) const override {
        return std::max(ST - K, 0.0);
    }
};

double price_monte_carlo(const Option& opt, const BlackScholesModel& model,
                         int N) {
    double sum = 0.0;
    for(int i = 0; i < N; i++){
        double ST = model.simulate_terminal_price(opt.T);
        sum += opt.payoff(ST);
    }
    return exp(-model.r * opt.T) * (sum / N);
}
