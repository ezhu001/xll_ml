// xll_math.cpp - Math functions
#include "fms_math.h"
#include "xll24/include/xll.h"

#define CATEGORY L"MATH"

using namespace xll;
using namespace fms::math;

AddIn xai_erf_as(
	Function(XLL_DOUBLE, L"xll_erf_as", CATEGORY L".ERF")
	.Arguments({
		Arg(XLL_DOUBLE, L"x", L"is the value to compute the error function for."),
	})
	.Category(CATEGORY)
	.FunctionHelp(L"Return the error function of x using Abramowitz& Stegun approximation.")
);
double WINAPI xll_erf_as(double x)
{
#pragma XLLEXPORT
	return erf_as(x);
}