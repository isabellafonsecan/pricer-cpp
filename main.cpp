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

    AsianOption asianOptionCall(K, T, true);
    AsianOption asianOptionPut(K, T, false);

    LookbackOption lookbackOptionCall(K, T, true);
    LookbackOption lookbackOptionPut(K, T, false);

    CliquetOption cliquetOptionCall(K, T, true);
    CliquetOption cliquetOptionPut(K, T, false);

    barriereOption barrierOptionPut(K, T, false, 120.0, barriereOption::UP_AND_OUT);
    barriereOption barrierOptionCall(K, T, true, 80.0, barriereOption::DOWN_AND_OUT);

    // Création des pricers
    BlackScholesMerton bsmPricer(S0, K, r, sigma);
    MonteCarloPricer mcPricer(S0, r, sigma, n_simulations);

    // Calcul et affichage des prix
    // std::cout << "Black-Scholes Call Price: " << bsmPricer.price(callOption) << std::endl;
    // std::cout << "Black-Scholes Put Price: " << bsmPricer.price(putOption) << std::endl;
    // std::cout << "Monte Carlo Asian Option Call Price: " << mcPricer.price(asianOptionCall) << std::endl;
    // std::cout << "Monte Carlo Asian Option Put Price: " << mcPricer.price(asianOptionPut) << std::endl;
    // std::cout << "Monte Carlo Lookback Option Call Price: " << mcPricer.price(lookbackOptionCall) << std::endl;
    // std::cout << "Monte Carlo Lookback Option Put Price: " << mcPricer.price(lookbackOptionPut) << std::endl;
    std::cout << "Monte Carlo Cliquet Option Call Price: " << mcPricer.price(cliquetOptionCall) << std::endl;
    std::cout << "Monte Carlo Cliquet Option Put Price: " << mcPricer.price(cliquetOptionPut) << std::endl;
    // std::cout << "Monte Carlo Barrier Option Put Price: " << mcPricer.price(barrierOptionPut) << std::endl;
    // std::cout << "Monte Carlo Barrier Option Call Price: " << mcPricer.price(barrierOptionCall) << std::endl;

    return 0;
}