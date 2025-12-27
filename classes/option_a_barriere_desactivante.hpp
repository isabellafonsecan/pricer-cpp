#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

class barriereOption {
public:
    enum barriereType {
        UP_AND_OUT,   // Barrière supérieure : l'option s'annule si le prix atteint ou dépasse la barrière
        DOWN_AND_OUT  // Barrière inférieure : l'option s'annule si le prix atteint ou descend en dessous de la barrière
    };

private:
    double S0;       // Spot price
    double K;        // Strike price
    double r;        // tx free risk
    double sigma;    // Volatilité 
    double T;        // Maturité en années
    double barriere;  // Niveau de la barrière
    bool isCall;     // Type d'option Call ou Put true si call and false si put
    int barriereType; // Type de la barrière Up and Out : 0  ou Down and Out : 1
    int steps;       // Nombre de pas de temps : nbr de jours d'observation

public:
    // Constructeur
    barriereOption(double s0, double k, double rate, double vol, double maturity, double b, bool call, int type)
        : S0(s0), K(k), r(rate), sigma(vol), T(maturity), barriere(b), isCall(call), barriereType(type) {
        
        this->steps = static_cast<int>(std::round(T * 252));
        if (this->steps < 1) this->steps = 1;
    }

    // Calcul du prix par simulation de Monte Carlo
    double price(int n_simulations) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);

        double dt = T / static_cast<double>(steps);
        double drift = (r - 0.5 * sigma * sigma) * dt; // tendnace du marche
        double vol_sqrt_dt = sigma * std::sqrt(dt); 
        
        double sum_payoffs = 0.0;

        for (int i = 0; i < n_simulations; ++i) {
            double current_S = S0;
            bool knocked_out = false; // Flag pour vérifier si la barrière a été touchée

            for (int j = 0; j < steps; ++j) {
                // Si l'option est déjà annulée, on arrête tout car l'option ne vaut plus rien
                if (knocked_out) {
                    break; 
                }

                double z = dist(gen);
                current_S *= std::exp(drift + vol_sqrt_dt * z);

                // Vérification de la barrière à chaque fois ( met le flag en true si jamais l'option est annulee)
                if (barriereType == UP_AND_OUT && current_S >= barriere) {
                    knocked_out = true;
                } else if (barriereType == DOWN_AND_OUT && current_S <= barriere) {
                    knocked_out = true;
                }
            }

            // Calcul du payoff (si et seulement si l'option n'a pas été annulée)
            if (!knocked_out) {
                double payoff = isCall ? std::max(current_S - K, 0.0) : std::max(K - current_S, 0.0);
                sum_payoffs += payoff;
            }
        }

        return (sum_payoffs / n_simulations) * std::exp(-r * T);
    }
};

