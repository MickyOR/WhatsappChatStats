#include <iostream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

int numberDaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int numberDaysInMonthLeap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

string weekDayName[7] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
string monthName[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

bool isLeapYear(int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int monthToDay(int month, int year)
{
    int daysBefore = 0;
    for (int i = 0; i < month; i++) 
    	daysBefore += numberDaysInMonth[i];
    if (month > 1 && isLeapYear(year)) 
    	daysBefore++;
    return daysBefore;
}

int yearToDay(int year)
{
    int base = year * 365;
    int numLeapYears = year / 4 - year / 100 + year / 400;
    return base + numLeapYears;
}


int getYearFromNumDays(int &numDays)
{
    int year = 1;
    int sizeOfYear = 365;
    
    while (numDays > sizeOfYear)
    {
        numDays -= sizeOfYear;
        year++;
        sizeOfYear = ((isLeapYear(year)) ? 366 : 365);
    }
    
    return year;
}

int getMonthFromNumDays(int& numDays, int year)
{
    int month = 0;
    int * numDayUsed = (isLeapYear(year)) ? numberDaysInMonthLeap : numberDaysInMonth;
    for (;numDays > numDayUsed[month]; month++)
        numDays -= numDayUsed[month];
    return month + 1;
}


struct date
{
	int day, month, year;

	date() {}
	date (int d, int m, int y)
	{
		day = d;
		month = m;
		year = y;
	}

	string getDate()
	{
		string dateString = "";
		dateString += (char)(day/10 + '0');
		dateString += (char)(day%10 + '0');
		dateString += '.';

		dateString += (char)(month/10 + '0');
		dateString += (char)(month%10 + '0');
		dateString += '.';

		dateString += "20";
		dateString += (char)(year%100/10 + '0');
		dateString += (char)(year%10 + '0');
		return dateString;
	}

	void aumentarUnDia()
	{
		int totDays = day;
		totDays += monthToDay(month-1, year);
		totDays += yearToDay(year-1);
		totDays++;
		year = getYearFromNumDays(totDays);
		month = getMonthFromNumDays(totDays, year);
		day = totDays;
	}

	int getWeekDay()
	{
		int totDays = day;
		totDays += monthToDay(month-1, year-1);
		totDays += yearToDay(year-1);
		return totDays%7;
	}

	string getMonthName()
	{
		return monthName[month-1];
	}
};

bool operator<(date a, date b)
{
	return make_pair(a.year, make_pair(a.month, a.day)) < make_pair(b.year, make_pair(b.month, b.day));
}

bool operator>(date a, date b)
{
	return make_pair(a.year, make_pair(a.month, a.day)) > make_pair(b.year, make_pair(b.month, b.day));
}


bool operator==(date a, date b)
{
	return make_pair(a.year, make_pair(a.month, a.day)) == make_pair(b.year, make_pair(b.month, b.day));
}

bool operator<=(date a, date b)
{
	return make_pair(a.year, make_pair(a.month, a.day)) <= make_pair(b.year, make_pair(b.month, b.day));
}

int differenceInDays(date a, date b)
{
	int totDaysA = a.day;
		totDaysA += monthToDay(a.month-1, a.year-1);
		totDaysA += yearToDay(a.year-1);
	int totDaysB = b.day;
		totDaysB += monthToDay(b.month-1, b.year-1);
		totDaysB += yearToDay(b.year-1);
	return abs(totDaysB - totDaysA);
}


bool isValidDate(date d)
{
	if (d.day == 29 && d.month == 1 && d.year%4 == 0) return true;
	if (d.day < 1 || d.day > numberDaysInMonth[d.month-1]) return false;
	if (d.month < 1 || d.month > 12) return false;
	if (d.year < 0) return false;
	return true;
}

date parseDate(string dateStr)
{
	stringstream ss;
	ss << dateStr;
	int d, m, y;
	char aux;
	ss >> d >> aux >> m >> aux >> y;
	return date(d, m, y);
}

struct time
{
	int minute, hour;
	
	time() {}
	time(int h, int m)
	{
		hour = h;
		minute = m;
	}

	string getTime()
	{
		string timeString = "";
		timeString += (char)(hour/10 + '0');
		timeString += (char)(hour%10 + '0');
		timeString += ':';
		timeString += (char)(minute/10 + '0');
		timeString += (char)(minute%10 + '0');
		return timeString;
	}
};

bool operator<(time a, time b)
{
	return make_pair(a.hour, a.minute) < make_pair(b.hour, b.minute);
}

bool operator>(time a, time b)
{
	return make_pair(a.hour, a.minute) > make_pair(b.hour, b.minute);
}

bool operator==(time a, time b)
{
	return make_pair(a.hour, a.minute) == make_pair(b.hour, b.minute);
}

bool operator<=(time a, time b)
{
	return make_pair(a.hour, a.minute) <= make_pair(b.hour, b.minute);
}

time parseTime(string timeStr)
{
	stringstream ss;
	ss << timeStr;
	int m, h;
	char meridiem;
	char aux;
	ss >> h >> aux >> m >> meridiem >> aux;
	h %= 12;
	if (meridiem == 'p') h += 12;
	return time(h, m);
}

struct parsedText
{
	date textDate;
	time textTime;
	string author;
	vector<string> words;

	parsedText() {}

	void printContent()
	{
		cout << "====   ====   ====   ====  ====" << endl;
		cout << "Date: " << textDate.getDate() << endl;
		cout << "Time: " << textTime.getTime() << endl;;
		cout << "Author: " << author << endl;
		cout << "Message content:" << endl;
		for (int i = 0; i < words.size(); i++) cout << "> " << words[i] << endl;
	}
};

vector<string> getWords(string txt)
{
	for (int i = 0; i < txt.size(); i++) 
		if ('A' <= txt[i] && txt[i] <= 'Z') txt[i] = tolower(txt[i]);
	string word = "";
	vector<string> res;
	txt += " ";
	for (int i = 0; i < txt.size(); i++)
	{
		if (txt[i] == ' ')
		{
			if (word != "") res.push_back(word);
			word = "";
			continue;
		} 
		word += txt[i];
	}
	return res;
}


parsedText parseMessage(string txt)
{
	parsedText message;

	int pos = 0;
	while (txt[pos] != ',') pos++;
	message.textDate = parseDate(txt.substr(0, pos));
	txt = txt.substr(pos+2);

	pos = 0;
	while (txt[pos] != '-') pos++;
	message.textTime = parseTime(txt.substr(0, pos-1));
	txt = txt.substr(pos+2);

	
	pos = 0;
	while (txt[pos] != ' ') pos++;
	message.author = txt.substr(0, pos);
	txt = txt.substr(pos+1);
	
	message.words = getWords(txt);

	return message;
}

int main()
{
    freopen("ChatWhatsappLimpio.txt","r",stdin);
    //freopen("","w",stdout);

    string txt;
    vector<parsedText> messages;
    map<date, int> messagesInDate, myMessagesInDate, herMessagesInDate;
    map<int, int> myMessagesByMonth, myMessagesByWeekDay, myMessagesByHour;
    map<int, int> herMessagesByMonth, herMessagesByWeekDay, herMessagesByHour;
    while ( getline(cin,txt) )
    {
    	txt = txt.substr(3);
    	parsedText message = parseMessage(txt);
    	if (message.words[0] != "<archivo") messages.push_back(message);
    	messagesInDate[message.textDate]++;
    	
    	// POR FECHA
    	if (message.author == "Miguel") myMessagesInDate[message.textDate]++;
    	else herMessagesInDate[message.textDate]++;

    	// POR MES
    	if (message.author == "Miguel") myMessagesByMonth[message.textDate.month]++;
    	else herMessagesByMonth[message.textDate.month]++;
    	
    	// POR DIA DE SEMANA
    	if (message.author == "Miguel") myMessagesByWeekDay[message.textDate.getWeekDay()]++;
    	else herMessagesByWeekDay[message.textDate.getWeekDay()]++;
    	
    	// POR HORA
    	if (message.author == "Miguel") myMessagesByHour[message.textTime.hour]++;
    	else herMessagesByHour[message.textTime.hour]++;
    }

    date startDate = messages[0].textDate;
    int totalDays = differenceInDays(messages[0].textDate, messages[ messages.size()-1 ].textDate);
    int totalMessages = messages.size();
    int totalWords = 0;
    for (int  i = 0; i < messages.size(); i++) totalWords += messages[i].words.size();

    cout << "Inicio: " << startDate.getDate() << endl;
	
	cout << "Dias: " << totalDays << endl;

	cout << "Mensajes: " << totalMessages << endl;

	cout << "Palabras: " << totalWords << endl;

    date mostActiveDay = messages[0].textDate;
	int messagesInMostActiveDay = messagesInDate[mostActiveDay];
	for (int i = 0; i < messages.size(); i++)
	{
		if (messagesInMostActiveDay <= messagesInDate[messages[i].textDate])
		{
			messagesInMostActiveDay = messagesInDate[messages[i].textDate];
			mostActiveDay = messages[i].textDate;
		}
	}
	cout << "Dia mas activo: " << mostActiveDay.getDate() << endl;
	cout << "Mensajes en dia mas activo: " << messagesInMostActiveDay << endl;

	vector<string> frase;
	
	frase.clear(); frase.push_back("te"); frase.push_back("amo");
	int teAmo = 0;
	for (int i = 0; i < messages.size(); i++)
	{
		parsedText message = messages[i];
		for (int j = 0; j < message.words.size()-frase.size()+1; j++)
		{
			bool found = true;
			for (int k = 0; k < frase.size(); k++)
			{
				if (message.words[j+k] != frase[k]) found = false;
			}
			if (found) teAmo++;
		}
	}
	cout << "Te amo: " << teAmo << endl;

	frase.clear(); frase.push_back("te"); frase.push_back("extraño");
	int teExtrano = 0;
	for (int i = 0; i < messages.size(); i++)
	{
		parsedText message = messages[i];
		for (int j = 0; j < message.words.size()-frase.size()+1; j++)
		{
			bool found = true;
			for (int k = 0; k < frase.size(); k++)
			{
				if (message.words[j+k] != frase[k]) found = false;
			}
			if (found) teExtrano++;
		}
	}
	cout << "Te extraño: " << teExtrano << endl;


	ofstream messagesSendOFSTREAM      ("numberOfMessagesSend.txt", ofstream::out);
    ofstream messagesOverTimeOFSTREAM  ("messagesOverTime.txt",     ofstream::out);
    ofstream messagesByMonthOFSTREAM   ("messagesByMonth.txt",      ofstream::out);
    ofstream messagesByWeekDayOFSTREAM ("messagesByWeekDay.txt",    ofstream::out);
    ofstream messagesByHourOFSTREAM    ("messagesByHour.txt",       ofstream::out);
    ofstream mostFrequentWordsOFSTREAM ("mostFrequentWords.txt",    ofstream::out);

    // MENSAJES ENVIADOS POR CADA UNO
    int messagesMariana = 0, messagesMiguel = 0;
    for (int i = 0; i < messages.size(); i++) if (messages[i].author == "Mariana") messagesMariana++;
	for (int i = 0; i < messages.size(); i++) if (messages[i].author == "Miguel" ) messagesMiguel++;
	cout << "Mensajes de Mariana: " << messagesMariana << endl;
	cout << "Mensajes de Miguel: " << messagesMiguel << endl;

	// MENSAJES ENVIADOS CADA DIA (FORMATO PARA EXCEL)
	date actualDate = startDate;
	messagesOverTimeOFSTREAM << "	" << "Mariana" << "	" << "Miguel" << endl;
	while (actualDate <= messages[messages.size()-1].textDate)
	{
		messagesOverTimeOFSTREAM << actualDate.getDate() << "	" << herMessagesInDate[actualDate] << "	" << myMessagesInDate[actualDate] << endl;
		actualDate.aumentarUnDia();
	}

	// MENSAJES ENVIADOS POR MES
	messagesByMonthOFSTREAM << "	" << "Mariana" << "	" << "Miguel" << endl;
	for (int i = 0; i < 12; i++)
	{
		messagesByMonthOFSTREAM << monthName[i] << "	" << herMessagesByMonth[i+1] << "	" << myMessagesByMonth[i+1] << endl;
	}

	// MENSAJES ENVIADOS POR DIA DE SEMANA
	messagesByWeekDayOFSTREAM << "	" << "Mariana" << "	" << "Miguel" << endl;
	for (int i = 0; i < 7; i++)
	{
		messagesByWeekDayOFSTREAM << weekDayName[(i+1)%7] << "	" << herMessagesByWeekDay[(i+1)%7] << "	" << myMessagesByWeekDay[(i+1)%7] << endl;
	}

	// MENSAJES ENVIADOS POR HORA
	messagesByHourOFSTREAM << "	" << "Mariana" << "	" << "Miguel" << endl;
	for (int i = 0; i < 24; i++)
	{
		messagesByHourOFSTREAM << i << "	" << herMessagesByHour[i] << "	" << myMessagesByHour[i] << endl;
	}

	// 15 PALABRAS MAS USADAS
	map<string, int> wordFrequency;
	vector< pair<int, string> > wordFreqVector;
	for (int i = 0; i < messages.size(); i++)
	{
		for (int j = 0; j < messages[i].words.size(); j++)
		{
			wordFrequency[ messages[i].words[j] ]++;
		}
	}
	for (map<string, int>::iterator it = wordFrequency.begin(); it != wordFrequency.end(); it++)
	{
		wordFreqVector.push_back( make_pair(it->second, it->first) );
	}
	sort(wordFreqVector.rbegin(), wordFreqVector.rend());
	for (int i = 0; i < min(15, (int)wordFreqVector.size()); i++) 
		mostFrequentWordsOFSTREAM << wordFreqVector[i].second << "	" << wordFreqVector[i].first << endl;

    return 0;
}
