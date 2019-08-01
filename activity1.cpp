/* ACTIVITY 1:
* Write a software that show how many seconds a person have lived based on his birth date.
*/
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main()
{
  time_t current_time;
  struct tm birth_date = {0};
  double seconds;
  int year, month, day, hour, min;
  cout << "Birth year: ";
  cin >> year;
  cout << "Birth month: ";
  cin >> month;
  cout << "Birth day: ";
  cin >> day;
  cout << "Birth hour: ";
  cin >> hour;
  cout << "Birth minute: ";
  cin >> min;

  birth_date.tm_sec = 0;
  birth_date.tm_hour = hour;
  birth_date.tm_min = min;
  birth_date.tm_year = year - 1900;
  birth_date.tm_mon = month - 1;
  birth_date.tm_mday = day;

  time(&current_time);

  seconds = difftime(current_time, mktime(&birth_date));
  cout << endl
       << "You have lived: " << endl;
  cout << seconds << " seconds," << endl;
  cout << seconds / 60.0 << " minutes," << endl;
  cout << seconds / 3600.0 << " hours and" << endl;
  cout << seconds / 86400.0 << " days so far." << endl;

  exit(0);
}