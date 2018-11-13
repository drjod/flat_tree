#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using namespace std::chrono;

struct Timer
{
	static high_resolution_clock::time_point get_time_point()
	{
		return high_resolution_clock::now();
	}

	void start()
	{
		std::cout << "***** Start timer\n";
		_start = get_time_point();
	}

	double stop() const
	{
		auto time_elapsed = duration_cast<std::chrono::microseconds>(
					get_time_point() - _start);
		std::cout << "***** Time elapsed (ms): " << time_elapsed.count() << "\n";
		return time_elapsed.count();
	}
private:
	high_resolution_clock::time_point _start;
};

#endif
