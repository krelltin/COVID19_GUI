
#ifndef Data_Per_Country_h
#define Data_Per_Country_h

#include "Data_Per_Province.h"

#include <map>
#include <TString.h>

class Data_Per_Country {
 public :

  // Constructor and Destructor
  Data_Per_Country(TString name);
  ~Data_Per_Country();

  bool verbose;
  
  TString name_country;
  
  std::map<TString, Data_Per_Province*> * Data_Provinces;

  Data_Per_Province * Data_Total_All_Provinces;

  void Add_Province( TString province_name );

  void Calculate_New_Cases_Per_Day_For_All_Provinces();
  bool Calculate_Cumulative_Total_For_All_Provinces();

  void GeneratePerCountryHistograms();
  void GeneratePerCountryTotalHistogramsShifted(int n_shifted_days, int n_plus_days);
  void GeneratePerCountryHistogramsShifted(int n_shifted_days, int n_plus_days);

  Data_Per_Province * Province( TString province_name );

  void setVerbose( bool verb );
  
};

#endif

