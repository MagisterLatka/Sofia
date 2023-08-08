#pragma once

namespace Sofia {

	class Timer
	{
	public:
		Timer() noexcept { Reset(); }

		void Reset() noexcept
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		double Elapsed() noexcept
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001 * 0.001;
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}
