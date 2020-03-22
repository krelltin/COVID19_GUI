
#ifndef Data_Plotter_h
#define Data_Plotter_h

#include "Data_Analyzer.h"

#include <map>
#include <TString.h>
#include <TMath.h>

#include <TStyle.h>
#include <TSystem.h>
#include <TPostScript.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TGAxis.h>

#include <iostream>
#include <fstream>
#include <sstream>

class Data_Plotter {
 public :

  // Constructor and Destructor
  Data_Plotter( std::map<TString, Data_Per_Country*> * Data_Countries );
  ~Data_Plotter();

  std::map<TString, Data_Per_Country*> * m_Data_Countries;
  
  bool verbose;
  
  std::stringstream m_sx ;
  
  void PlotPerCountry       (TString output,  int n_cases_threshold, int type_cases);
  void PlotPerCountryOverlay(TString output,  int n_cases_threshold, int type_cases);
  
  void setVerbose(bool verb);
  
};

#endif

