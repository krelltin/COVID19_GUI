#include "Data_Per_Province.h"
#include "types.h"

//Constructor: Initiate Vectors
Data_Per_Province::Data_Per_Province( TString name ) {

  name_province = name;
  
  verbose = false;
  
  Data_Cumulative        = new std::vector<Data_Per_Day*>;
  Data_New_Cases_Per_Day = new std::vector<Data_Per_Day*>;

  h_cumulative_confirmed = NULL;
  h_cumulative_deaths = NULL;
  h_cumulative_recovered = NULL;      
  h_new_cases_per_day_confirmed = NULL;
  h_new_cases_per_day_deaths = NULL;
  h_new_cases_per_day_recovered = NULL;
  
  h_cumulative_confirmed_shifted        = NULL;
  h_cumulative_deaths_shifted             = NULL;
  h_cumulative_recovered_shifted        = NULL;
  h_new_cases_per_day_confirmed_shifted = NULL;
  h_new_cases_per_day_deaths_shifted      = NULL;
  h_new_cases_per_day_recovered_shifted = NULL;
  
  Clear();

}

//Destructor: Delete all pointers
Data_Per_Province::~Data_Per_Province(){
  delete Data_Cumulative;
  delete Data_New_Cases_Per_Day;
}

//Method for adding the number of cumulative number of cases that are confirmed, deaths, and recovered
void Data_Per_Province::Add_Cumulative_Data_for_Day ( int day, int month, int year,
						      int cumulative_cases_confirmed,
						      int cumulative_cases_deaths,
						      int cumulative_cases_recovered ) {


  Data_Per_Day * tmp_Data_Per_Day = Find_Data_For_Day( day, month, year, Data_Cumulative );
  
  bool found_day = true;

  if ( !tmp_Data_Per_Day ) {
    found_day = false;
    tmp_Data_Per_Day = new Data_Per_Day( day, month, year );
  }
  
  // The pointer will point to the one in Data_Cumulative
  tmp_Data_Per_Day->cases_confirmed = cumulative_cases_confirmed;
  tmp_Data_Per_Day->cases_deaths      = cumulative_cases_deaths;
  tmp_Data_Per_Day->cases_recovered = cumulative_cases_recovered;

  // If not found.  add day to the end.
  if (!found_day)  Data_Cumulative->push_back(tmp_Data_Per_Day);
    
  return;
}

void  Data_Per_Province::Add_Cumulative_Data_for_Day_Confirmed ( int day, int month, int year,
								 int cumulative_cases_confirmed ) {

  Data_Per_Day * tmp_Data_Per_Day = Find_Data_For_Day( day, month, year, Data_Cumulative );
  bool found_day = true;

  if ( !tmp_Data_Per_Day ) {
    tmp_Data_Per_Day = new Data_Per_Day( day, month, year );
    found_day = false;
  }                                                                                         

  tmp_Data_Per_Day->cases_confirmed = cumulative_cases_confirmed;
  
  if (!found_day)  Data_Cumulative->push_back(tmp_Data_Per_Day);
  
}
void  Data_Per_Province::Add_Cumulative_Data_for_Day_Deaths (      int day, int month, int year,
								 int cumulative_cases_deaths ) {
  
  Data_Per_Day * tmp_Data_Per_Day = Find_Data_For_Day( day, month, year, Data_Cumulative );
  bool found_day = true;
  
  if ( !tmp_Data_Per_Day ) {
    tmp_Data_Per_Day = new Data_Per_Day( day, month, year );
    found_day = false;
  }
  
  tmp_Data_Per_Day->cases_deaths = cumulative_cases_deaths;

  if (!found_day)  Data_Cumulative->push_back(tmp_Data_Per_Day);
}

void  Data_Per_Province::Add_Cumulative_Data_for_Day_Recovered ( int day, int month, int year,
								 int cumulative_cases_recovered ){

    Data_Per_Day * tmp_Data_Per_Day = Find_Data_For_Day( day, month, year, Data_Cumulative );
  bool found_day = true; 

  if ( !tmp_Data_Per_Day ) {
    tmp_Data_Per_Day = new Data_Per_Day( day, month, year );
    found_day = false;
  }
  
  tmp_Data_Per_Day->cases_recovered = cumulative_cases_recovered;
  if (!found_day)  Data_Cumulative->push_back(tmp_Data_Per_Day);
  
}

