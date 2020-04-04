#include <windows.h>
#include <boost/timer.hpp>
#include <boost/progress.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
int main() {
	boost::progress_display dis(12);
	for (int i=0;i<1;i++) {
		++dis;
		Sleep(1000);
	}
	boost::gregorian::date abc(1980, 1, 2);
	auto week = abc.day_of_week();
}