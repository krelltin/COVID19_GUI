
#ifndef Data_Analyzer_h
#define Data_Analyzer_h

#include "Data_Per_Country.h"

#include <map>
#include <TString.h>

#include <iostream>
#include <fstream>
#include <sstream>

class Data_Analyzer {
 public :

  // Constructor and Destructor
  Data_Analyzer();
  ~Data_Analyzer();

  bool verbose;
  
  std::map<TString, Data_Per_Country*> * Data_Countries;

  void DecodeDate( std::string date_str, int & day, int & month, int & year );
  
  void SaveData(TString CountryName, TString ProvinceName,
		std::vector<std::string> date_vector,
		std::vector<int>         rate,
		int                      rate_type);

  void Read_Data(TString input, int cases_type);

  void CalculateNewCasesPerDay();
  void CalculateCumulativeTotalForAllProvinces();

  void GeneratePerCountryHistograms();
  void GeneratePerCountryHistogramsShifted(int n_shifted_days, int n_plus_days);
  
  void setVerbose( bool verb );
  
};

#endif

