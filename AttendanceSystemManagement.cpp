#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

// ANSI color codes for use consol text color
const string RED = "\033[91m";
const string DARK_BLUE = "\033[34m";
const string GREEN = "\033[92m";
const string YELLOW = "\033[93m";
const string BLUE = "\033[94m";
const string RESET = "\033[0m";

class Teacher
{
public:
    string username;
    string password;

    Teacher(const string &username, const string &password) : username(username), password(password) {}
};
class Student
{
public:
    int studentID;
    Student(int studentID) : studentID(studentID) {}
};
class Subject
{
public:
    string name;
    map<int, vector<int>> attendance; // Key: Student ID, Value: Vector of daily attendance

    Subject(const string &name) : name(name) {}

    void addStudent(int studentID)
    {
        vector<int> dailyAttendance;
        // Initialize as absent (0) for all days
        for (int i = 0; i < 31; ++i)
        {
            dailyAttendance.push_back(0);
        }
        attendance[studentID] = dailyAttendance;
    }

    void takeAttendance()
    {
        cout << "Taking attendance for " << name << endl;
        int day;

        // Prompt for the day
        cout << BLUE << "Enter the day for which you want to take attendance: " << RESET;
        cin >> day;

        // Check if the provided day is within a valid range (1-31)
        if (day < 1 || day > 31)
        {
            cout << RED << "Invalid day. Attendance not taken." << RESET << endl;
            return;
        }

        for (auto &entry : attendance)
        {
            int studentID = entry.first;
            int status;
            cout << BLUE << "Is student with ID " << studentID << "  present on day " << day << "? (1 for present, 0 for absent): " << RESET;
            cin >> status;

            // Update attendance without overwriting previous data for the specific day
            entry.second[day - 1] = status;
        }

        // Calculate and display the total number of students present and absent
        int totalPresent = 0;
        int totalAbsent = 0;

        for (auto &entry : attendance)
        {
            if (entry.second[day - 1] == 1)
            {
                totalPresent++;
            }
            else
            {
                totalAbsent++;
            }
        }

        cout << YELLOW << "Total Present: " << totalPresent << RESET << endl;
        cout << RED << "Total Absent: " << totalAbsent << RESET << endl;

        saveAttendanceForDay(day);
        cout << BLUE << "Attendance taken for " << name << " on day " << day << RESET << endl;
    }

    void saveAttendanceForDay(int day)
    {
        ofstream file(name + "_attendance.csv", ios::app); // Open the file in append mode

        if (!file.is_open())
        {
            cerr << RED << "Error: Could not open the file for saving attendance data." << RESET << endl;
            return;
        }

        // Check if the file is empty, and if so, write the column headers
        if (file.tellp() == 0)
        {
            file << "Day";
            for (auto &entry : attendance)
            {
                file << "," << entry.first;
            }
            file << endl;
        }

        file << day; // Write the day as the first column
        for (auto &entry : attendance)
        {
            file << "," << entry.second[day - 1];
        }
        file << endl;

        file.close();
        cout << YELLOW << "Attendance data for " << name << " on day " << day << " saved to " << name + "_attendance.csv" << RESET << endl;
    }
};
int main()
{

    return 0;
}