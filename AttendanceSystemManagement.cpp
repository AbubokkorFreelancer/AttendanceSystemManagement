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

vector<Teacher> teachers;
vector<Subject> subjects;

Teacher *currentTeacher = nullptr;
int subjectChoice;

Teacher *loginTeacher()
{
    string username, password;
    cout << BLUE << "Enter your username: " << RESET;
    cin >> username;
    cout << GREEN << "Enter your password: " << RESET;
    cin >> password;

    for (Teacher &teacher : teachers)
    {
        if (teacher.username == username && teacher.password == password)
        {
            cout << YELLOW << "Login successful!" << RESET << endl;
            return &teacher;
        }
    }

    cout << RED << "Login failed. Invalid username or password." << RESET << endl;
    return nullptr;
}

void saveTeacherDataToFile()
{
    ofstream file("teachers.csv");
    for (Teacher &teacher : teachers)
    {
        file << teacher.username << "," << teacher.password << endl;
    }
    file.close();
    cout << "Teacher data saved to teachers.csv" << endl;
}

void saveStudentDataToFile(const Subject &subject)
{
    ofstream file(subject.name + "_students.csv");
    for (const auto &entry : subject.attendance)
    {
        file << entry.first << endl;
    }
    file.close();
    cout << "Student data saved to " << subject.name << "_students.csv" << endl;
}

void saveSubjectDataToFile()
{
    ofstream file("subjects.csv");
    for (const Subject &subject : subjects)
    {
        file << subject.name << endl;
    }
    file.close();
    cout << "Subject data saved to subjects.csv" << endl;
}

void loadTeacherDataFromFile()
{
    ifstream file("teachers.csv");
    string line;
    while (getline(file, line))
    {
        vector<string> tokens;
        string token;
        for (char c : line)
        {
            if (c == ',')
            {
                tokens.push_back(token);
                token.clear();
            }
            else
            {
                token += c;
            }
        }
        if (!token.empty())
        {
            tokens.push_back(token);
        }
        if (tokens.size() == 2)
        {
            teachers.push_back(Teacher(tokens[0], tokens[1]));
        }
    }
    file.close();
    cout << "Teacher data loaded from teachers.csv" << endl;
}

void loadStudentDataFromFile(Subject &subject)
{
    ifstream file(subject.name + "_students.csv");
    string line;
    while (getline(file, line))
    {
        int studentID;
        if (line.empty())
            continue;
        try
        {
            studentID = stoi(line);
        }
        catch (...)
        {
            continue;
        }
        subject.addStudent(studentID);
    }
    file.close();
    cout << "Student data loaded from " << subject.name << "_students.csv" << endl;
}

void loadSubjectDataFromFile()
{
    ifstream file("subjects.csv");
    string line;
    while (getline(file, line))
    {
        if (!line.empty())
        {
            subjects.push_back(Subject(line));
            loadStudentDataFromFile(subjects.back());
        }
    }
    file.close();
    cout << "Subject data loaded from subjects.csv" << endl;
}

void registerTeacher()
{
    string username, password;
    cout << BLUE << "Enter a username: " << RESET;
    cin >> username;
    cout << GREEN << "Enter a password: " << RESET;
    cin >> password;
    teachers.push_back(Teacher(username, password));
    saveTeacherDataToFile();
    cout << YELLOW << "Teacher registered successfully!" << RESET << endl;
}

void createSubject()
{
    if (currentTeacher == nullptr)
    {
        cout << "Please log in first." << endl;
        return;
    }

    string name;
    cout << "Enter the subject name: ";
    cin >> name;
    subjects.push_back(Subject(name));
    saveSubjectDataToFile();
    saveStudentDataToFile(subjects.back());
    cout << "Subject created successfully!" << endl;
}

void addStudentToSubject(Subject &subject)
{
    if (currentTeacher == nullptr)
    {
        cout << "Please log in first." << endl;
        return;
    }

    int numStudents;
    cout << "How many students do you want to add? ";
    cin >> numStudents;

    for (int i = 0; i < numStudents; ++i)
    {
        int studentID;
        cout << "Enter student ID " << i + 1 << ": ";
        cin >> studentID;
        subject.addStudent(studentID);
    }
    saveStudentDataToFile(subject);
    cout << numStudents << " student(s) added to " << subject.name << " successfully!" << endl;
}

int main()
{
    loadTeacherDataFromFile();
    loadSubjectDataFromFile();

    while (true)
    {
        int choice;
        cout << DARK_BLUE << "***** Attendance Management System *****" << RESET << endl;
        cout << "1. " << GREEN << "Register Teacher" << RESET << endl;
        cout << "2. " << GREEN << "Login Teacher" << RESET << endl;
        cout << "3. " << GREEN << "Create Subject" << RESET << endl;
        cout << "4. " << GREEN << "Add Student to Subject" << RESET << endl;
        cout << "5. " << GREEN << "Take Attendance" << RESET << endl;
        cout << "6. " << GREEN << "Save Attendance" << RESET << endl;
        cout << "7. " << RED << "Exit" << RESET << endl;
        cout << BLUE << "Enter your choice: " << RESET;
        cin >> choice;

        switch (choice)
        {
        case 1:
            registerTeacher();
            break;
        case 2:
            currentTeacher = loginTeacher();
            break;
        case 3:
            createSubject();
            break;
        case 4:
            if (currentTeacher == nullptr)
            {
                cout << "Please log in first." << endl;
                break;
            }

            if (subjects.empty())
            {
                cout << "No subjects created. Please create a subject first." << endl;
                break;
            }

            cout << "Select a subject to add a student to:" << endl;
            for (size_t i = 0; i < subjects.size(); ++i)
            {
                cout << i + 1 << ". " << subjects[i].name << endl;
            }
            cin >> subjectChoice;
            if (subjectChoice >= 1 && subjectChoice <= subjects.size())
            {
                addStudentToSubject(subjects[subjectChoice - 1]);
            }
            else
            {
                cout << "Invalid choice." << endl;
            }
            break;
        case 5:
            if (currentTeacher == nullptr)
            {
                cout << "Please log in first." << endl;
                break;
            }

            if (subjects.empty())
            {
                cout << "No subjects created. Please create a subject first." << endl;
                break;
            }

            cout << "Select a subject to take attendance for:" << endl;
            for (size_t i = 0; i < subjects.size(); ++i)
            {
                cout << i + 1 << ". " << subjects[i].name << endl;
            }
            cin >> subjectChoice;
            if (subjectChoice >= 1 && subjectChoice <= subjects.size())
            {
                subjects[subjectChoice - 1].takeAttendance();
            }
            else
            {
                cout << "Invalid choice." << endl;
            }
            break;
        case 6:
            cout << "Exiting the program. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    return 0;
}
