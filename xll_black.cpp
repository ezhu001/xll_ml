// xll_black.cpp - Generalized Black model
#include "fms_option_black.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"BLACK"

using namespace xll;
using namespace fms::option::black;

AddIn xai_option_model_black_normal(
		Function(XLL_DOUBLE, L"xll_black_normal", L"BLACK.NORMAL")
	.Arguments({
		Arg(XLL_DOUBLE, L"f", L"is the forward price."),
		Arg(XLL_DOUBLE, L"s", L"is the volatility."),
		Arg(XLL_DOUBLE, L"k", L"is the strike price."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return price of a European put option under the Black model with normal distribution.")
);

AddIn xai_black_moneyness(
	Function(XLL_DOUBLE, L"xll_black_moneyness", L"BLACK.MONEYNESS")
	.Arguments({
		Arg(XLL_DOUBLE, L"f", L"is the forward price."),
		Arg(XLL_DOUBLE, L"s", L"is the volatility."),
		Arg(XLL_DOUBLE, L"k", L"is the strike price."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return moneyness of an option.")
);
double WINAPI xll_black_moneyness(double f, double s, double k)
{	
#pragma XLLEXPORT
	normal m;

	return moneyness(f, s, k, m);
}