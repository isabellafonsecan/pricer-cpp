#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

class AsianOption {
private:
    double S0;       // Spot price
    double K;        // Strike prince
    double r;        // Taux d'intérêt free risk
    double sigma;    // Volatilité
    double T;        // Maturité en années
    int steps;       // Nombre de jours d'observation (caclulé à partir de la durée et du fait que 252 jours de bourse/an
    bool isCall;     // Type d'option (Call ou Put) : true si call , false si put

public:
    // Constructeur : steps est calculé automatiquement (T * 252 jours ouvrés)
    AsianOption(double s0, double k, double rate, double vol, double maturity, bool call)
        : S0(s0), K(k), r(rate), sigma(vol), T(maturity), isCall(call) {
        
        // Calcul des points d'observation
        this->steps = static_cast<int>(std::round(T * 252));
        
        // 1 etape mini ( au moins un jour de cotation)
        if (this->steps < 1) this->steps = 1;
    }

    // Calcul du prix par simulation de Monte Carlo
    double price(int n_simulations) {
        // Initialisation du générateur de nombres aléatoires 
        std::random_device rd; // Le vrai aléatoire
        std::mt19937 gen(rd()); // le "faux aleatoire"
        std::normal_distribution<double> dist(0.0, 1.0);

        // Pré-calcul des constantes pour optimiser la boucle 
        double dt = T / static_cast<double>(steps);
        double drift = (r - 0.5 * sigma * sigma) * dt;
        double vol_sqrt_dt = sigma * std::sqrt(dt);
        
        double sum_payoffs = 0.0;

        for (int i = 0; i < n_simulations; ++i) {
            double current_S = S0;
            double sum_S = 0.0;

            // Simulation de la trajectoire 
            for (int j = 0; j < steps; ++j) {
                double z = dist(gen);
                // Évolution du prix (Mouvement Brownien Géométrique)
                current_S *= std::exp(drift + vol_sqrt_dt * z);
                sum_S += current_S;
            }

            // Calcul de la moyenne arithmétique sur la période
            double average_S = sum_S / steps;
            
            // Calcul du gain (payoff) à l'échéance
            double payoff = isCall ? std::max(average_S - K, 0.0) : std::max(K - average_S, 0.0); // pay off nul si cours trop bas pour que ça soit interessant d'exercer l'action
            sum_payoffs += payoff;
        }

        // Moyenne des payoffs et actualisation à la valeur présente exp(-rT)
        return (sum_payoffs / n_simulations) * std::exp(-r * T);
    }

    // Nous dis le nombre de jours de cotation pris en compte pour le caclul
    int getSteps() const { return steps; }

    // Return the option type code (EXASICA for call, EXASIPUT for put)
    std::string type() const {
        return isCall ? "EXASICA" : "EXASIPUT";
    }
};

