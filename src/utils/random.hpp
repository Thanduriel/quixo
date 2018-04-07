#pragma once
#include <random>
#include <cstdint>
#include <limits>

namespace Utils {

	// basic template for single seed based generators
	template <typename Impl >
	class BaseGen : public Impl
	{
	public:
		typedef uint32_t result_type;
		static constexpr result_type default_seed = 0x14bfc13U;

		explicit BaseGen(result_type _seed = default_seed)
		{
			Impl::m_seed = _seed;
			Impl::init();
		}

		void seed(result_type _seed)
		{
			Impl::m_seed = _seed;
			Impl::init();
		}

		static constexpr result_type(min)()
		{	// return minimum possible generated value
			return 1;
		}

		static constexpr result_type(max)()
		{	// return maximum possible generated value
			return std::numeric_limits<result_type>::max();
		}

		void discard(result_type _num)
		{
			for (; 0 < _num; --_num)
				(*this)();
		}
	protected:
	};

	// decent and fast generator with  a period of 2^32
	class Xorshift32Impl
	{
	public:
		uint32_t operator()();

	protected:
		void init()
		{
			m_state = m_seed;
		}
		uint32_t m_seed;
	private:
		uint32_t m_state;
	};

	typedef BaseGen<Xorshift32Impl> Xorshift32;

	// slower variant with a period of 2^128
	class Xorshift128Impl
	{
	public:
		uint32_t operator()();


	protected:
		void init();
		uint32_t m_seed;
	private:
		uint32_t m_state[4];
	};

	typedef BaseGen<Xorshift128Impl> Xorshift128;

	typedef Xorshift128 DefaultRandomGen;

	// Provides a random value that should only be used to seed a generator.
	// For some random numbers use g_random instead.
	uint32_t RandomSeed();

	// a simple sampler for most common distributions
	template <typename GenT = DefaultRandomGen>
	class RandomSampler
	{
	public:
		RandomSampler(GenT& _generator) : m_generator(_generator) {};

		int32_t Uniform(int32_t _min, int32_t _max)
		{
			uint32_t interval = uint32_t(_max - _min + 1);
	
			uint32_t value = m_generator();
			return _min + static_cast<int32_t>(value % interval);
		}

		float Uniform(float _min = 0.f, float _max = 1.f)
		{
			double scale = (_max - _min) / 0xffffffff;
			return float(_min + scale * m_generator());
		}

		float Uniform(std::pair<float, float> _range)
		{
			return Uniform(_range.first, _range.second);
		}

		// normal distribution
		float Normal(float _variance)
		{
			// Make several turns for a good quality (addition of two standard
			// normal distributions is again a standard normal distribution).
			double n = 0.0;
			for (int i = 0; i<3; ++i)
			{
				// Box-Muller method
				float u1 = Uniform();
				float u2 = Uniform();
				double r;
				if (u1 <= 0.0f) r = 1487.0;	// -2*log(e-323) == 1487.493849482
				else if (u1 >= 1.0f) continue;
				else r = sqrt(-2.0*log(u1));
				// Use both independent samples to generate one sample of a
				// higher quality.
				n += r*cos(6.28318530718*u2) + r*sin(6.28318530718*u2);
			}
			return float(sqrt(_variance / 6.0) * n);
		}

		// sample exponential distribution
		float Exponential(float _lambda)
		{
			double n = 0.0;
			for (int i = 0; i<5; ++i)
			{
				float u = Uniform();
				if (u <= 0.0f) n += 743.7;	// -log(e-323) == 743.746924741
				else if (u >= 1.0f) continue;
				n += -log(u);
			}
			return float(n / (_lambda * 5.0));
		}
	private:
		GenT& m_generator;
	};

	// The combination of a generator and a sampler for ease of use
	// if you just want some random numbers.
	class RandomGenerator : public DefaultRandomGen, public RandomSampler<DefaultRandomGen>
	{
	public:
		RandomGenerator(uint32_t _seed) : 
			DefaultRandomGen(_seed),
			RandomSampler<DefaultRandomGen>(static_cast<DefaultRandomGen&>(*this))
		{}
	};

	// A global instance if no deterministic sequence is required.
	extern thread_local RandomGenerator g_random;
}