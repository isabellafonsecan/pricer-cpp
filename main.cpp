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
    // double S0, K, r, sigma, T;
    // int n_simulations = 200000;

    // std::cout << "Entrez le prix initial (S0) : ";
    // std::cin >> S0;

    // std::cout << "Entrez le prix d'exercice (K) : ";
    // std::cin >> K;

    // std::cout << "Entrez le taux d'interet sans risque (r) : ";
    // std::cin >> r;

    // std::cout << "Entrez la volatilite (sigma) : ";
    // std::cin >> sigma;

    // std::cout << "Entrez la maturite en annees (T) : ";
    // std::cin >> T;

    // std::cout << "\nValeurs saisies :" << std::endl;
    // std::cout << "Prix initial (S0) : " << S0 << std::endl;
    // std::cout << "Prix d'exercice (K) : " << K << std::endl;
    // std::cout << "Taux d'interet sans risque (r) : " << r << std::endl;
    // std::cout << "Volatilite (sigma) : " << sigma << std::endl;
    // std::cout << "Maturite (T) : " << T << " ans" << std::endl;

    // Paramètres du sous-jacent
    double S0 = 100.0;   // Prix initial
    double K = 100.0;    // Prix d'exercice
    double r = 0.05;     // Taux d'intérêt sans risque
    double sigma = 0.2;  // Volatilité
    double T = 1;      // Maturité en années

    // Nombre de simulations pour Monte Carlo
    int n_simulations = 200000;

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
std::cout << "===== VANILLA OPTIONS =====" << std::endl;
// --- Call ---0
std::cout << "Vanilla Call (BSM)" << std::endl;
std::cout << "Price: " << bsmPricer.price(callOption) << std::endl;
std::cout << "Delta: " << bsmPricer.delta(callOption) << std::endl;
std::cout << "Vega : " << bsmPricer.vega(callOption) << std::endl;
std::cout << "Theta: " << bsmPricer.theta(callOption) << std::endl;
auto hedgeCall_BSM = bsmPricer.hedgingPortfolio(callOption);
std::cout << "Hedging Portfolio (units, cash): (" << hedgeCall_BSM.first << ", " << hedgeCall_BSM.second << ")" << std::endl;
std::cout << "--------------------------------" << std::endl;

std::cout << "Vanilla Call (Monte Carlo)" << std::endl;
std::cout << "Price: " << mcPricer.price(callOption) << std::endl;
std::cout << "Delta: " << mcPricer.delta(callOption) << std::endl;
std::cout << "Vega : " << mcPricer.vega(callOption) << std::endl;
std::cout << "Theta: " << mcPricer.theta(callOption) << std::endl;
auto hedgeCall_MC = mcPricer.hedgingPortfolio(callOption);
std::cout << "Hedging Portfolio (units, cash): (" << hedgeCall_MC.first << ", " << hedgeCall_MC.second << ")" << std::endl;
std::cout << "--------------------------------" << std::endl;

// --- Put ---
std::cout << "Vanilla Put (BSM)" << std::endl;
std::cout << "Price: " << bsmPricer.price(putOption) << std::endl;
std::cout << "Delta: " << bsmPricer.delta(putOption) << std::endl;
std::cout << "Vega : " << bsmPricer.vega(putOption) << std::endl;
std::cout << "Theta: " << bsmPricer.theta(putOption) << std::endl;
auto hedgePut_BSM = bsmPricer.hedgingPortfolio(putOption);
std::cout << "Hedging Portfolio (units, cash): (" << hedgePut_BSM.first << ", " << hedgePut_BSM.second << ")" << std::endl;
std::cout << "--------------------------------" << std::endl;

std::cout << "Vanilla Put (Monte Carlo)" << std::endl;
std::cout << "Price: " << mcPricer.price(putOption) << std::endl;
std::cout << "Delta: " << mcPricer.delta(putOption) << std::endl;
std::cout << "Vega : " << mcPricer.vega(putOption) << std::endl;
std::cout << "Theta: " << mcPricer.theta(putOption) << std::endl;
auto hedgePut_MC = mcPricer.hedgingPortfolio(putOption);
std::cout << "Hedging Portfolio (units, cash): (" << hedgePut_MC.first << ", " << hedgePut_MC.second << ")" << std::endl;
std::cout << "--------------------------------" << std::endl;

