// xll_option_discrete.cpp
#include "fms_option_discrete.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"OPTION"

using namespace xll;
using namespace fms::option;

AddIn xai_option_discrete(
	Function(XLL_HANDLEX, L"xll_option_discrete", L"\\" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_FP, L"x", L"is the vector of x."),
		Arg(XLL_FP, L"p", L"is the vector of p."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to discrete option pricing model.")
);
HANDLEX WINAPI xll_option_discrete(_FP12* x, _FP12* p)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {

		size_t n = size(*x);

		discrete::model mod = discrete::model(n, (double*)x, (double*)p);

		//Before that I could do this by just inputting the FP12 but now it seems like not working
		//discrete::model mod = discrete::model(n, x, p);

		handle<discrete::model<>> m_(new discrete::model(std::move(mod)));
		ensure(m_);
		result = m_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return result;
}

AddIn xai_option_dicrete_getter(
	Function(XLL_FP, L"xll_option_discrete_getter", L"" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_HANDLEX, L"x", L"is a discrete model handle.")
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return information in the normalized xi values.")
);
_FP12* WINAPI xll_option_discrete_getter(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		/*
		handle<discrete::model<>> m(h);

		size_t n = m->xi.size();

		ensure(m->xi.size() == m->pi.size());
		
		// Well I see that x is normalized as soon as the class discrete::model is created so I guess don't have to do it now.

		result.resize(static_cast<int>(n), 1);

		for (size_t i = 0; i < n; ++i) {
			result(static_cast<int>(i), 0) = m->xi[i];
		}

		return result.get();
		*/

		result.resize(0, 0);
		handle<discrete::model<>> m(h);
		ensure(m);
		discrete::model<>* ptf = m.as<discrete::model<>>();
		size_t n = ptf->xi.size();
		
		result.resize(static_cast<int>(n), 2);

		for (size_t i = 0; i < n; ++i) {
			result(static_cast<int>(i), 0) = ptf->xi[i];
			result(static_cast<int>(i), 1) = ptf->pi[i];
		}

		return result.get();

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return result.get();
}
