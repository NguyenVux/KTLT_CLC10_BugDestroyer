﻿#include "Student.h"

void Student::ViewInfo()
{
	cout << "ID: " << this->ID << endl;
	cout << "Name: " << this->name << endl;
	if (gender == MALE)
	{
		cout << "Gender: Male" << endl;
	}
	else
	{
		cout << "Gender: Female" << endl;
	}
	cout << "Day of Birth: " << this->DoB << endl;
	cout << "class: " << this->ClassID << endl;
}

void Student::showEnrolledCourse()
{
	CourseID->current = CourseID->head;
	while (CourseID->current != 0)
	{
		node<Course>* tmp = CourseList->head;
		while (tmp != 0)
		{
			if (tmp->data->ID == *(CourseID->current->data))
			{
				tmp->data->viewInfo();
			}
			tmp = tmp->next;
		}
		CourseID->current = CourseID->current->next;
	}
}

void Student::showAllCourse()
{
	node<Course>* tmp = CourseList->head;
	while (tmp != 0)
	{
		tmp->data->viewInfo();
		tmp = tmp->next;
	}
}

void Student::showSchedue()
{
	int cellSize = 30;
	int TimeCellSize =20;
	cout <<endl <<"|"<< left << setw(cellSize) <<  ioHelper::centered("Course Name",cellSize) << "|"
		 << left << setw(TimeCellSize) << ioHelper::centered("Mon", TimeCellSize) << "|"
		 <<left << setw(TimeCellSize) << ioHelper::centered("Tue", TimeCellSize) << "|"
		<< left << setw(TimeCellSize) << ioHelper::centered("Wed", TimeCellSize) << "|"
		<< left << setw(TimeCellSize) << ioHelper::centered("Thu", TimeCellSize) << "|"
		<< left << setw(TimeCellSize) << ioHelper::centered("Fri", TimeCellSize) << "|"
		<< left << setw(TimeCellSize) << ioHelper::centered("Sat", TimeCellSize) << "|"
		<< left << setw(TimeCellSize) << ioHelper::centered("Sun", TimeCellSize) << "|";
	node<Course>* currentCourse = CourseList->head;
	while (currentCourse)
	{
		if (isEnrolled(currentCourse->data->ID))
		{
			cout << endl << "|" << left << setw(cellSize) << currentCourse->data->courseName << "|";
			for (int i = Mon; i <= Sun; i++)
			{
				if (i == currentCourse->data->dayOfweek)
				{

					string time = to_string(currentCourse->data->startTime.hour) + ':' + to_string(currentCourse->data->startTime.minute) + " - "
						+ to_string(currentCourse->data->endTime.hour) + ':' + to_string(currentCourse->data->endTime.minute);
					ioHelper::whiteLine();
					cout << left << setw(TimeCellSize) << ioHelper::centered(time, TimeCellSize);
					ioHelper::blackLine();
					cout << "|";
				}
				else
				{
					cout << setw(TimeCellSize) << " " << "|";
				}
			}
		}
		currentCourse = currentCourse->next;
	}
	ioHelper::blackLine();
}

void Student::ShowScoreBoard()
{
	int cellSize = 30;
	int TimeCellSize = 10;
	cout << endl << "|" << left << setw(cellSize) << ioHelper::centered("Course Name", cellSize) << "|"
		<< left << setw(TimeCellSize) << ioHelper::centered("MidTerm", TimeCellSize) << "|"
		<< left << setw(TimeCellSize) << ioHelper::centered("Final", TimeCellSize) << "|"
		<< left << setw(TimeCellSize) << ioHelper::centered("Lab", TimeCellSize) << "|"
		<< left << setw(TimeCellSize) << ioHelper::centered("Bonus", TimeCellSize) << "|";
	node<Course>* currentCourse = CourseList->head;
	bool Highlighted = true;
	while (currentCourse)
	{
		if (isEnrolled(currentCourse->data->ID))
		{
			SCOREBOARD scoreboard(currentCourse->data->ID);
			ScoreRecord rec = scoreboard.getByID(this->ID);
			
			cout << endl << "|" << left << setw(cellSize) << currentCourse->data->courseName << "|"
				<< left << setw(TimeCellSize);
			rec.midTerm > 5 ? ioHelper::textGreen() : rec.midTerm == 5 ? ioHelper::textYellow() : ioHelper::textRed();
			cout << ioHelper::centered(to_string(rec.midTerm).substr(0, 3), TimeCellSize);
			ioHelper::blackLine();
			cout << "|" << left << setw(TimeCellSize);

			rec.final > 5 ? ioHelper::textGreen() : rec.final == 5 ? ioHelper::textYellow() : ioHelper::textRed();
			cout << ioHelper::centered(to_string(rec.final).substr(0, 3), TimeCellSize);
			ioHelper::blackLine();
			cout << "|" << left << setw(TimeCellSize);


			rec.lab > 5 ? ioHelper::textGreen() : rec.lab == 5 ? ioHelper::textYellow() : ioHelper::textRed();
			cout << ioHelper::centered(to_string(rec.lab).substr(0, 3), TimeCellSize);
			ioHelper::blackLine();
			cout << "|" << left << setw(TimeCellSize);

			cout << left << setw(TimeCellSize) << ioHelper::centered(to_string(rec.bonus).substr(0, 3), TimeCellSize) << "|";
		}
		currentCourse = currentCourse->next;
	}
}