// std::cout << "===== ASIAN OPTIONS =====" << std::endl;
// // Call
// std::cout << "Asian Call (Monte Carlo)" << std::endl;
// std::cout << "Price: " << mcPricer.price(asianOptionCall) << std::endl;
// std::cout << "Delta: " << mcPricer.delta(asianOptionCall) << std::endl;
// std::cout << "Vega : " << mcPricer.vega(asianOptionCall) << std::endl;
// std::cout << "Theta: " << mcPricer.theta(asianOptionCall) << std::endl;
// std::cout << "--------------------------------" << std::endl;

// // Put
// std::cout << "Asian Put (Monte Carlo)" << std::endl;
// std::cout << "Price: " << mcPricer.price(asianOptionPut) << std::endl;
// std::cout << "Delta: " << mcPricer.delta(asianOptionPut) << std::endl;
// std::cout << "Vega : " << mcPricer.vega(asianOptionPut) << std::endl;
// std::cout << "Theta: " << mcPricer.theta(asianOptionPut) << std::endl;
// std::cout << "--------------------------------" << std::endl;

// std::cout << "===== LOOKBACK OPTIONS =====" << std::endl;
// // Call
// std::cout << "Lookback Call (Monte Carlo)" << std::endl;
// std::cout << "Price: " << mcPricer.price(lookbackOptionCall) << std::endl;
// std::cout << "Delta: " << mcPricer.delta(lookbackOptionCall) << std::endl;
// std::cout << "Vega : " << mcPricer.vega(lookbackOptionCall) << std::endl;
// std::cout << "Theta: " << mcPricer.theta(lookbackOptionCall) << std::endl;
// std::cout << "--------------------------------" << std::endl;

// // Put
// std::cout << "Lookback Put (Monte Carlo)" << std::endl;
// std::cout << "Price: " << mcPricer.price(lookbackOptionPut) << std::endl;
// std::cout << "Delta: " << mcPricer.delta(lookbackOptionPut) << std::endl;
// std::cout << "Vega : " << mcPricer.vega(lookbackOptionPut) << std::endl;
// std::cout << "Theta: " << mcPricer.theta(lookbackOptionPut) << std::endl;
// std::cout << "--------------------------------" << std::endl;

// std::cout << "===== CLIQUET OPTIONS =====" << std::endl;
// // Call
// std::cout << "Cliquet Call (Monte Carlo)" << std::endl;
// std::cout << "Price: " << mcPricer.price(cliquetOptionCall) << std::endl;
// std::cout << "Delta: " << mcPricer.delta(cliquetOptionCall) << std::endl;
// std::cout << "Vega : " << mcPricer.vega(cliquetOptionCall) << std::endl;
// std::cout << "Theta: " << mcPricer.theta(cliquetOptionCall) << std::endl;
// std::cout << "--------------------------------" << std::endl;

// // Put
// std::cout << "Cliquet Put (Monte Carlo)" << std::endl;
// std::cout << "Price: " << mcPricer.price(cliquetOptionPut) << std::endl;
// std::cout << "Delta: " << mcPricer.delta(cliquetOptionPut) << std::endl;
// std::cout << "Vega : " << mcPricer.vega(cliquetOptionPut) << std::endl;
// std::cout << "Theta: " << mcPricer.theta(cliquetOptionPut) << std::endl;
// std::cout << "--------------------------------" << std::endl;

// std::cout << "===== BARRIER OPTIONS =====" << std::endl;
// // Call
// std::cout << "Barrier Call (Monte Carlo)" << std::endl;
// std::cout << "Price: " << mcPricer.price(barrierOptionCall) << std::endl;
// std::cout << "Delta: " << mcPricer.delta(barrierOptionCall) << std::endl;
// std::cout << "Vega : " << mcPricer.vega(barrierOptionCall) << std::endl;
// std::cout << "Theta: " << mcPricer.theta(barrierOptionCall) << std::endl;
// std::cout << "--------------------------------" << std::endl;

// // Put
// std::cout << "Barrier Put (Monte Carlo)" << std::endl;
// std::cout << "Price: " << mcPricer.price(barrierOptionPut) << std::endl;
// std::cout << "Delta: " << mcPricer.delta(barrierOptionPut) << std::endl;
// std::cout << "Vega : " << mcPricer.vega(barrierOptionPut) << std::endl;
// std::cout << "Theta: " << mcPricer.theta(barrierOptionPut) << std::endl;
// std::cout << "--------------------------------" << std::endl;


//     return 0;
// }