// Method to compute the differences in rate per day by subtracting today from the day before
void Data_Per_Province::Calculate_New_Cases_Per_Day() {

  //  Delete everything in cases per day vector to refill
  Data_New_Cases_Per_Day->clear();

  // Temporary variables to store data from today and yesterday
  Data_Per_Day * Data_Yesterday;
  Data_Per_Day * Data_Today;
  
  for (uint iday=0; iday < Data_Cumulative->size(); iday++ ) {

    // If first day consider all cases as new cases on day 1
    if ( iday == 0 ) {
      Data_New_Cases_Per_Day->push_back( Data_Cumulative->at(iday) );
      continue;
    }

    Data_Yesterday = Data_Cumulative->at( iday - 1 );
    Data_Today     = Data_Cumulative->at( iday );

    // Calculate the new cases today as the difference between cases yesterday and today
    int new_cases_confirmed = Data_Today->cases_confirmed - Data_Yesterday->cases_confirmed;
    int new_cases_deaths      = Data_Today->cases_deaths      - Data_Yesterday->cases_deaths;
    int new_cases_recovered = Data_Today->cases_recovered - Data_Yesterday->cases_recovered;

    // Store new cases today in a new temporary Data_Per_Day object
    Data_Per_Day * tmp_New_Cases_Today = new Data_Per_Day(Data_Today->day,
							  Data_Today->month,
							  Data_Today->year);

    tmp_New_Cases_Today->cases_confirmed = new_cases_confirmed;
    tmp_New_Cases_Today->cases_deaths      = new_cases_deaths;
    tmp_New_Cases_Today->cases_recovered = new_cases_recovered;

    // Save the cases per day in the Data_New_Cases_Per_Day vector
    Data_New_Cases_Per_Day->push_back( tmp_New_Cases_Today );
  }
  
}

// Not so much data, just loop and search.  Could do binary later if we need it
Data_Per_Day * Data_Per_Province::Find_Data_For_Day( int day, int month, int year,
						     std::vector<Data_Per_Day*> * Data_Vector) {

  Data_Per_Day * Data_on_Day = NULL;
  
  for ( uint iday=0; iday < Data_Vector->size(); iday++ ) {

    if ( Data_Vector->at(iday)->year == year ) {
      if ( Data_Vector->at(iday)->month == month ) {
	if ( Data_Vector->at(iday)->day == day ) {
	  Data_on_Day = Data_Vector->at(iday);
	  break;
	}
      }
    }
  }

  return Data_on_Day;
  
}

Data_Per_Day * Data_Per_Province::Get_Cumulative_Data_for_Day( int day, int month, int year) {

  Data_Per_Day * Data_on_Day = Find_Data_For_Day( day, month, year,
						  Data_Cumulative);
  return Data_on_Day;
}

Data_Per_Day * Data_Per_Province::Get_New_Cases_Data_for_Day ( int day, int month, int year) {

  Data_Per_Day * Data_on_Day = Find_Data_For_Day( day, month, year, 
						  Data_New_Cases_Per_Day);
  return Data_on_Day;
}

Data_Per_Day * Data_Per_Province::Get_Cumulative_Data_for_Day( uint iday ) {

  Data_Per_Day * Data_on_Day = NULL;

  if ( Data_Cumulative->size() <= iday ) {
    Data_on_Day = Data_Cumulative->at(iday);
  }

  return Data_on_Day;
}

Data_Per_Day * Data_Per_Province::Get_New_Cases_Data_for_Day( uint iday ) {

  Data_Per_Day * Data_on_Day = NULL;

  if ( Data_New_Cases_Per_Day->size() <= iday ) {
    Data_on_Day = Data_New_Cases_Per_Day->at(iday);
  }

  return Data_on_Day;
}

