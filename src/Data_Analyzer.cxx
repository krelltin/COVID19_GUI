#include "Data_Analyzer.h"
#include "types.h"

//Constructor: Initiate Vectors
Data_Analyzer::Data_Analyzer() {
  Data_Countries = new std::map<TString, Data_Per_Country*>;
  verbose = false;
}

//Destructor: Delete all pointers
Data_Analyzer::~Data_Analyzer(){
  delete Data_Countries;
}

void Data_Analyzer::SaveData(TString CountryName, TString ProvinceName,
			     std::vector<std::string> date_vector,
			     std::vector<int>         rate,
			     int                      rate_type) {

  if( verbose ) std::cout << "INFO: Saving Data for " << CountryName.Data()
			  << ", " << ProvinceName.Data() << " for "
			  << date_vector.size() << " days \n";
  
  //-------------------------------------------------------------//
  //        If country is not saved in map add the name
  //-------------------------------------------------------------//
  if ( Data_Countries->find(CountryName) == Data_Countries->end() ) {
    if ( verbose ) std::cout << "INFO: New country " << CountryName << std::endl;
    Data_Per_Country * tmp_country_data = new Data_Per_Country(CountryName);
    //    if ( verbose ) std::cout << "INFO: New country Created " << CountryName << std::endl;
    //if ( verbose ) std::cout << "INFO: New country Created " << tmp_country_data->name_country << std::endl;
    //    Data_Countries->insert( std::pair<TString, Data_Per_Country*>(CountryName, tmp_country_data) );
    Data_Countries->emplace( CountryName, tmp_country_data );
    //if ( verbose ) std::cout << "INFO: New country Added " << CountryName << std::endl;
    Data_Countries->at( CountryName )->setVerbose(verbose);
    //if ( verbose ) std::cout << "INFO: New country verbose Added " << CountryName << std::endl;
  };

  //--------------------------------------------------------------//
  //       Will only create a new province if it does not exist
  //       Protection alreay created in Data_Per_Country.h
  //--------------------------------------------------------------//
  Data_Countries->at( CountryName )->Add_Province( ProvinceName );

  if ( verbose ) std::cout << "INFO: New country Province Added " << CountryName << std::endl;
  
  for ( uint iday=0; iday < date_vector.size(); iday++ ) {
    
    int day, month, year;
    DecodeDate( date_vector.at(iday), day, month, year );
    
    if ( rate_type == type_cases_confirmed ) {
      //if ( verbose ) std::cout << "Number Confirmed " << rate.at(iday) << std::endl;

      Data_Countries->at( CountryName )
	->Province( ProvinceName )
	->Add_Cumulative_Data_for_Day_Confirmed( day, month, year,
						 rate.at(iday) );
    }
    else if ( rate_type == type_cases_deaths ) {

      //if ( verbose ) std::cout << "Number Deaths " << rate.at(iday) << std::endl;
      
      Data_Countries->at( CountryName )
	->Province( ProvinceName )
	->Add_Cumulative_Data_for_Day_Deaths(      day, month, year,
					         rate.at(iday) );
    }                                                            
    else if ( rate_type == type_cases_recovered ) {

      //if ( verbose ) std::cout << "Number Recovered " << rate.at(iday) << std::endl;
      
      Data_Countries->at( CountryName )
	->Province( ProvinceName )
	->Add_Cumulative_Data_for_Day_Recovered( day, month, year,
						 rate.at(iday) );
    }
    else {
      std::cout << "Error: rate type not found" << rate_type << "\n";
    }

  }
  
}

void Data_Analyzer::CalculateNewCasesPerDay() {

  std::map<TString, Data_Per_Country*>::iterator it;

  for (it = Data_Countries->begin(); it != Data_Countries->end(); it++) {
    it->second->Calculate_New_Cases_Per_Day_For_All_Provinces();
  }
  
}

void Data_Analyzer::CalculateCumulativeTotalForAllProvinces() {
  std::map<TString, Data_Per_Country*>::iterator it;

  for (it = Data_Countries->begin(); it != Data_Countries->end(); it++) {
    it->second->Calculate_Cumulative_Total_For_All_Provinces();
  }
}

void Data_Analyzer::GeneratePerCountryHistograms() {
  std::map<TString, Data_Per_Country*>::iterator it;

  for (it = Data_Countries->begin(); it != Data_Countries->end(); it++) {
    it->second->GeneratePerCountryHistograms();
  }

}

