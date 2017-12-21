#pragma once
#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;
void part1();
void part2();
void printHeader();

class Student {
protected:
	char name[21];
	char ssn[10];
	float gpa;
	int credits;
public:
	Student(string name_, string ssn_, int credits_, float gpa_) {
		strcpy_s(name, name_.c_str());
		strcpy_s(ssn, ssn_.c_str());
		gpa = gpa_;
		credits = credits_;
	}
	virtual void print() {
		cout << "Name: " << name << endl;
		cout << "SSN: " << ssn << endl;
		cout << "GPA: " << gpa << endl;
		cout << "Credits: " << credits << endl;
	}
	virtual float tuition() = 0;
};

class Undergrad : public Student {
protected:
	float undergrad_rate;
	char* year;
public:
	Undergrad(string name_, string ssn_, char* year_, int credits_, float undergrad_rate_, float gpa_) : Student(name_, ssn_, credits_, gpa_) {
		undergrad_rate = undergrad_rate_;
		year = year_;
	}
	char* get_year() {
		return year;
	}
	float get_rate() {
		return undergrad_rate;
	}
	void set_year(char* newYear) {
		year = newYear;
	}
	float get_gpa() {
		return gpa;
	}
	virtual void print() override {
		Student::print();
		cout << "Undergraduate rate: " << undergrad_rate << endl;
		cout << "Year: " << year << endl;
	}
	virtual float tuition() {
		return (undergrad_rate * credits);
	}
	bool operator < (Undergrad& u)
	{
		return this->get_gpa() < u.get_gpa();
	}
	void printSorted() {
		cout.width(20);
		cout << left << name;
		cout.width(10);
		cout << left << ssn;
		cout.width(1);
		cout << left << " ";
		cout.width(10);
		cout << left << year;
		cout.width(1);
		cout << left << " ";
		cout.width(10);
		cout << left << credits;
		cout.width(1);
		cout << "  $";
		cout.width(10);
		cout << right << tuition();
		cout.width(10);
		cout << right << gpa;
		cout.width(1);
		cout << left << " ";
		cout << endl;
	}
};


class Grad : public Student {
protected:
	char* thesis;
	float grad_rate;
public:
	Grad(string name_, string ssn_, char* year, int credits_, float grad_rate_, char* thesis_, float gpa_) : Student(name_, ssn_, credits_, gpa_) {
		thesis = thesis_;
		grad_rate = grad_rate_;

	}
	char* get_thesis() {
		return thesis;
	}
	float get_rate() {
		return grad_rate;
	}
	void set_thesis(char* newThesis) {
		thesis = newThesis;
	}
	void print() {
		Student::print();
		cout << "Graduate rate: $" << get_rate() << endl;
		cout << "Thesis: " << get_thesis() << endl;
	}
	float tuition() {
		return grad_rate * credits;
	}
};
class GradAsst : public Grad {
protected:
	float hourPay;
	char* superviser;
	char* task;
	float gpa;

public:
	GradAsst(string name_, string ssn_, char* year_, int credits_, float grad_rate_, char* thesis_, float gpa_, char* task_, float hourPay_, char* superviser_)
		: Grad(name_, ssn_, year_, credits_, grad_rate_, thesis_, gpa_) {
		superviser = superviser_;
		task = task_;
		hourPay = hourPay_;

	}
	char* get_superviser() {
		return superviser;
	}
	float get_hourPay() {
		return hourPay;
	}
	void set_superviser(char* newSuperv) {
		superviser = newSuperv;
	}
	void set_hourPay(float newPay) {
		hourPay = newPay;
	}
	char* get_task() {
		return task;
	}
	void set_task(char* newTask) {
		task = newTask;
	}
	void print() {
		Grad::print();
		cout << "Hourly Pay: $" << get_hourPay() << endl;
		cout << "Superviser: " << get_superviser() << endl;
		cout << "Task: " << get_task() << endl;
	}
	float tuition() {
		return grad_rate * credits;
	}
};

