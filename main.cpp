#include <iostream> //Std IO
#include <fstream> //Data handling
#include <Windows.h> //Win API

//Path for the Binary file to be saved
std::string path = "S:\\data\\data.bin";

//Time class for better data management
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

//Returns true if file exists
bool fileExists(std::string filename) {
	std::ifstream infile(filename);
	return infile.good();
}

//Saves Time object into the .bin
int saveData(Time time) {
	std::ofstream outfile(path, std::ios::binary);
	if (outfile.is_open()) {
		outfile.write(reinterpret_cast<char*>(&time), sizeof(time));
	}
	outfile.close();
	return 0;
}

//Retrives Time from the .bin file
Time retrieveData() {
	std::ifstream infile(path, std::ios::binary);

	if (infile.is_open()) {
		// Create an object of MyClass to store the read data
		Time time;
		infile.read(reinterpret_cast<char*>(&time), sizeof(time));
		infile.close();
		return time;
	}
}

//Initializes default variables
void initialize(Time& ScreenTime, Time& StartTime, std::string path) {

	//Default initialization 
	StartTime.getCurrentTime();
	ScreenTime.inititializeDate();

	if (fileExists(path)) {
		//Retrive data if file exists
		ScreenTime = retrieveData();
	}
}

int main() {

	//Initialization
	Time ScreenTime, StartTime, CurrentTime;
	int SyncTime = 1;
	initialize(ScreenTime, StartTime, path);

	//Loops until program is terminated
	while (true)
	{	
		CurrentTime.getCurrentTime();

		std::cout << "######################" << std::endl;
		std::cout << "DIGITAL WELLBEING" << std::endl;
		std::cout << "######################\n\n";

		std::cout << "Session Started At: ";
		StartTime.printTime();

		std::cout << "Current Time: ";
		CurrentTime.printTime();

		std::cout << "Screen Time: ";
		ScreenTime.printTime();

		//Resets the screen time if data has changed
		if (CurrentTime.date != ScreenTime.date) {
			ScreenTime.reset();
			ScreenTime.inititializeDate();
		}

		Sleep(SyncTime * 1000);
		ScreenTime.incrementTime();
		saveData(ScreenTime);

		system("cls");

	}
	return 0;
}