bool Data_Per_Province::CombineCumulative( Data_Per_Province * province) {

  if ( Data_Cumulative->size() == 0 ) {

    for ( uint iday = 0; iday < province->Data_Cumulative->size(); iday++ ) {

      Data_Per_Day * tmp = new Data_Per_Day( province->Data_Cumulative->at(iday)->day,
					     province->Data_Cumulative->at(iday)->month,
					     province->Data_Cumulative->at(iday)->year );

      tmp->cases_confirmed = province->Data_Cumulative->at(iday)->cases_confirmed;
      tmp->cases_deaths      = province->Data_Cumulative->at(iday)->cases_deaths;
      tmp->cases_recovered = province->Data_Cumulative->at(iday)->cases_recovered;
      
      Data_Cumulative->push_back( tmp );
    }

    return true;
	
  }

  if ( (province->Data_Cumulative->size()        != Data_Cumulative->size()) ) {
    std::cout << "Error: Province " << province->name_province
	      << " and Province "   << name_province
	      << " don't have the same number of days in their dataset.  They cannot be combined. ";
    return false;
  }
  
  for ( uint iday = 0; iday < province->Data_Cumulative->size(); iday++ ) {
    Data_Cumulative->at(iday)->cases_confirmed        += province->Data_Cumulative->at(iday)->cases_confirmed;  
    Data_Cumulative->at(iday)->cases_deaths             += province->Data_Cumulative->at(iday)->cases_deaths;
    Data_Cumulative->at(iday)->cases_recovered        += province->Data_Cumulative->at(iday)->cases_recovered;
  }

  return true;
  
}

