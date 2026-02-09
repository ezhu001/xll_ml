// xll_ml.cpp
#include "fms_perceptron.h"
#include "xll_ml.h"

using namespace xll;
using namespace fms::perceptron;

AddIn xai_perceptron_update(
	Function(XLL_FP, L"xll_perceptron_update", L"PERCEPTRON.UPDATE")
	.Arguments({
		Arg(XLL_FP, L"w", L"is an array of weights."),
		Arg(XLL_FP, L"x", L"is an array representing the input vector."),
		Arg(XLL_BOOL, L"y", L"is the label"),
		Arg(XLL_DOUBLE, L"alpha", L"is the learning rate. (default=1.0)", 1.0)
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Update perceptron weights input vector and label.")
);
_FP12* WINAPI xll_perceptron_update(_FP12* pw, _FP12* px, BOOL y, double alpha)
{
#pragma XLLEXPORT
	try {
		ensure(size(*pw) == size(*px) || !"weight and input vector size mismatch");
		
		alpha = alpha ? alpha : 1;

		update(size(*pw), pw->array, px->array, y, alpha);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCDNAME__ ": unknown exception");
	}
	return pw;
}

AddIn xai_perceptron_train(
	Function(XLL_FP, L"xll_perceptron_train", L"PERCEPTRON.TRAIN")
	.Arguments({
		Arg(XLL_FP, L"w", L"is an array of weights."),
		Arg(XLL_FP, L"x", L"is an array representing the input vector."),
		Arg(XLL_BOOL, L"y", L"is the label."),
		Arg(XLL_DOUBLE, L"alpha", L"is the learning rate. (default=1.0)", 1.0),
		Arg(XLL_UINT, L"n", L"is the maximum number of iterations. (default=100)", 100),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Train perceptron weights on single input vector and label.")
);
_FP12* WINAPI xll_perceptron_train(_FP12* pw, _FP12* px, BOOL y, double alpha, UINT n)
{
#pragma XLLEXPORT
	try {
		ensure(size(*pw) == size(*px) || !"weight and input vector size mismatch");
		
		alpha = alpha ? alpha : 1;
		n = n ? n : 100;

		train(size(*pw), pw->array, px->array, y, alpha, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCDNAME__ ": unknown exception");
	}
	
	return pw;
}

AddIn xai_neuron_(
	Function(XLL_HANDLEX, L"xll_neuron_", L"\\NEURON")
	.Arguments({
		Arg(XLL_FP, L"w", L"is an array of initial weights."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to a neuron with given weights.")
);
HANDLEX WINAPI xll_neuron_(_FP12* pw)
{	
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<neuron<>> h_(new neuron<>(size(*pw), pw->array));
		ensure(h_);

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCDNAME__ ": unknown exception");
	}

	return h;
}

AddIn xai_neuron(
	Function(XLL_FP, L"xll_neuron", L"NEURON")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle returned by \\NEURON."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return array of weights.")
);
_FP12* WINAPI xll_neuron(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX w;

	try {
		handle<neuron<>> h_(h);
		ensure(h_);

		std::span<double> s = h_->span();
		FPX w_((int)s.size(), 1, s.data());
		w.swap(w_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0; // 
	}
	catch (...) {
		XLL_ERROR(__FUNCDNAME__ ": unknown exception");
		return 0;
	}

	return w.get();
}

AddIn xai_neuron_update(
	Function(XLL_HANDLEX, L"xll_neuron_update", L"NEURON.UPDATE")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle to a neuron."),
		Arg(XLL_FP, L"x", L"is an array representing the input vector."),
		Arg(XLL_BOOL, L"y", L"is the label."),
		Arg(XLL_DOUBLE, L"alpha", L"is the learning rate. (defaul 1.0)", 1.0),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle of updated neuron.")
);
HANDLEX WINAPI xll_neuron_update(HANDLEX h, _FP12* px, BOOL y, double alpha)
{
#pragma XLLEXPORT
	try {
		handle<neuron<>> h_(h);
		ensure(h_);

		alpha = alpha ? alpha : 1;
		
		h_->update(px->array, y, alpha);
	}
	catch (const std::exception& ex) {
		h = INVALID_HANDLEX;
		XLL_ERROR(ex.what());
	}
	catch (...) {
		h = INVALID_HANDLEX;
		XLL_ERROR(__FUNCDNAME__ ": unknown exception");
	}

	return h;
}

AddIn xai_neuron_train(
	Function(XLL_FP, L"xll_neuron_train", L"NEURON.TRAIN")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle to a neuron."),
		Arg(XLL_FP, L"x", L"is an array representing the input vector."),
		Arg(XLL_BOOL, L"y", L"is the label."),
		Arg(XLL_DOUBLE, L"alpha", L"is the learning rate. Default 1.", 1.0),
		Arg(XLL_UINT, L"n", L"is the maximum number of iterations. Default 100.", 100),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return {handle, steps} after training a point.")
);
_FP12* WINAPI xll_neuron_train(HANDLEX h, _FP12* px, BOOL y, double alpha, UINT n)
{
#pragma XLLEXPORT
	static FPX w(1,2); // 1 x 2 array of doubles

	try {
		handle<neuron<>> h_(h);
		ensure(h_);
		ensure(size(*px) == h_->span().size() || !"input vector size mismatch");

		alpha = alpha ? alpha : 1.0;
		n = n ? n : 100;
		auto m = h_->train(px->array, y, alpha, n);

		w[0] = h;
		w[1] = static_cast<double>(m);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}
	catch (...) {
		XLL_ERROR(__FUNCDNAME__ ": unknown exception");
		return 0;
	}

	return w.get();
}
