#include <iostream>
#include <random>

#include "../options/option.hpp"
#include "../stats/pathstats.hpp"

// The class MonteCarloPricer implements a Monte Carlo pricing model for various types of options.
// It simulates multiple paths of the underlying asset price and calculates the average discounted payoff.
// We use polymorphism to handle different option types through the Option base class. 
//It allows us to use the payoff method defined in each derived option class.

class MonteCarloPricer : public Pricer {
private:
    double _S0;       // Spot price
    double _r;        // Risk-free interest rate
    double _sigma;    // Volatility
    int _nb_simulations; // Number of Monte Carlo simulations
    

public:
    // Constructor
    MonteCarloPricer(double s0, double rate, double vol, int nb_simulations) : _S0(s0), _r(rate), _sigma(vol), _nb_simulations(nb_simulations) {};
    
    // Method to calculate the price of the option
    double price(const Option& option) const override {

        // Determination of steps based on maturity, assuming 252 trading days per year
        int steps = static_cast<int>(std::round(option.getMaturity() * 252));

        // At least 1 step (at least one trading day)
        if (steps < 1) steps = 1;

        // Initialization of the random number generator
        // std::random_device rd; // Random seed
        std::mt19937 gen(42); // Fixed seed for reproducibility
        std::normal_distribution<double> dist(0.0, 1.0); // Standard normal distribution

        // Constant calculations to optimize the loop
        double dt = option.getMaturity() / static_cast<double>(steps);
        double drift = (_r - 0.5 * _sigma * _sigma) * dt;
        double vol_sqrt_dt = _sigma * std::sqrt(dt);
        
        // Variable to accumulate payoffs
        double sum_payoff = 0.0;

        // Monte Carlo simulations
        for (int i = 0; i < _nb_simulations; ++i) {
            // The initial price for this path is 
            double S = _S0;

            // We initialize path statistics that will be updated at each step
            Stats stats = Stats();

            // Simulation of the price path
            for (int j = 0; j < steps; ++j) {

                double z = dist(gen);

                // Évolution du prix (Mouvement Brownien Géométrique)
                S *= std::exp(drift + vol_sqrt_dt * z);
                stats.update(S);
            }
            
            // Calcul du payoff de l'option pour ce tour
            double payoff = option.payoff(stats);
            sum_payoff += payoff;

        } 

        // Return the discounted average payoff
        double discount_factor = std::exp(-_r * option.getMaturity());
        double average_payoff = sum_payoff / _nb_simulations;
        return average_payoff * discount_factor;
    }   
};