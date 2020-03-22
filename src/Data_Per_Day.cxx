#include "Data_Per_Day.h"

//Constructor: Initiate Vectors
Data_Per_Day::Data_Per_Day(int iday, int imonth, int iyear) {

  day   = iday;
  month = imonth;
  year  = iyear;

}

//Destructor: Delete all pointers
Data_Per_Day::~Data_Per_Day(){
}