void part1() {
	float undergrad_rate = 380.0;
	float grad_rate = 500.0;
	//static part
	Undergrad uStudent("Mary", "000111222", "Junior", 12, undergrad_rate, (float)4.0);
	Grad gStudent("David", "111222333", "Graduate", 9, grad_rate, "How to learn data structures using C++ / STL?", (float)3.7);
	GradAsst gAsst("Jason", "222333444", "Graduate Assistant", 9, grad_rate, "Design of efficient algorithms", (float)3.9, "Grading 330 projects and implementing a data mining algorithm", 20, "Dr. Fu");
	uStudent.print();
	cout << "Undergraduate tuition: $" << uStudent.tuition() << "\n" << endl;
	gStudent.print();
	cout << "Graduate tuition: $" << gStudent.tuition() << "\n" << endl;
	gAsst.print();
	cout << "Graduate assistant tuition: $" << gAsst.tuition() << "\n" << endl;

	//dynamic part
	Student *studentptr = &uStudent;
	studentptr->print();
	cout << "Undergraduate tuition: $" << studentptr->tuition() << "\n" << endl;
	studentptr = &gStudent;
	studentptr->print();
	cout << "Undergraduate tuition: $" << studentptr->tuition() << "\n" << endl;
	studentptr = &gAsst;
	studentptr->print();
	cout << "Undergraduate tuition: $" << studentptr->tuition() << "\n" << endl;

	studentptr = 0;
	delete studentptr;
}

void part2() {
	//data declarations
	ifstream inFile("Records.txt");
	string currLineSegment;
	vector<string> seglist;
	string name;
	string ssn;
	char* year;
	int credits;
	float gpa;
	list<Undergrad> students;

	//seperate data using space as a deliminator 
	while (getline(inFile, currLineSegment, ' ')) {
		//checks to see if there's a \n at the end of the line.
		//if there's a \n, break that loop and continue to construct another student.
		if (currLineSegment.find('\n') != string::npos) {
			seglist.push_back(currLineSegment.substr(0, currLineSegment.find('\n')));
			seglist.push_back(currLineSegment.substr(currLineSegment.find('\n') + 1, currLineSegment.length()));
			continue;
		}
		seglist.push_back(currLineSegment);
	}
	//Adding populating fields from strings from file
	//and adding them to the list one by one
	//This will only work with the format:
	//FirstName LastName SSN Year credits gpa
	//Any different formats will break the program.
		while (!seglist.empty()) {
			istringstream(seglist.back()) >> gpa;
			seglist.pop_back();
			istringstream(seglist.back()) >> credits;
			seglist.pop_back();
			year = _strdup(seglist.back().c_str());
			seglist.pop_back();
			ssn = seglist.back();
			seglist.pop_back();
			string lastName = seglist.back();
			seglist.pop_back();
			name = seglist.back();
			seglist.pop_back();
			name += " ";
			name += lastName;
			Undergrad student(name, ssn, year, credits, 380.0, gpa);
			students.push_back(student); // add this student to the back of list
		}
	//move the backmost student to the front
	Undergrad temp = students.back();
	students.pop_back();
	students.push_front(temp);

	//copy the unsorted students to the soon to be sorted students vector.
	//Also prints unsorted students.
	vector<Undergrad> sortedStudents;
	printHeader();
	for (list<Undergrad>::iterator it = students.begin(); it != students.end(); ++it) {
		sortedStudents.push_back(*it);
		it->printSorted();
	}
	//Ascending sorting of student GPAs.
	sort(sortedStudents.begin(), sortedStudents.end());
	cout << "\n" << "-------Now printing sorted students-------" << "\n" << endl;
	printHeader();
	//Print sorted students.
	for (Undergrad u : sortedStudents)
	{
		u.printSorted();
	}
};

void printHeader() {
	cout.width(20);
	cout << left << "Name";
	cout.width(10);
	cout << left << "SSN ";
	cout.width(1);
	cout << left << " ";
	cout.width(10);
	cout << left << "Year";
	cout.width(1);
	cout << left << " ";
	cout.width(10);
	cout << left << "Credits";
	cout.width(1);
	cout << right << "  ";
	cout.width(10);
	cout << right << "Tuition";
	cout.width(10);
	cout << right << "GPA";
	cout.width(1);
	cout << left << " ";
	cout << endl;
};

int main() {
	part1();
	part2();

	return 0;
}