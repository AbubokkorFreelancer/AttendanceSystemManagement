#include <iostream>
#include <string>
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

int main()
{

    return 0;
}