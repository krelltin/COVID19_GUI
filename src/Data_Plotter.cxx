#include "Data_Plotter.h"
#include "types.h"

//Constructor: Initiate Vectors
Data_Plotter::Data_Plotter( std::map<TString, Data_Per_Country*> * Data_Countries ) {

  m_sx = std::stringstream("");
  
  m_Data_Countries = Data_Countries;
  verbose = false;

  gStyle->SetOptStat(0);   
  
}

//Destructor: Delete all pointers
Data_Plotter::~Data_Plotter(){
}

void Data_Plotter::PlotPerCountry(TString output,  int n_cases_threshold, int type_cases) {

  auto legend_cumulative = new TLegend(0.1,0.7,0.4,0.9);
  auto legend_per_day    = new TLegend(0.1,0.7,0.4,0.9);

  TPostScript ps_per_country( (output+".eps").Data(), 112);

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->cd();
  c1->SetLogy(1);
  
  std::map<TString, Data_Per_Country*>::iterator it;

  for (it = m_Data_Countries->begin(); it != m_Data_Countries->end(); it++) {

    Data_Per_Country * icountry = it->second;

    if ( verbose ) std::cout << "INFO: Plotting histograms for " << icountry->name_country << std::endl;

    std::map<TString, Data_Per_Province*> * icountry_Provinces = icountry->Data_Provinces;
    Data_Per_Province                     * icountry_total     = icountry->Data_Total_All_Provinces;

    int icountry_total_cases_confirmed = icountry_total->Data_Cumulative->back()->cases_confirmed;
    int icountry_total_cases_deaths      = icountry_total->Data_Cumulative->back()->cases_deaths;

    if ( (icountry_total_cases_confirmed >= n_cases_threshold && type_cases == type_cases_confirmed) ||
	 (icountry_total_cases_deaths      >= n_cases_threshold && type_cases == type_cases_deaths)         ) {

      icountry_total->h_cumulative_confirmed->SetLineColor(kRed);
      icountry_total->h_cumulative_deaths     ->SetLineColor(kBlack);
      icountry_total->h_cumulative_recovered->SetLineColor(kGreen+2);

      legend_cumulative->Clear();
      legend_cumulative->AddEntry(icountry_total->h_cumulative_confirmed,
				  (icountry->name_country+" confirmed cases").Data(),"l");
      legend_cumulative->AddEntry(icountry_total->h_cumulative_deaths,
				  (icountry->name_country+" deaths"         ).Data(),"l");
      legend_cumulative->AddEntry(icountry_total->h_cumulative_recovered,
				  (icountry->name_country+" recovered cases").Data(),"l");

      legend_cumulative->SetHeader("Cumulative COVID19 Cases","C"); // option "C" allows to center the header
      
      icountry_total->h_cumulative_confirmed->Draw();
      icountry_total->h_cumulative_deaths     ->Draw("same");
      icountry_total->h_cumulative_recovered->Draw("same");
      legend_cumulative->Draw();
      c1->Update();
      ps_per_country.NewPage();      

      icountry_total->h_new_cases_per_day_confirmed->SetLineColor(kRed);
      icountry_total->h_new_cases_per_day_deaths     ->SetLineColor(kBlack);
      icountry_total->h_new_cases_per_day_recovered->SetLineColor(kGreen+2);

      legend_per_day->Clear();
      legend_per_day->SetHeader("New COVID19 Cases Per Day","C");
      legend_per_day->AddEntry(icountry_total->h_new_cases_per_day_confirmed,
			       (icountry->name_country+" confirmed cases").Data(),"l");
      legend_per_day->AddEntry(icountry_total->h_new_cases_per_day_deaths,
			       (icountry->name_country+" deaths"         ).Data(),"l");
      legend_per_day->AddEntry(icountry_total->h_new_cases_per_day_recovered,
			       (icountry->name_country+" recovered cases").Data(),"l");
      
      icountry_total->h_new_cases_per_day_confirmed->Draw();
      icountry_total->h_new_cases_per_day_deaths     ->Draw("same");
      icountry_total->h_new_cases_per_day_recovered->Draw("same");
      legend_per_day->Draw();
      c1->Update();
      ps_per_country.NewPage();

    }

  }

  ps_per_country.Close();

  c1->SetLogy(0);
  
  m_sx.str("");
  m_sx << "ps2pdf " << (output+".eps").Data();
  gSystem->Exec(m_sx.str().c_str());

  m_sx.str("");
  m_sx << "mv " << (output+".eps").Data() << " ./plots";
  gSystem->Exec(m_sx.str().c_str());

  m_sx.str("");
  m_sx << "mv " << (output+".pdf").Data() << " ./plots";
  gSystem->Exec(m_sx.str().c_str());

}

