#include <iostream>

#include "pricer.hpp"
#include "../options/asian_option.hpp"
#include "../options/lookback_option.hpp"
#include "../options/option_a_barriere_desactivante.hpp"
#include "../options/cliquet_option.hpp"

#include "../stats/pathstats.hpp"

class MonteCarloPricer : public Pricer {
private:
    double _S0;       // Spot price
    double _r;        // Risk-free interest rate
    double _sigma;    // Volatility
    int _nb_simulations; // Number of Monte Carlo simulations
    

public:
    MonteCarloPricer(double s0, double rate, double vol, int nb_simulations) : _S0(s0), _r(rate), _sigma(vol), _nb_simulations(nb_simulations) {};
    
    double price(const Option& option) const override {

        // Determination of steps based on maturity
        int steps = static_cast<int>(std::round(option.getMaturity() * 252));
        if (steps < 1) steps = 1;

        // Calculation of the average price
        // Initialize the random number generator
        std::random_device rd; // Le vrai aléatoire
        std::mt19937 gen(rd()); // le "faux aleatoire"
        std::normal_distribution<double> dist(0.0, 1.0);

        // Constant calculations to optimize the loop
        double dt = option.getMaturity() / static_cast<double>(steps);
        double drift = (_r - 0.5 * _sigma * _sigma) * dt;
        double vol_sqrt_dt = _sigma * std::sqrt(dt);
        
        // Variable to accumulate payoffs
        double sum_payoff = 0.0;

        // Monte Carlo simulations
        for (int i = 0; i < _nb_simulations; ++i) {
            double S = _S0;
            Stats stats = Stats();

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

        return sum_payoff / _nb_simulations * std::exp(-_r * option.getMaturity());
    }   
};