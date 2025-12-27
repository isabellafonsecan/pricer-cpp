// Test blackScholes.cpp
#include <iostream>
#include <memory>
#include "classes/pricers/blackScholesMerton.hpp"
#include "classes/options/vanillaEuropeanOption.hpp"
#include "classes/pricers/monteCarlo.hpp"
#include "classes/options/asian_option.hpp"
#include "classes/options/lookback_option.hpp"
#include "classes/options/cliquet_option.hpp"
#include "classes/options/option_a_barriere_desactivante.hpp"

int main() {
    // Paramètres du sous-jacent
    double S0 = 100.0;   // Prix initial
    double K = 100.0;    // Prix d'exercice
    double r = 0.05;     // Taux d'intérêt sans risque
    double sigma = 0.2;  // Volatilité
    double T = 1.0;      // Maturité en années

    // Nombre de simulations pour Monte Carlo
    int n_simulations = 100000;

    // Création des options
    VanillaEuropeanOption callOption(K, T, true);
    VanillaEuropeanOption putOption(K, T, false);
    AsianOption asianOption(K, T, true);
    LookbackOption lookbackOption(K, T, true);
    CliquetOption cliquetOption(K, T, true);
    barriereOption barrierOption(K, T, true, 120.0, barriereOption::UP_AND_OUT);

    // Création des pricers
    BlackScholesMerton bsmPricer(S0, K, r, sigma);
    MonteCarloPricer mcPricer(S0, r, sigma, n_simulations);

    // Calcul et affichage des prix
    std::cout << "Black-Scholes Call Price: " << bsmPricer.price(callOption) << std::endl;
    std::cout << "Black-Scholes Put Price: " << bsmPricer.price(putOption) << std::endl;
    std::cout << "Monte Carlo Asian Option Price: " << mcPricer.price(asianOption) << std::endl;
    std::cout << "Monte Carlo Lookback Option Price: " << mcPricer.price(lookbackOption) << std::endl;
    std::cout << "Monte Carlo Cliquet Option Price: " << mcPricer.price(cliquetOption) << std::endl;
    std::cout << "Monte Carlo Barrier Option Price: " << mcPricer.price(barrierOption) << std::endl;

    return 0;
}