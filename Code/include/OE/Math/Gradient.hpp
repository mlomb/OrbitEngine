#ifndef MATH_GRADIENT_HPP
#define MATH_GRADIENT_HPP

#include <vector>

#include "OE/Math/Math.hpp"

namespace OrbitEngine { namespace Math {

	template<typename T> class Gradient;

	/// Value stop within a Gradient
	template<typename T>
	class GradientStop {
	public:
		GradientStop(float time, const T& value)
			: time(time), value(value) { }

	private:
		friend class Gradient<T>;

		float time;
		T value;
	};

	/**
		@brief Linear gradient used for animations.
		@note time ∈ [0, 1]
	*/
	template<typename T>
	class Gradient {
	public:
		/// Remove all stops
		void reset() {
			m_Stops.clear();
		}

		/// Check if there are no stops defined
		bool empty() {
			return m_Stops.size() == 0;
		}

		/// Add a stop
		void addStop(float time, const T& value) {
			auto it = m_Stops.begin();
			for (; it != m_Stops.end(); it++)
				if ((*it).time > time) break;
			m_Stops.insert(it, GradientStop<T>(time, value));
		}

		/// Evaluate the gradient
		T evaluate(float time) {
			if (empty())
				return T();

			GradientStop<T> *start = NULL, *stop = NULL;

			auto it = m_Stops.begin();
			for (; it != m_Stops.end(); it++) {
				stop = &(*it);
				if (stop->time > time)
					break;
			}
			if (it == m_Stops.begin() || it == m_Stops.end())
				return stop->value;
			start = &(*(--it));
			float frac = (time - start->time) / (stop->time - start->time);
			return lerp(start->value, stop->value, frac);
		}

	private:
		std::vector<GradientStop<T>> m_Stops;
	};


} }

#endif