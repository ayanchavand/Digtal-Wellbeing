#include <iostream>
#include <fstream>
#include <Windows.h>


class Time {
public:
	int date = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	void printTime() {
		std::cout << hour << ":" << minute << ":" << second << std::endl;
	}

	void incrementTime() {
		second++;
		if (second == 60) {
			second = 0;
			minute++;
			if (minute == 60) {
				minute = 0;
				hour++;
			}
		}

	}

	void getCurrentTime() {
		SYSTEMTIME systemtime;
		GetLocalTime(&systemtime);

		date = systemtime.wDay;
		hour = systemtime.wHour;
		minute = systemtime.wMinute;
		second = systemtime.wSecond;
	}

	void getCurrentDate() {
		SYSTEMTIME systemtime;
		GetLocalTime(&systemtime);

		date = systemtime.wDay;
	}

	void reset() {
		date = 0;
		hour = 0;
		minute = 0;
		second = 0;
	}

	void inititializeDate() {
		SYSTEMTIME systemtime;
		GetLocalTime(&systemtime);

		date = systemtime.wDay;
		hour = 0;
		minute = 0;
		second = 0;
	}
};

bool fileExists(const char* filename) {
	std::ifstream infile(filename);
	return infile.good();
}

int saveData(Time time) {
	std::ofstream outfile("data.bin", std::ios::binary);
	if (outfile.is_open()) {
		// Write the array to the file
		outfile.write(reinterpret_cast<char*>(&time), sizeof(time));
		return 0;
	}
	// Close the file
	outfile.close();
}


Time retrieveData() {
	std::ifstream infile("data.bin", std::ios::binary);

	if (infile.is_open()) {
		// Create an object of MyClass to store the read data
		Time time;
		infile.read(reinterpret_cast<char*>(&time), sizeof(time));
		infile.close();
		return time;
	}
}

int main() {

	Time ScreenTime, StartTime;
	int SyncTime = 1;

	StartTime.getCurrentTime();
	ScreenTime.inititializeDate();

	bool FileExists = fileExists("data.bin");

	if (FileExists) {
		ScreenTime = retrieveData();
		if (StartTime.date != ScreenTime.date) {
			ScreenTime.reset();
			ScreenTime.inititializeDate();
		}
	}



	while (true)
	{
		std::cout << "######################" << std::endl;
		std::cout << "DIGITAL WELLBEING" << std::endl;
		std::cout << "######################\n\n";
		std::cout << "Session Started At: ";
		StartTime.printTime();
		std::cout << "Screen Time: ";
		ScreenTime.printTime();

		Sleep(SyncTime * 1000);
		ScreenTime.incrementTime();
		saveData(ScreenTime);

		system("cls");

	}

	return 0;
}