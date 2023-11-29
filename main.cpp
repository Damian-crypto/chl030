#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Student
{
public:
    int id;
    int classRank;
    int schoolRank;
    int totalMarks;
    unordered_map<string, int> marks;

    virtual void readMarks() {};

    virtual string toString()
    {
        stringstream ss;
        ss << "Student " << id << "\n";
        ss << "\tRanking:\n";
        ss << "\t\tClass rank: " << classRank << "\n";
        ss << "\t\tSchool rank: " << schoolRank << "\n";

        return ss.str();
    }
};

class ScienceStudent : public Student
{
public:
    virtual void readMarks() override
    {
        this->id = id;

        cout << "Student " << id << " Physics marks: ";
        cin >> marks["Physics"];
        cout << "Student " << id << " Chemistry marks: ";
        cin >> marks["Chemistry"];

        this->totalMarks += marks["Physics"] + marks["Chemistry"];
    }

    virtual string toString() override
    {
        stringstream ss;
        ss << Student::toString();
        ss << "\tScores:\n";
        ss << "\t\tPhysics marks: " << marks["Physics"] << "\n";
        ss << "\t\tChemistry marks: " << marks["Chemistry"] << "\n";

        return ss.str();
    }
};

class MathsStudent : public ScienceStudent
{
public:
    void readMarks() override
    {
        ScienceStudent::readMarks();
        cout << "Student " << id << " Combined Maths marks: ";
        cin >> marks["Maths"];

        this->totalMarks += marks["Maths"];
    }

    virtual string toString() override
    {
        stringstream ss;
        ss << ScienceStudent::toString();
        ss << "\t\tCombined Maths marks: " << marks["Maths"] << "\n";

        return ss.str();
    }
};

class BioStudent : public ScienceStudent
{
public:
    int bioMarks;

    void readMarks() override
    {
        ScienceStudent::readMarks();
        cout << "Student " << id << " Biology marks: ";
        cin >> marks["Biology"];

        this->totalMarks += marks["Biology"];
    }

    virtual string toString() override
    {
        stringstream ss;
        ss << ScienceStudent::toString();
        ss << "\t\tBiology marks: " << marks["Biology"] << "\n";

        return ss.str();
    }
};

class SchoolClass
{
public:
    static int id;
    vector<Student*> students;

    void addStudent(Student* stu)
    {
        stu->id = id++;
        students.push_back(stu);
    }

    void rankStudents()
    {
        sort(begin(students), end(students), [](Student* s1, Student* s2) {
            return s1->totalMarks > s2->totalMarks;
        });

        int prvMarks = -1;
        int currentRank = 0;
        for (Student* stu : students)
        {
            if (stu->totalMarks != prvMarks)
                currentRank++;
            prvMarks = stu->totalMarks;
            stu->classRank = currentRank;
        }
    }
};

int SchoolClass::id = 1;

class School
{
public:
    vector<SchoolClass> classes;

    void addClass(const SchoolClass &stu)
    {
        classes.push_back(stu);
    }

    void rankStudents()
    {
        int currentRank = 1;
        for (SchoolClass& cls : classes)
        {
            cls.rankStudents();

            for (Student* stu : cls.students)
            {
                stu->schoolRank = currentRank++;
            }
        }
    }

    void printToConsole()
    {
        for (SchoolClass& cls : classes)
        {
            for (Student* stu : cls.students)
            {
                cout << stu->toString() << "\n";
            }
        }
    }

    void writeToFile()
    {
        ofstream file("StudentRecord.txt");
        if (!file.is_open())
        {
            cout << "File error" << endl;
            return;
        }

        for (SchoolClass& cls : classes)
        {
            for (Student* stu : cls.students)
            {
                file << stu->toString() << "\n";
            }
        }
    }
};

int main()
{
    const int classSize = 2;
    const int bioClasses = 1;
    const int mathsClasses = 2;

    School school;

    string input;
    do
    {
        cout << "Press G to get student marks" << endl;
        cout << "Press C to print to console student details" << endl;
        cout << "Press S to store student details to a file" << endl;
        cout << "Press R to rank students" << endl;
        cout << "Press E to exit the program" << endl;
        cin >> input;

        if (input == "G")
        {
            // Bio students
            for (int cls = 1; cls <= bioClasses; cls++)
            {
                SchoolClass bioClass;
                for (int id = 1; id <= classSize; id++)
                {
                    BioStudent* stu = new BioStudent();
                    bioClass.addStudent(stu);
                    stu->readMarks();
                }
                school.addClass(bioClass);
            }

            // Maths students
            for (int cls = 1; cls <= bioClasses; cls++)
            {
                SchoolClass mathsClass;
                for (int id = 1; id <= classSize; id++)
                {
                    MathsStudent* stu = new MathsStudent();
                    mathsClass.addStudent(stu);
                    stu->readMarks();
                }
                school.addClass(mathsClass);
            }
        }
        else if (input == "R")
        {
            school.rankStudents();
        }
        else if (input == "S")
        {
            school.writeToFile();
        }
        else if (input == "C")
        {
            school.printToConsole();
        }
    } while (input != "E");

    return 0;
}
