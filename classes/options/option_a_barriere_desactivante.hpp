#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

#include "option.hpp"
#include "../stats/pathstats.hpp"

class barriereOption : public Option {
public:
    enum barriereType {
        UP_AND_OUT,   // Barrière supérieure : l'option s'annule si le prix atteint ou dépasse la barrière
        DOWN_AND_OUT  // Barrière inférieure : l'option s'annule si le prix atteint ou descend en dessous de la barrière
    };

private:
    double _barriere; // Niveau de la barrière
    int _type; // Type de la barrière Up and Out : 0  ou Down and Out : 1

public:
    // Constructeur
    barriereOption(double K, double T, bool isCall, double barriere, int type): Option(K, T, isCall), _barriere(barriere), _type(type){}

    // Payoff function
    double payoff(const Stats& pathStats) const override {
        if (_type == UP_AND_OUT) {
            if (pathStats.max_S() >= _barriere) {
                return 0.0;
            }
            return std::max(pathStats.max_S() - _K, 0.0);
        } else {   
            if (pathStats.min_S() <= _barriere) {
                return 0.0;
            }         
            return std::max(_K - pathStats.min_S(), 0.0);
        }
    }
    // Type function
    std::string type() const override {
        return isCall ? "EXBARICA" : "EXBARIPUT";
    }

    ~barriereOption() override {}
};

