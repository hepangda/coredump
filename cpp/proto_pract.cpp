#include<iostream>
#include<vector>
#include<boost/proto/proto.hpp>
using namespace boost;

template<int I>
struct placeholder {};

struct calculator_context
    : proto::callable_context<calculator_context const>
{
    std::vector<double> args;

    typedef double result_type;

    template<int I>
    double operator()(proto::tag::terminal, placeholder<I>) const
    {
        return this->args[I];
    }
};

template<typename Expr> struct calculator;

struct calculator_domain
    : proto::domain<proto::generator<calculator> > {};

template<typename Expr>
struct calculator
    : proto::extends<Expr, calculator<Expr>, calculator_domain>
{
    typedef proto::extends<Expr, calculator<Expr>, calculator_domain> base_type;

    calculator(Expr const &expr = Expr()): base_type(expr) {}

    typedef double result_type;

    double operator()(double a1 = 0, double a2 = 0) const
    {
        calculator_context ctx;
        ctx.args.push_back(a1);
        ctx.args.push_back(a2);

        return proto::eval(*this, ctx);
    }
};

calculator<proto::terminal<placeholder<0> >::type> const _1;
calculator<proto::terminal<placeholder<1> >::type> const _2;

int main()
{
    double result = ((_2 - _1) / _2 * 100)(1.0, 6.0);
    std::cout << result << std::endl;
    return 0;
}