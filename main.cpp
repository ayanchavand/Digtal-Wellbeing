#include <iostream> //Std IO
#include <fstream> //Data handling
#include <Windows.h> //Win API
#include<string>
//Path for the Binary file to be saved
std::string path = "S:\\data\\data.bin";

//Time class for better data management
class Time {

public:
	int date = 0;
	int month = 0;
	int year = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	void printTime() {
		std::cout << hour << ":" << minute << ":" << second << std::endl;
	}
	void printDate() {
		std::cout << date << "-" << month << "-" << year << std::endl;
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
		month = systemtime.wMonth;
		year = systemtime.wYear;
		hour = systemtime.wHour;
		minute = systemtime.wMinute;
		second = systemtime.wSecond;
	}

	std::wstring returnTimeAsWstring() {
		
		std::wstring string;
		string.append(std::to_wstring(hour));
		string.append(L":");
		string.append(std::to_wstring(minute));
		string.append(L":");
		string.append(std::to_wstring(second));

		return string;
	}

	std::wstring returnDateAsWstring() {

		std::wstring string;
		string.append(std::to_wstring(date));
		string.append(L"-");
		string.append(std::to_wstring(month));
		string.append(L"-");
		string.append(std::to_wstring(year));

		return string;
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
		month = systemtime.wMonth;
		year = systemtime.wYear;
		hour = 0;
		minute = 0;
		second = 0;
	}
};

class SaveData {
public:
	Time Screetime;
	int reboots = 0;

	void printData() {
		std::cout << "\n########################\n";
		std::cout << "Date :";
		Screetime.printDate();
		std::cout << "------------------------\n";
		std::cout << "Screen Time: ";
		Screetime.printTime();
		std::cout << "------------------------\n";
		std::cout << "Number Of Reboots: " << reboots << std::endl;
		std::cout << "########################\n";
	}


};

// Function to show a notification
void ShowNotification(const std::wstring& title, const std::wstring& message, int time, const std::wstring& soundPath) {
	// Initialize COM for this thread
	CoInitialize(0);

	PlaySound(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);

	// Create the notification icon
	NOTIFYICONDATAW nid;
	ZeroMemory(&nid, sizeof(NOTIFYICONDATAW));
	nid.cbSize = sizeof(NOTIFYICONDATAW);
	nid.hWnd = GetConsoleWindow(); // Assuming it's a console application
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_USER + 1;
	nid.hIcon = LoadIconW(NULL, IDI_INFORMATION);

	// Convert string data to wide characters
	wcsncpy_s(nid.szTip, _countof(nid.szTip), L"Notification Tip", _TRUNCATE);

	// Add the notification
	Shell_NotifyIconW(NIM_ADD, &nid);

	// Show the notification
	nid.uFlags = NIF_INFO;
	wcsncpy_s(nid.szInfoTitle, _countof(nid.szInfoTitle), title.c_str(), _TRUNCATE);
	wcsncpy_s(nid.szInfo, _countof(nid.szInfo), message.c_str(), _TRUNCATE);
	nid.dwInfoFlags = NIIF_INFO;
	nid.uTimeout = time * 1000; // Display time in milliseconds

	Shell_NotifyIconW(NIM_MODIFY, &nid);

	// Remove the notification
	Shell_NotifyIconW(NIM_DELETE, &nid);

	// Uninitialize COM for this thread
	CoUninitialize();
}
//Returns true if file exists
bool fileExists(std::string filename) {
	std::ifstream infile(filename);
	return infile.good();
}

//Saves Time object into the .bin
int saveData(Time time, int reboots) {
	
	SaveData data;
	data.Screetime = time;
	data.reboots = reboots;

	std::ofstream outfile(path, std::ios::binary);
	if (outfile.is_open()) {
		outfile.write(reinterpret_cast<char*>(&data), sizeof(data));
	}
	outfile.close();
	return 0;
}

//Retrives Time from the .bin file
SaveData retrieveData() {
	std::ifstream infile(path, std::ios::binary);

	if (infile.is_open()) {
		// Create an object of MyClass to store the read data
		SaveData data;
		infile.read(reinterpret_cast<char*>(&data), sizeof(data));
		infile.close();
		return data;
	}
}
SaveData data;
//Initializes default variables
void initialize(Time& ScreenTime, Time& StartTime, int& reboots, std::string path) {

	data.Screetime.inititializeDate();
	//Default initialization 
	StartTime.getCurrentTime();
	ScreenTime.inititializeDate();

	if (fileExists(path)) {
		//Retrive data if file exists
		data = retrieveData();
		ScreenTime = data.Screetime;
		reboots = data.reboots;
	}
	else {
		data.Screetime.getCurrentDate();
	}
	reboots++;
}


int main() {
	HWND consoleWindow = GetConsoleWindow();
	SetConsoleTitle(L"Digital Wellbeing");
	if (!ShowWindow(consoleWindow, 0)) {

		std::cout << "Error minimizing " << std::endl;
	}
	//Initialization
	Time ScreenTime, StartTime, CurrentTime;
	int SyncTime = 1, reboots = 0, notif = 0;
	initialize(ScreenTime, StartTime, reboots, path);
	ShowNotification(L"Digital Wellbeing Is Active", L"Have A Great Day", 20, L"notification.wav");
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
			std::wstring title = L"Stats for ";
			std::wstring des = L"Your Screen time was ";

			title.append(ScreenTime.returnDateAsWstring());
			des.append(ScreenTime.returnTimeAsWstring());
			ShowNotification(title, des, 6, L"ringtone.wav");
			ScreenTime.reset();
			ScreenTime.inititializeDate();
			
		}

		if(ScreenTime.hour == 0){
			notif = ScreenTime.hour;
		}
		else {
			if (ScreenTime.hour != notif) {
				ShowNotification(L"Time To Stretch", L"You have been sitting for one hour :<", 6, L"untitled.wav");
				notif++;
			}
		}

		data.printData();
		Sleep(SyncTime * 1000);
		ScreenTime.incrementTime();
		saveData(ScreenTime, reboots);

		system("cls");

	}
	return 0;
}