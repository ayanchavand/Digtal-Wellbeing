#include <iostream>
#include <Windows.h>

SYSTEMTIME timeDifference(SYSTEMTIME a) {
	
	SYSTEMTIME endTime, diff;
	GetLocalTime(&endTime);

	int minDiff = endTime.wMinute - a.wMinute;
	int hourDiff = endTime.wHour - a.wHour;
	diff.wMinute = minDiff;
	diff.wHour = hourDiff;

	return diff;
	
}

int main() {

	int SyncTime = 60;
	SYSTEMTIME startTime, screenTime;
	GetLocalTime(&startTime);

	while (true)
	{	
		std::cout << "######################" << std::endl;
		std::cout << "DIGITAL WELLBEING" << std::endl;
		std::cout << "######################\n\n";
		std::cout << "Session started at: " << startTime.wHour << ":" << startTime.wMinute << std::endl;
		screenTime = timeDifference(startTime);
		std::cout << "Screen Time:" << screenTime.wHour << ":" << screenTime.wMinute << std::endl;
		Sleep(SyncTime * 1000);
		system("cls");
		
	}
	
	return 0;
}