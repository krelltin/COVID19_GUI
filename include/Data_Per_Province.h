
#ifndef Data_Per_Province_h
#define Data_Per_Province_h

#include "Data_Per_Day.h"

#include <vector>
#include <iostream>
#include <sstream>

#include <TString.h>

#include <TH1F.h>

class Data_Per_Province {
 public :

  // Constructor and Destructor
  Data_Per_Province(TString name);
  ~Data_Per_Province();

  bool verbose;
  
  TString name_province;

  float longitude;
  float latitude;

  TH1F * h_cumulative_confirmed;
  TH1F * h_cumulative_deaths;
  TH1F * h_cumulative_recovered;
  TH1F * h_new_cases_per_day_confirmed;
  TH1F * h_new_cases_per_day_deaths;
  TH1F * h_new_cases_per_day_recovered;

  TH1F * h_cumulative_confirmed_shifted;
  TH1F * h_cumulative_deaths_shifted;
  TH1F * h_cumulative_recovered_shifted;
  
  TH1F * h_new_cases_per_day_confirmed_shifted;
  TH1F * h_new_cases_per_day_deaths_shifted;
  TH1F * h_new_cases_per_day_recovered_shifted;
  
  // Methods for adding information
  void           Add_Cumulative_Data_for_Day ( int day, int month, int year,
					       int cumulative_cases_confirmed,
					       int cumulative_cases_deaths,
					       int cumulative_cases_recovered);
  void           Add_Cumulative_Data_for_Day_Confirmed ( int day, int month, int year,
							 int cumulative_cases_confirmed );
  void           Add_Cumulative_Data_for_Day_Deaths (      int day, int month, int year,
							 int cumulative_cases_deaths );
  void           Add_Cumulative_Data_for_Day_Recovered ( int day, int month, int year,
							 int cumulative_cases_recovered );
  
  Data_Per_Day * Find_Data_For_Day( int day, int month, int year,
				    std::vector<Data_Per_Day*> * Data_Vector);
  
  // Method for getting information for a day
  Data_Per_Day * Get_Cumulative_Data_for_Day( int day, int month, int year);
  Data_Per_Day * Get_New_Cases_Data_for_Day ( int day, int month, int year);

  // Method for getting information for ith day
  Data_Per_Day * Get_Cumulative_Data_for_Day( uint iday );
  Data_Per_Day * Get_New_Cases_Data_for_Day ( uint iday );
  
  void Calculate_New_Cases_Per_Day();

  bool CombineCumulative( Data_Per_Province * province );

  void GenerateHistograms       (TString CountryName);
  void GenerateHistogramsShifted(TString CountryName, int n_shifted_days, int n_plus_days);

  void Clear();

  void setVerbose( bool verb );
  
  std::vector<Data_Per_Day*> * Data_Cumulative;
  std::vector<Data_Per_Day*> * Data_New_Cases_Per_Day;
  
};

#endif

