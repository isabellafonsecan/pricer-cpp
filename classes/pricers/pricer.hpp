#pragma once
#include "../options/option.hpp"

// The class Pricer is an abstract base class for different pricing models.
class Pricer {
public:
    // methods to be implemented by derived classes
    // function to calculate the price of the option
    virtual double price(const Option& option) const = 0;

    //destructor
    virtual ~Pricer() {} 
};