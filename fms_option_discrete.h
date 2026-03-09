// fms_option_discrete.h - Discrete distribution for option pricing	
#pragma once
#include <valarray>
#include "fms_option.h"
#include <numbers>
#include "fms_math.h"

namespace fms::option::discrete {
	template<class F = double, class S = double>
	struct model : option::base<F, S> {
	public:
		std::valarray<F> xi, pi; // P(X = x_i) = p_i
	
		void normalize()
		{
			pi /= pi.sum(); // pi.sum() == 1
			F Ex = (xi * pi).sum(); // mean
			xi -= Ex; // mean 0
			xi /= std::sqrt((xi * xi * pi).sum()); // variance 1
		}
	public:
		model(std::size_t n, const F* x, const F* p)
			: xi(x, n), pi(p, n)
		{
			normalize();
		}

		model(const model&) = default;
		model& operator=(const model&) = default;
		model(model&&) = default;
		model& operator=(model&&) = default;
		virtual ~model() = default;
	
		// E[exp(s X - kappa(s)) 1(X <= x) ] 
		//   = sum_{x_i <= x} exp(s x_i - kappa(s)) pi_i
		F _cdf(F x, S s) const override
		{
			double sum = 0;
			for (int i = 0; i < x; ++i) {
				sum += exp(s * xi[i] - _cgf(s)) * pi[i];
			}
			return sum; // TODO: implement
		}
	
		// kappa(s) = log E[exp(s X)] = log sum p_i exp(s x_i)
		S _cgf(S s) const override
		{
			double result = 0;
			for (int i = 0; i < xi.size(); ++i) {
				result += pi[i] * exp(s * xi[i]);
			}
			return log(result); // TODO: implement
		}
	};
} // namespace fms::option::discrete

// TODO: Create xll_option_discrete.cpp based on xll_option_normal.cpp
// TODO: Implement add-in for \OPTION.DISCRETE
// TODO: Implement add-in for OPTION.DISCRETE to return normalized xi values
// TODO: Load add-in and follow comments in final.xlsx.
// TODO: Put link to your GitHub repository on Brightspace submission.