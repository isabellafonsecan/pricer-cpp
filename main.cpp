#include <iostream>
#include <cmath>
#include <random>

using namespace std;

class BlackScholesModel {
private:
    double _S0, _r, _sigma;

public:
    BlackScholesModel(double S0, double r, double sigma)
        : _S0(S0), _r(r), _sigma(sigma) {}

    double simulate_terminal_price(double T) const {
        static mt19937_64 gen(42);
        static normal_distribution<double> dist(0.0, 1.0);
        double Z = dist(gen);
        return _S0 * exp((_r - 0.5*_sigma*_sigma)*T + _sigma*sqrt(T)*Z);
    }
};

class Option {

private:
    double _K, _T;

public:
    Option(double K, double T) : _K(K), _T(T) {}
    virtual double payoff(double ST) const = 0;
};

class Call : public Option {
public:
    Call(double K_, double T_) : Option(K_, T_) {}
    double payoff(double ST) const override {
        return max(ST - K_, 0.0);
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