void Data_Per_Province::GenerateHistograms(TString CountryName) {

  std::stringstream m_ss_date;
  
  if ( !h_cumulative_confirmed ) {
    h_cumulative_confirmed = new TH1F( (CountryName+"_"+name_province+"_Cumulative_Confirmed_Cases").Data(),
				       (CountryName+"_"+name_province+"_Cumulative_Confirmed_Cases").Data(),
				       Data_Cumulative->size()+5,0,Data_Cumulative->size()+5 );
  }
  else {
    h_cumulative_confirmed->SetBins( Data_Cumulative->size()+5,0,Data_Cumulative->size()+5 );
  }

  if ( !h_cumulative_deaths ) {
    h_cumulative_deaths      = new TH1F( (CountryName+"_"+name_province+"_Cumulative_Deaths_Cases").Data(),
				       (CountryName+"_"+name_province+"_Cumulative_Deaths_Cases").Data(),
				       Data_Cumulative->size()+5,0,Data_Cumulative->size()+5 );
  }
  else {
    h_cumulative_deaths->SetBins( Data_Cumulative->size()+5,0,Data_Cumulative->size()+5 );
  }
  
  if ( !h_cumulative_recovered ) {
    h_cumulative_recovered = new TH1F( (CountryName+"_"+name_province+"_Cumulative_Recovered_Cases").Data(),
				       (CountryName+"_"+name_province+"_Cumulative_Recovered_Cases").Data(),
				       Data_Cumulative->size()+5,0,Data_Cumulative->size()+5 );
  }
  else {
    h_cumulative_recovered->SetBins( Data_Cumulative->size()+5,0,Data_Cumulative->size()+5 );
  }
  
  if ( !h_new_cases_per_day_confirmed ) {
    h_new_cases_per_day_confirmed = new TH1F( (CountryName+"_"+name_province+"_New_Cases_Per_Day_Confirmed_Cases").Data(),
					      (CountryName+"_"+name_province+"_New_Cases_Per_Day_Confirmed_Cases").Data(),
					      Data_New_Cases_Per_Day->size()+5,0,Data_New_Cases_Per_Day->size()+5 );
  }
  else {
    h_new_cases_per_day_confirmed->SetBins( Data_New_Cases_Per_Day->size()+5,0,Data_New_Cases_Per_Day->size()+5 );
  }
  
  if ( !h_new_cases_per_day_deaths ) {
    h_new_cases_per_day_deaths      = new TH1F( (CountryName+"_"+name_province+"_New_Cases_Per_Day_Deaths_Cases").Data(),
					      (CountryName+"_"+name_province+"_New_Cases_Per_Day_Deaths_Cases").Data(),
					      Data_New_Cases_Per_Day->size()+5,0,Data_New_Cases_Per_Day->size()+5 );
  }
  else {
    h_new_cases_per_day_deaths->SetBins( Data_New_Cases_Per_Day->size()+5,0,Data_New_Cases_Per_Day->size()+5 );
  }
  
  if ( !h_new_cases_per_day_recovered ) {
    h_new_cases_per_day_recovered = new TH1F( (CountryName+"_"+name_province+"_New_Cases_Per_Day_Recovered_Cases").Data(),
					      (CountryName+"_"+name_province+"_New_Cases_Per_Day_Recovered_Cases").Data(),
					      Data_New_Cases_Per_Day->size()+5,0,Data_New_Cases_Per_Day->size()+5 );
  }
  else {
    h_new_cases_per_day_recovered->SetBins( Data_New_Cases_Per_Day->size()+5,0,Data_New_Cases_Per_Day->size()+5 );
  }

  for ( uint iday=0; iday<Data_Cumulative->size()+5; iday++ ) {

    if ( iday < Data_Cumulative->size() ) {
      //    if ( verbose ) std::cout << "Number deaths " << Data_Cumulative->at(iday)->cases_deaths << std::endl;
      
      m_ss_date.str("");
      m_ss_date << Data_Cumulative->at(iday)->day << "/" << Data_Cumulative->at(iday)->month;
      
      h_cumulative_confirmed->SetBinContent( iday+1, Data_Cumulative->at(iday)->cases_confirmed );
      h_cumulative_confirmed->SetBinError(   iday+1, sqrt(Data_Cumulative->at(iday)->cases_confirmed) );
      h_cumulative_confirmed->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
      
      h_cumulative_deaths->SetBinContent( iday+1, Data_Cumulative->at(iday)->cases_deaths );
      h_cumulative_deaths->SetBinError(   iday+1, sqrt(Data_Cumulative->at(iday)->cases_deaths) );
      h_cumulative_deaths->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
      
      h_cumulative_recovered->SetBinContent( iday+1, Data_Cumulative->at(iday)->cases_recovered );
      h_cumulative_recovered->SetBinError(   iday+1, sqrt(Data_Cumulative->at(iday)->cases_recovered) );
      h_cumulative_recovered->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
    }
    else {

      m_ss_date.str("");
      
      h_cumulative_confirmed->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
      h_cumulative_deaths     ->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
      h_cumulative_recovered->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
    }

  }

  for ( uint iday=0; iday<Data_New_Cases_Per_Day->size()+5; iday++ ) {

    if ( iday < Data_New_Cases_Per_Day->size() ) {
      m_ss_date.str("");
      m_ss_date << Data_New_Cases_Per_Day->at(iday)->day << "/" << Data_New_Cases_Per_Day->at(iday)->month;
      
      h_new_cases_per_day_confirmed->SetBinContent( iday+1, Data_New_Cases_Per_Day->at(iday)->cases_confirmed );
      h_new_cases_per_day_confirmed->SetBinError(   iday+1, sqrt(Data_New_Cases_Per_Day->at(iday)->cases_confirmed) );
      h_new_cases_per_day_confirmed->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
      
      h_new_cases_per_day_deaths->SetBinContent( iday+1, Data_New_Cases_Per_Day->at(iday)->cases_deaths );
      h_new_cases_per_day_deaths->SetBinError(   iday+1, sqrt(Data_New_Cases_Per_Day->at(iday)->cases_deaths) );
      h_new_cases_per_day_deaths->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
      
      h_new_cases_per_day_recovered->SetBinContent( iday+1, Data_New_Cases_Per_Day->at(iday)->cases_recovered );
      h_new_cases_per_day_recovered->SetBinError(   iday+1, sqrt(Data_New_Cases_Per_Day->at(iday)->cases_recovered) );
      h_new_cases_per_day_recovered->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
    }
    else {
      m_ss_date.str("");

      h_new_cases_per_day_confirmed->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
      h_new_cases_per_day_deaths     ->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
      h_new_cases_per_day_recovered->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
    }

      
  }

}

