class Option {

protected:
    double _K;  // Strike price
    double _T;  // Maturity time

public:
    Option(double K, double T) : _K(K), _T(T) {}
    virtual double payoff(double ST) const = 0;
    virtual ~Option() {}
};