#pragma once
#include <iostream>

#include "../options/option.hpp"

class Pricer {
public:
    virtual double price(const Option& option) const = 0;
    virtual double delta() const = 0;
    virtual ~Pricer() {} 
};