void Data_Per_Province::GenerateHistogramsShifted(TString CountryName, int n_shifted_days, int n_plus_days) {

  std::stringstream m_ss_date;
  
  if ( !h_cumulative_confirmed_shifted ) {
    h_cumulative_confirmed_shifted = new TH1F( (CountryName+"_"+name_province+"_Cumulative_Confirmed_Cases_Shifted").Data(),
					       (CountryName+"_"+name_province+"_Cumulative_Confirmed_Cases_Shifted").Data(),
					       Data_Cumulative->size()+n_plus_days,0,Data_Cumulative->size()+n_plus_days );
  }
  else {
    h_cumulative_confirmed_shifted->SetBins( Data_Cumulative->size()+n_plus_days,0,Data_Cumulative->size()+n_plus_days );
  }

  if ( !h_cumulative_deaths_shifted ) {
    h_cumulative_deaths_shifted      = new TH1F( (CountryName+"_"+name_province+"_Cumulative_Deaths_Cases_Shifted").Data(),
					       (CountryName+"_"+name_province+"_Cumulative_Deaths_Cases_Shifted").Data(),
					       Data_Cumulative->size()+n_plus_days,0,Data_Cumulative->size()+n_plus_days );
  }
  else {
    h_cumulative_deaths_shifted->SetBins( Data_Cumulative->size()+n_plus_days,0,Data_Cumulative->size()+n_plus_days );
  }

  if ( !h_cumulative_recovered_shifted ) {
    h_cumulative_recovered_shifted = new TH1F( (CountryName+"_"+name_province+"_Cumulative_Recovered_Cases_Shifted").Data(),
					       (CountryName+"_"+name_province+"_Cumulative_Recovered_Cases_Shifted").Data(),
					       Data_Cumulative->size()+n_plus_days,0,Data_Cumulative->size()+n_plus_days );
  }
  else {
    h_cumulative_recovered_shifted->SetBins( Data_Cumulative->size()+n_plus_days,0,Data_Cumulative->size()+n_plus_days );
  }
  
  if ( !h_new_cases_per_day_confirmed_shifted ) {
    h_new_cases_per_day_confirmed_shifted = new TH1F( (CountryName+"_"+name_province+"_New_Cases_Per_Day_Confirmed_Cases_Shifted").Data(),
						      (CountryName+"_"+name_province+"_New_Cases_Per_Day_Confirmed_Cases_Shifted").Data(),
						      Data_New_Cases_Per_Day->size()+n_plus_days,0,Data_New_Cases_Per_Day->size()+n_plus_days );
  }
  else {
    h_new_cases_per_day_confirmed_shifted->SetBins( Data_New_Cases_Per_Day->size()+n_plus_days,0,Data_New_Cases_Per_Day->size()+n_plus_days );
  }

  if ( !h_new_cases_per_day_deaths_shifted ) {
    h_new_cases_per_day_deaths_shifted      = new TH1F( (CountryName+"_"+name_province+"_New_Cases_Per_Day_Deaths_Cases_Shifted").Data(),
						      (CountryName+"_"+name_province+"_New_Cases_Per_Day_Deaths_Cases_Shifted").Data(),
						      Data_New_Cases_Per_Day->size()+n_plus_days,0,Data_New_Cases_Per_Day->size()+n_plus_days );
  }
  else {
    h_new_cases_per_day_deaths_shifted->SetBins( Data_New_Cases_Per_Day->size()+n_plus_days,0,Data_New_Cases_Per_Day->size()+n_plus_days );
  }

  if ( !h_new_cases_per_day_recovered_shifted ) {
    h_new_cases_per_day_recovered_shifted = new TH1F( (CountryName+"_"+name_province+"_New_Cases_Per_Day_Recovered_Cases_Shifted").Data(),
						      (CountryName+"_"+name_province+"_New_Cases_Per_Day_Recovered_Cases_Shifted").Data(),
						      Data_New_Cases_Per_Day->size()+n_plus_days,0,Data_New_Cases_Per_Day->size()+n_plus_days );
  }
  else {
    h_new_cases_per_day_recovered_shifted->SetBins( Data_New_Cases_Per_Day->size()+n_plus_days,0,Data_New_Cases_Per_Day->size()+n_plus_days );
  }

  for ( uint ibin=1; ibin<Data_Cumulative->size()+n_plus_days+1; ibin++ ) {

    int iday = ibin - 1 - n_shifted_days;
    
    if (  iday >= 0 && iday < Data_Cumulative->size() ) { 
    
      m_ss_date.str("");
      m_ss_date << Data_Cumulative->at(iday)->day << "/" << Data_Cumulative->at(iday)->month;
      
      h_cumulative_confirmed_shifted->SetBinContent( ibin, Data_Cumulative->at(iday)->cases_confirmed );
      h_cumulative_confirmed_shifted->SetBinError(   ibin, sqrt(Data_Cumulative->at(iday)->cases_confirmed) );
      h_cumulative_confirmed_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
      
      h_cumulative_deaths_shifted->SetBinContent( ibin, Data_Cumulative->at(iday)->cases_deaths );
      h_cumulative_deaths_shifted->SetBinError(   ibin, sqrt(Data_Cumulative->at(iday)->cases_deaths) );
      h_cumulative_deaths_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
      
      h_cumulative_recovered_shifted->SetBinContent( ibin, Data_Cumulative->at(iday)->cases_recovered );
      h_cumulative_recovered_shifted->SetBinError(   ibin, sqrt(Data_Cumulative->at(iday)->cases_recovered) );
      h_cumulative_recovered_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
    }
    else {
      
      m_ss_date.str("");
      
      h_cumulative_confirmed_shifted->SetBinContent( ibin, 0.0 );
      h_cumulative_confirmed_shifted->SetBinError(   ibin, 0.0 );
      
      h_cumulative_deaths_shifted->SetBinContent(    ibin, 0.0 );
      h_cumulative_deaths_shifted->SetBinError(      ibin, 0.0 );

      h_cumulative_recovered_shifted->SetBinContent( ibin, 0.0 );
      h_cumulative_recovered_shifted->SetBinError(   ibin, 0.0 );

      h_cumulative_confirmed_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
      h_cumulative_deaths_shifted   ->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
      h_cumulative_recovered_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );

    }
  } // End of Loop over Data_Cumulative

  for ( uint ibin=1; ibin<Data_New_Cases_Per_Day->size()+n_plus_days+1; ibin++ ) {
    
    int iday = ibin - 1 - n_shifted_days;
    
    if (  iday >= 0 && iday < Data_New_Cases_Per_Day->size() ) {
    
      m_ss_date.str("");
      m_ss_date << Data_New_Cases_Per_Day->at(iday)->day << "/" << Data_New_Cases_Per_Day->at(iday)->month;
      
      h_new_cases_per_day_confirmed_shifted->SetBinContent( ibin, Data_New_Cases_Per_Day->at(iday)->cases_confirmed );
      h_new_cases_per_day_confirmed_shifted->SetBinError(   ibin, sqrt(Data_New_Cases_Per_Day->at(iday)->cases_confirmed) );
      h_new_cases_per_day_confirmed_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
      
      h_new_cases_per_day_deaths_shifted->SetBinContent( ibin, Data_New_Cases_Per_Day->at(iday)->cases_deaths );
      h_new_cases_per_day_deaths_shifted->SetBinError(   ibin, sqrt(Data_New_Cases_Per_Day->at(iday)->cases_deaths) );
      h_new_cases_per_day_deaths_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
      
      h_new_cases_per_day_recovered_shifted->SetBinContent( ibin, Data_New_Cases_Per_Day->at(iday)->cases_recovered );
      h_new_cases_per_day_recovered_shifted->SetBinError(   ibin, sqrt(Data_New_Cases_Per_Day->at(iday)->cases_recovered) );
      h_new_cases_per_day_recovered_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
    }
    else {
      m_ss_date.str("");

      h_new_cases_per_day_confirmed_shifted->SetBinContent( ibin, 0.0 );
      h_new_cases_per_day_confirmed_shifted->SetBinError(   ibin, 0.0 );
      h_new_cases_per_day_deaths_shifted->SetBinContent( ibin, 0.0 );
      h_new_cases_per_day_deaths_shifted->SetBinError(   ibin, 0.0 );
      h_new_cases_per_day_recovered_shifted->SetBinContent( ibin, 0.0 );
      h_new_cases_per_day_recovered_shifted->SetBinError(   ibin, 0.0 );
      
      h_new_cases_per_day_confirmed_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
      h_new_cases_per_day_deaths_shifted     ->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
      h_new_cases_per_day_recovered_shifted->GetXaxis()->SetBinLabel( ibin, m_ss_date.str().c_str() );
    }

  }// end of loop over New Cases Per Day Vector

  if ( verbose ) std::cout << "INFO: Shifted by " << n_shifted_days << std::endl;
  
}

void Data_Per_Province::Clear() {
  Data_Cumulative->clear();
  Data_New_Cases_Per_Day->clear();
}

void Data_Per_Province::setVerbose( bool verb ) {
  verbose = verb;
}
