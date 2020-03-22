#include "Data_Per_Country.h"

#include <iostream>

//Constructor: Initiate Vectors
Data_Per_Country::Data_Per_Country(TString name) {

  name_country = name;

  verbose = false;
  
  Data_Provinces = new std::map<TString, Data_Per_Province*> ;
  Data_Provinces ->clear();
  Data_Total_All_Provinces = new Data_Per_Province("Total");

}

//Destructor: Delete all pointers
Data_Per_Country::~Data_Per_Country(){
  delete Data_Provinces;
  delete Data_Total_All_Provinces;
}

void Data_Per_Country::Add_Province( TString province_name ) {

  // if you don't find the province in the map then add it
  if ( Data_Provinces->find(province_name) == Data_Provinces->end() ) {

    if( verbose ) std::cout << "INFO: New province " << name_country
			    << ", " << province_name << "\n";
                                                                            
    Data_Per_Province * tmp_province_data = new Data_Per_Province(province_name);
    //Data_Provinces->insert( std::pair<TString, Data_Per_Province*>(province_name, tmp_province_data) );
    Data_Provinces->emplace( province_name, tmp_province_data );
    Data_Provinces->at( province_name )->setVerbose( verbose );
  }                                                                    
  
}

Data_Per_Province * Data_Per_Country::Province( TString province_name ) {

  if ( Data_Provinces->find(province_name) == Data_Provinces->end() ) {
    std::cout << "Error: Province Name does not exist" << std::endl;
    Add_Province( province_name );
  }
  
  return Data_Provinces->at(province_name);
  
}

void Data_Per_Country::GeneratePerCountryHistograms() {

  Data_Total_All_Provinces->GenerateHistograms(name_country);

  std::map<TString, Data_Per_Province*>::iterator itr;
  for (itr =  Data_Provinces->begin(); itr !=  Data_Provinces->end(); ++itr) {

    if (verbose) std::cout  << "Generating Histogram for " << name_country
			    << ", " << itr->second->name_province.Data() << '\n';

    itr->second->GenerateHistograms(name_country);
  }

  return;
  
}

void Data_Per_Country::GeneratePerCountryTotalHistogramsShifted(int n_shifted_days, int n_plus_days) {

  if (verbose) std::cout  << "Generating Histogram for " << name_country
			  << ", " << Data_Total_All_Provinces->name_province.Data() << '\n';
  
  Data_Total_All_Provinces->GenerateHistogramsShifted(name_country, n_shifted_days, n_plus_days);

  return;

}

void Data_Per_Country::GeneratePerCountryHistogramsShifted(int n_shifted_days, int n_plus_days) {

  std::map<TString, Data_Per_Province*>::iterator itr;
  for (itr =  Data_Provinces->begin(); itr !=  Data_Provinces->end(); ++itr) {

    if (verbose) std::cout  << "Generating Histogram for " << name_country
			    << ", " << itr->second->name_province.Data() << '\n';

    itr->second->GenerateHistogramsShifted(name_country, n_shifted_days, n_plus_days);
  }

  return;

}

void Data_Per_Country::Calculate_New_Cases_Per_Day_For_All_Provinces() {

  Data_Total_All_Provinces->Calculate_New_Cases_Per_Day();
  
  std::map<TString, Data_Per_Province*>::iterator itr;
  for (itr =  Data_Provinces->begin(); itr !=  Data_Provinces->end(); ++itr) {
    
    if (verbose) std::cout  << "Calculating Cases Per Day for " << name_country
			    << ", " << itr->second->name_province.Data() << '\n';

    itr->second->Calculate_New_Cases_Per_Day();
  }

  return;
}

bool Data_Per_Country::Calculate_Cumulative_Total_For_All_Provinces() {

  Data_Total_All_Provinces->Clear();

  bool success = true;
  
  std::map<TString, Data_Per_Province*>::iterator itr;
  for (itr =  Data_Provinces->begin(); itr !=  Data_Provinces->end(); ++itr) {
    if (verbose) std::cout  << "Combining all data " << name_country
			    << ", " << itr->second->name_province.Data() << '\n';

    success = Data_Total_All_Provinces->CombineCumulative( itr->second );

    if ( !success ) {
      Data_Total_All_Provinces->Clear();
      return success;
    }
    
  }

  return success;  
  
}

void Data_Per_Country::setVerbose( bool verb ) {

  verbose = verb;
  Data_Total_All_Provinces->setVerbose( verb );

  std::map<TString, Data_Per_Province*>::iterator itr;
  for (itr =  Data_Provinces->begin(); itr !=  Data_Provinces->end(); ++itr) {
    itr->second->setVerbose( verb );
  }
  
}
