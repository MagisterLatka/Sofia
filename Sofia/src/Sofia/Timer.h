#pragma once

namespace Sofia {

	class Timer
	{
	public:
		SOF_CORE Timer() noexcept { Reset(); }

		SOF_CORE void Reset() noexcept
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		SOF_CORE double Elapsed() noexcept
		{
			//note: nanoseconds are required here, because microseconds are not enough on linux (process is running too fast)
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001 * 0.001 * 0.001;
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}