void Student::checkin()
{
	///Sumary
	/*
	1.Get today course
	2.Get course want to checkin
	3.check if today Checkin is created
		-if created check if selected course is checked
			-if not checked check -else print you already check (valid check)
		-if not created check create check and check selected course (valid check)	
	
	
	
	*/
	///
	//Get today Course
	tm now = Checkin::getCurrentTime();
	now.tm_wday -= 1;
	if (now.tm_wday < 0)
	{
		now.tm_wday += 7;
	}
	vector<Course*> todayCourse;
	CourseList->resetCurrent();
	do
	{
		if(isEnrolled(CourseList->current->data->ID) && CourseList->current->data->dayOfweek == now.tm_wday)
		{ 
			todayCourse.push_back(CourseList->current->data);
		}
	} while (CourseList->next());
	CourseList->resetCurrent();

	//Load check list
	fstream checkinFile;
	checkinFile.open("Data\\checkin\\" + this->ID+".txt", ios::in);
	Date lastDate;
	string buffer;
	vector<CheckinResult> lastCheckin;
	if (checkinFile.is_open())
	{
		while (checkinFile >> buffer)
		{
			lastDate = StringToDate(buffer);

			if (!(checkinFile >> buffer))
			{
				buffer = "";
			}
		}
		size_t commaPos = 0;
		while (commaPos != string::npos && buffer != "")
		{
			commaPos = buffer.find(',');
			string result = buffer.substr(0, commaPos);
			if (commaPos != string::npos)
			{
				buffer.erase(0,buffer.find(',')+1);
			}

			string CourseID =  result.substr(0, result.find("="));
			int state = stoi(result.substr(result.find("=") + 1, 1));
			CheckinResult tmp = { CourseID,state };
			lastCheckin.push_back(tmp);
		}
		checkinFile.close();
		//check if lastDate is today
		if (lastDate.day == now.tm_mday && lastDate.month - 1 == now.tm_mon && lastDate.year-1900 == now.tm_year)
		{
			cout << "This is today course: " << endl;
			//show today course
			for (int i = 0; i < todayCourse.size(); i++)
			{
				
				cout << "Course ID: " << todayCourse[i]->ID << endl;
				cout << "Course Name: " << todayCourse[i]->courseName << endl;
				cout << "Time: ";
				if (todayCourse[i]->startTime.hour < 10) cout << "0";
				cout << todayCourse[i]->startTime.hour << ":";
				if (todayCourse[i]->startTime.minute <10)cout <<"0";
				cout << todayCourse[i]->startTime.minute << " - ";
				if (todayCourse[i]->endTime.hour < 10) cout << "0";
				cout << todayCourse[i]->endTime.hour << ":";
				if (todayCourse[i]->endTime.minute < 10)cout << "0";
				cout << todayCourse[i]->endTime.minute << endl;
				cout << "---------------------------------------------" << endl;
			}
			cout << "Select course to check-in: ";
			string selectedCourse;
			cin >> selectedCourse;
			now = Checkin::getCurrentTime();
			bool validCourse = false;
			//check if selected course is in today course list
			Course *validCoursePtr = 0;
			for (int courseIndex = 0; courseIndex < todayCourse.size(); courseIndex++)
			{
				if (selectedCourse == todayCourse[courseIndex]->ID)
				{
					validCoursePtr = todayCourse[courseIndex];
					validCourse = true;
					break;
				}
			}
			//check if selected course was checked before
			for (int i = 0; i < lastCheckin.size(); i++)
			{
				if (selectedCourse == lastCheckin[i].CourseID)
				{
					validCourse = false;
					cout << "you have checked in this course already" << endl;
					break;
				}
				validCourse = true;
			}
			//check if course meet 2 requirements above
			if (validCourse)
			{
				
				//checkin status 
				//0 = absent; 
				//1 = eary,in-time;
				//2 = late;
				CheckinResult tmp = {validCoursePtr->ID,1};
				if (now.tm_hour >= validCoursePtr->endTime.hour && now.tm_min >= validCoursePtr->endTime.minute)
				{
					tmp.result = 0;
				}
				else
				{
					if (now.tm_hour >= validCoursePtr->startTime.hour && now.tm_min > validCoursePtr->startTime.minute)
					{
						tmp.result = 2;
					}
					else
					{
						tmp.result = 1;
					}
				}
				checkinFile.open("Data\\checkin\\" + this->ID + ".txt", ios::app);
				if (lastCheckin.size() == 0)
				{
					checkinFile << " " << tmp.CourseID << "=" << tmp.result;
				}
				else
				{
					checkinFile << "," << tmp.CourseID << "=" << tmp.result;
				}
				checkinFile.close();
				
			}
		}
		else
		{
			checkinFile.open("Data\\checkin\\" + this->ID + ".txt", ios::app);
			checkinFile <<endl<< lastDate.day << "-" << lastDate.month << "-" << lastDate.year << " ";
			checkinFile.close();
		}
	}
	else
	{
		cout << "Can't find checkin file";
	}
}

