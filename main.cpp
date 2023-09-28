#include <iostream>
#include <fstream>
#include <Windows.h>


class Time {
public:
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
			if(minute == 60){
				minute = 0;
				hour++;
			}
		}

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

	Time ScreenTime;
	int SyncTime = 1;

	bool FileExists = fileExists("data.bin");
	if (FileExists) {
		ScreenTime = retrieveData();
	}

	while (true)
	{	
		std::cout << "######################" << std::endl;
		std::cout << "DIGITAL WELLBEING" << std::endl;
		std::cout << "######################\n\n";
		std::cout << "Screen Time:";
		ScreenTime.printTime();
		Sleep(SyncTime * 1000);
		ScreenTime.incrementTime();
		saveData(ScreenTime);
		system("cls");
		
	}
	
	return 0;
}