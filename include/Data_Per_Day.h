#ifndef Data_Per_Day_h
#define Data_Per_Day_h

// Data stored per day class
class Data_Per_Day {
 public :

  // Constructor and Destructor
  Data_Per_Day(int iday, int imonth, int year);
  ~Data_Per_Day();

  // Information Stored Per Day
  int day;
  int month;
  int year;

  int cases_confirmed;
  int cases_deaths;
  int cases_recovered;
  
};

#endif