void Student::showCheckin()
{
}

Student::Student()
{
	this->role = STUDENT;
	this->CourseID = new linkedList<string>;
}

void Student::showMenu()
{
	int choice = 0;
	while (choice != -1)
	{
		system("cls");
		cout << "View My Course" << endl;
		cout << "View All Course" << endl;
		cout << "View All info" << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:
			showEnrolledCourse();
			cin.ignore();
			cin.get();
			break;
		case 2:
			showAllCourse();
			cin.ignore();
			cin.get();
			break;
		case 3:
			ViewInfo();
			cin.ignore();
			cin.get();
			break;
		case 4:
			showSchedue();
			cin.ignore();
			cin.get();
			break;
		case 5:
			ShowScoreBoard();
			cin.ignore();
			cin.get();
			break;
		case 6:
			checkin();
			cin.ignore();
			cin.get();
			break;
		default:
			return;
		}
	}
	//ViewInfo();
}

int Student::init(string dataString)
{
	int commaPos = dataString.find(',');
	this->ID = dataString.substr(0, commaPos);
	dataString.erase(0, commaPos + 1);


	commaPos = dataString.find(',');
	this->password = dataString.substr(0, commaPos);
	dataString.erase(0, commaPos + 1);

	commaPos = dataString.find(',');
	this->name = dataString.substr(0, commaPos);
	dataString.erase(0, commaPos + 1);

	commaPos = dataString.find(',');
	this->gender = strToInt(dataString.substr(0, commaPos));
	dataString.erase(0, commaPos + 1);

	commaPos = dataString.find(',');
	this->DoB = dataString.substr(0, commaPos);
	dataString.erase(0, commaPos + 1);
	
	commaPos = dataString.find(',');
	this->ClassID = dataString.substr(0, commaPos);
	dataString.erase(0, commaPos + 1);

	if (this->ClassID != dataString)
	{
		while (dataString.find(',') != string::npos)
		{
			node<string>* CourseIdNode = new node <string>;
			CourseIdNode->data = new string;
			commaPos = dataString.find(',');
			*(CourseIdNode->data) = dataString.substr(0, commaPos);
			dataString.erase(0, commaPos + 1);
			this->CourseID->insert(CourseIdNode);
		}
		node<string>* CourseIdNode = new node <string>;
		CourseIdNode->data = new string;
		*(CourseIdNode->data) = dataString;
		this->CourseID->insert(CourseIdNode);
	}
	return 0;
}

void Student::addCourse(string CourseID)
{
	node<string>* courseIdCurrent = this->CourseID->head;
	while (courseIdCurrent != 0)
	{
		if (CourseID == *(courseIdCurrent->data))
		{
			return;
		}
		courseIdCurrent = courseIdCurrent->next;
	}
	node<string>* CourseIdNode = new node <string>;
	CourseIdNode->data = new string;
	*(CourseIdNode->data) = CourseID;
	this->CourseID->insertTop(CourseIdNode);
}
void Student::setCourseList(linkedList<Course>* courseList)
{
	this->CourseList = courseList;
}

//Thay đổi tên
void Student::setName(string newName)
{
	name = newName;
}

void Student::setDoB(string newDoB)
{
	DoB = newDoB;
}



string Student::parse()
{
	string parseResult = "1 " + this->ID + ',' + this->password + ',' + this->name + ',' + char(this->gender + 48) + ',' + this->DoB+','+this->ClassID;
	node<string>* courseIdCurrent = this->CourseID->head;
	while (courseIdCurrent != 0)
	{
		parseResult += ',' + *(courseIdCurrent->data);
		courseIdCurrent = courseIdCurrent->next;
	}
	return parseResult;
}

void Student::ChangeClass(string ClassID) {
	this->ClassID = ClassID;
}

string Student::getClassID() {
	return this->ClassID;
}
bool Student::isEnrolled(string CourseID)
{
	node<string>* current = this->CourseID->head;
	while (current)
	{
		if (*(current->data) == CourseID)
		{
			return true;
		}
		current = current->next;
	}
	return false;
}

void Student::removeCourse(string courseName)
{
	if (CourseID->head != NULL && *(CourseID->head->data) == courseName)
	{
		if (CourseID->head->next == NULL)
		{
			delete CourseID->head;
			CourseID->head = NULL;
		}
		else
		{
			node<string>* tmp = CourseID->head->next;
			delete CourseID->head;
			CourseID->head = tmp;
		}
	}
	else
	{
		node<string>* currentCourse = CourseID->head;
		while (currentCourse->next != NULL)
		{
			if (*(currentCourse->next->data) == courseName)
			{
				node<string>* tmp = currentCourse->next;
				currentCourse->next = tmp->next;
				delete tmp;
			}
			else
			{
				currentCourse = currentCourse->next;
			}
		}
	}
}

Student::~Student()
{
	delete CourseID;
}