void Data_Plotter::PlotPerCountryOverlay(TString output,  int n_cases_threshold, int type_cases) {

  auto legend = new TLegend(0.6,0.7,0.9,0.9);
  legend->SetHeader("Countries With >= 30 Coronavirus Deaths","C"); // option "C" allows to center the header
  
  TPostScript ps_per_country( (output+".eps").Data(), 112);
  
  TCanvas *c_shift_cumulative_confirmed = new TCanvas("c_shift_cumulative_confirmed","c_shift_cumulative_confirmed");
  TCanvas *c_shift_cumulative_deaths      = new TCanvas("c_shift_cumulative_deaths"     ,"c_shift_cumulative_deaths");
  TCanvas *c_shift_cumulative_recovered = new TCanvas("c_shift_cumulative_recovered","c_shift_cumulative_recovered");
  TCanvas *c_shift_per_day_confirmed    = new TCanvas("c_shift_per_day_confirmed"   ,"c_shift_per_day_confirmed");
  TCanvas *c_shift_per_day_deaths         = new TCanvas("c_shift_per_day_deaths"        ,"c_shift_per_day_deaths");
  TCanvas *c_shift_per_day_recovered    = new TCanvas("c_shift_per_day_recovered"   ,"c_shift_per_day_recovered");

  int nCountriesPassed         = 0;
  int color                    = 0;
  TString firstCountry         = "";
  
  double max_cumulative_confirmed = 0;
  double max_cumulative_deaths      = 0;
  double max_cumulative_recovered = 0;
  double max_per_day_confirmed = 0;
  double max_per_day_deaths      = 0;
  double max_per_day_recovered = 0;    

  std::map<TString, Data_Per_Country*>::iterator it;

  for (it = m_Data_Countries->begin(); it != m_Data_Countries->end(); it++) {

    Data_Per_Country * icountry = it->second;

    if ( verbose ) std::cout << "INFO: Plotting histograms for " << icountry->name_country << std::endl; 
    
    std::map<TString, Data_Per_Province*> * icountry_Provinces = icountry->Data_Provinces;
    Data_Per_Province                     * icountry_total     = icountry->Data_Total_All_Provinces;
    
    int icountry_total_cases_confirmed = icountry_total->Data_Cumulative->back()->cases_confirmed;
    int icountry_total_cases_deaths      = icountry_total->Data_Cumulative->back()->cases_deaths;
    
    if ( (icountry_total_cases_confirmed >= n_cases_threshold && type_cases == type_cases_confirmed) ||
	 (icountry_total_cases_deaths      >= n_cases_threshold && type_cases == type_cases_deaths)         ) {

      color++;
      if (color == 5 || color == 10 ) color++; // skip yellow, white 

      if ( nCountriesPassed == 0 ) firstCountry = it->first;
      
      max_cumulative_confirmed = TMath::Max( max_cumulative_confirmed,
					     icountry_total->h_cumulative_confirmed_shifted->GetMaximum() );
      max_cumulative_deaths      = TMath::Max( max_cumulative_deaths,
					     icountry_total->h_cumulative_deaths_shifted->GetMaximum() );
      max_cumulative_recovered = TMath::Max( max_cumulative_recovered,
					     icountry_total->h_cumulative_recovered_shifted->GetMaximum() );

      max_per_day_confirmed    = TMath::Max( max_per_day_confirmed,
					     icountry_total->h_new_cases_per_day_confirmed_shifted->GetMaximum() );
      max_per_day_deaths         = TMath::Max( max_per_day_deaths,
					     icountry_total->h_new_cases_per_day_deaths_shifted->GetMaximum() );
      max_per_day_recovered    = TMath::Max( max_per_day_recovered,
					     icountry_total->h_new_cases_per_day_recovered_shifted->GetMaximum() );
      
      m_sx.str("same");
      if( nCountriesPassed == 0 ) m_sx.str("");
      
      icountry_total->h_cumulative_confirmed_shifted->SetLineColor(color);
      icountry_total->h_cumulative_deaths_shifted     ->SetLineColor(color);
      icountry_total->h_cumulative_recovered_shifted->SetLineColor(color);  
      
      c_shift_cumulative_confirmed->cd();
      icountry_total->h_cumulative_confirmed_shifted->Draw(m_sx.str().c_str());
      c_shift_cumulative_deaths->cd();
      icountry_total->h_cumulative_deaths_shifted     ->Draw(m_sx.str().c_str());
      c_shift_cumulative_recovered->cd();
      icountry_total->h_cumulative_recovered_shifted->Draw(m_sx.str().c_str());

      
      icountry_total->h_new_cases_per_day_confirmed_shifted->SetLineColor(color);
      icountry_total->h_new_cases_per_day_deaths_shifted     ->SetLineColor(color);
      icountry_total->h_new_cases_per_day_recovered_shifted->SetLineColor(color); 

      c_shift_per_day_confirmed->cd();
      icountry_total->h_new_cases_per_day_confirmed_shifted->Draw(m_sx.str().c_str());
      c_shift_per_day_deaths->cd();
      icountry_total->h_new_cases_per_day_deaths_shifted     ->Draw(m_sx.str().c_str());
      c_shift_per_day_recovered->cd();
      icountry_total->h_new_cases_per_day_recovered_shifted->Draw(m_sx.str().c_str());

      legend->AddEntry(icountry_total->h_cumulative_confirmed_shifted,icountry->name_country.Data(),"l");
      
      nCountriesPassed++;
    }
    
  }

  if ( nCountriesPassed > 0 ) {

    float scale = 1.5;
    
    m_Data_Countries->at( firstCountry )->Data_Total_All_Provinces
      ->h_cumulative_confirmed_shifted  ->SetMaximum( max_cumulative_confirmed*scale );
    m_Data_Countries->at( firstCountry )->Data_Total_All_Provinces
      ->h_cumulative_deaths_shifted       ->SetMaximum( max_cumulative_deaths     *scale );
    m_Data_Countries->at( firstCountry )->Data_Total_All_Provinces
      ->h_cumulative_recovered_shifted  ->SetMaximum( max_cumulative_recovered*scale );
    m_Data_Countries->at( firstCountry )->Data_Total_All_Provinces
      ->h_new_cases_per_day_confirmed_shifted->SetMaximum( max_per_day_confirmed*scale );
    m_Data_Countries->at( firstCountry )->Data_Total_All_Provinces
      ->h_new_cases_per_day_deaths_shifted     ->SetMaximum( max_per_day_deaths     *scale );
    m_Data_Countries->at( firstCountry )->Data_Total_All_Provinces
      ->h_new_cases_per_day_recovered_shifted->SetMaximum( max_per_day_recovered*scale );    

    c_shift_cumulative_confirmed->SetLogy(1);
    c_shift_cumulative_deaths->SetLogy(1);
    c_shift_cumulative_recovered->SetLogy(1);
    c_shift_per_day_confirmed->SetLogy(1);
    c_shift_per_day_deaths->SetLogy(1);
    c_shift_per_day_recovered->SetLogy(1);    
    
    c_shift_cumulative_confirmed->cd();
    legend->Draw();
    c_shift_cumulative_confirmed->Update();
    ps_per_country.NewPage();

    c_shift_cumulative_deaths->cd();
    legend->Draw();
    c_shift_cumulative_deaths->Update();
    ps_per_country.NewPage();

    c_shift_cumulative_recovered->cd();
    legend->Draw();
    c_shift_cumulative_recovered->Update();
    ps_per_country.NewPage();

    c_shift_per_day_confirmed->cd();
    legend->Draw();
    c_shift_per_day_confirmed->Update();
    ps_per_country.NewPage();

    c_shift_per_day_deaths->cd();
    legend->Draw();
    c_shift_per_day_deaths->Update();
    ps_per_country.NewPage();

    c_shift_per_day_recovered->cd();
    legend->Draw();
    c_shift_per_day_recovered->Update();
    ps_per_country.NewPage();

    c_shift_cumulative_confirmed->SetLogy(0);
    c_shift_cumulative_deaths->SetLogy(0);
    c_shift_cumulative_recovered->SetLogy(0);
    c_shift_per_day_confirmed->SetLogy(0);
    c_shift_per_day_deaths->SetLogy(0);
    c_shift_per_day_recovered->SetLogy(0);

    c_shift_cumulative_confirmed->cd();
    legend->Draw();
    c_shift_cumulative_confirmed->Update();
    ps_per_country.NewPage();

    c_shift_cumulative_deaths->cd();
    legend->Draw();
    c_shift_cumulative_deaths->Update();
    ps_per_country.NewPage();

    c_shift_cumulative_recovered->cd();
    legend->Draw();
    c_shift_cumulative_recovered->Update();
    ps_per_country.NewPage();

    c_shift_per_day_confirmed->cd();
    legend->Draw();
    c_shift_per_day_confirmed->Update();
    ps_per_country.NewPage();

    c_shift_per_day_deaths->cd();
    legend->Draw();
    c_shift_per_day_deaths->Update();
    ps_per_country.NewPage();

    c_shift_per_day_recovered->cd();
    legend->Draw();
    c_shift_per_day_recovered->Update();
    ps_per_country.NewPage();
    
  }
  
  ps_per_country.Close();
  
  m_sx.str("");
  m_sx << "ps2pdf " << (output+".eps").Data();
  gSystem->Exec(m_sx.str().c_str());

  m_sx.str("");
  m_sx << "mv " << (output+".eps").Data() << " ./plots";
  gSystem->Exec(m_sx.str().c_str());
  
  m_sx.str("");
  m_sx << "mv " << (output+".pdf").Data() << " ./plots";  
  gSystem->Exec(m_sx.str().c_str());
  
}



void Data_Plotter::setVerbose( bool verb ) {

  verbose = verb;

}