void Data_Analyzer::GeneratePerCountryHistogramsShifted( int n_shifted_days, int n_plus_days ) {
  std::map<TString, Data_Per_Country*>::iterator it;

  for (it = Data_Countries->begin(); it != Data_Countries->end(); it++) {
    it->second->GeneratePerCountryTotalHistogramsShifted( n_shifted_days, n_plus_days);
    it->second->GeneratePerCountryHistogramsShifted     ( n_shifted_days, n_plus_days );
  }

}

void Data_Analyzer::DecodeDate( std::string date_str, int & day, int & month, int & year ) {

  std::istringstream ss_date;
  ss_date.str(date_str);
  std::string token_date;

  int itoken=0;
  
  while(std::getline(ss_date, token_date, '/')) {
    if(itoken == 0) day   = stoi(token_date);
    else if(itoken == 1) month = stoi(token_date);
    else if(itoken == 2) year  = stoi(token_date);
    else {
      std::cout << "Error: Date String of wrong length or format " << date_str << std::endl;
    }
    itoken++;
  }
  
}

void Data_Analyzer::Read_Data(TString input_file, int cases_type) {

  std::string line;
  std::istringstream ss;
  std::string token;

  std::vector<std::string> date_vector;
  std::vector<int> rate;

  std::cout << "INFO: reading data from " << input_file.Data() << std::endl;
  
  //--------------------------------------------------------//
  //               Open File from directory
  //--------------------------------------------------------//
  std::ifstream myfile;
  myfile.open(input_file.Data());
  
  date_vector.clear();

  //--------------------------------------------------------//
  //           Open file and read data line by line
  //--------------------------------------------------------//
  
  if (myfile.is_open()) {

    
    std::getline (myfile,line);

    //-------------------------------------------------------------//
    //      Process First Line which contain labels and dates
    //         Break up Data in line using ","
    //-------------------------------------------------------------//

    ss.str(line);
    
    int itoken = 0;

    while ( std::getline(ss, token, ',') ) {

      if ( verbose ) std::cout << token << ' ';

      //--------------------------------------------//
      //      First line is label for the table
      //--------------------------------------------//
      if ( itoken < 4 ) { }
      else {
	date_vector.push_back(token);
      }

      itoken++;

    } // end of loop over tokens in a single line

    std::cout << std::endl;
    
    //---------------------------------------------//
    
    // Read File line by line
    while ( std::getline (myfile,line) ) {

      if ( verbose ) std::cout << line << std::endl;

      //---------------------------------------------//
      //    Process all data lines after first line
      //    Each line is a province, country
      //---------------------------------------------//
      
      TString ProvinceName = "";
      TString CountryName  = "";
      
      rate.clear();

      //---------------------------------------------//
      //  If we find a '"' character in the line
      //  This means we have a composite territory like "Virgin Islands, U.S."
      //  We need to remove any "," in the composite territory between the two
      //  '"' to avoid confusing the "," as deliminator
      //---------------------------------------------//

      std::size_t loc1 = line.find("\"");
      
      if ( loc1 != std::string::npos ) {
	std::size_t loc2 = line.find("\"", loc1+1);
	std::size_t loc  = line.find(",",  loc1+1);

	std::cout << loc1 << " " << loc << " " << loc2 << " " << loc2 - loc1 << std::endl;
	
	if ( loc != std::string::npos ) {
	  line.erase (loc, 1);
	  std::cout << loc1 << " " << loc << " " << loc2 << std::endl;
	  std::cout << line << std::endl;
	}
      }

      ss.clear();
      ss.str(line);

      for ( uint itoken = 0; itoken<date_vector.size()+4; itoken++ ) { 

	std::getline(ss, token, ',');

	if ( verbose ) std::cout << token << ' ';

	// Province Name
	if      ( itoken == 0 ) { ProvinceName = token; }
	// Country Name
	else if ( itoken == 1 ) { CountryName  = token; }
	// Latitude
	else if ( itoken == 2 ) { }
	// Longitude
	else if ( itoken == 3 ) { }
	else {
	  rate.push_back( std::stoi(token) );
	}

      } // end of loop over tokens in a single line

      //--------------------------------------------------------------//
      //     Ignore the First Line as it is the label of the table
      //     Save all other lines as country and province names
      //--------------------------------------------------------------//

      std::cout << std::endl;
      
      SaveData( CountryName, ProvinceName,
		date_vector, rate, cases_type );
      
    } // end of reading of all lines

    myfile.close();

    std::cout << std::endl;
    
  }
  else {
    std::cout << "Cannot open file" << std::endl;
  }

}

void Data_Analyzer::setVerbose( bool verb ) {

  verbose = verb;

  std::map<TString, Data_Per_Country*>::iterator it;
  
  for (it = Data_Countries->begin(); it != Data_Countries->end(); it++) {
    it->second->setVerbose(verb);
  }
}
