#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "types.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

  n_future_days = 10;
  
  ui->setupUi(this);

  CreateCanvas();

  SetupCanvas();
  
  verbose = true;
  
  TString inputDir = "/mnt/c/Users/sssun/Documents/Coronavirus_Analysis/COVID-19/csse_covid_19_data/csse_covid_19_time_series";
  
  dAna = new Data_Analyzer();
  dAna->setVerbose(verbose);

  dAna->Read_Data( inputDir+"/time_series_19-covid-Confirmed.csv", type_cases_confirmed );
  dAna->Read_Data( inputDir+"/time_series_19-covid-Deaths.csv",    type_cases_deaths );
  dAna->Read_Data( inputDir+"/time_series_19-covid-Recovered.csv", type_cases_recovered );
  
  //  Total must be calculated before new cases per day
  dAna->CalculateCumulativeTotalForAllProvinces();
  //  Calculate cases per day
  dAna->CalculateNewCasesPerDay();

  dAna->GeneratePerCountryHistograms();
  dAna->GeneratePerCountryHistogramsShifted( 0, n_future_days );
  //  dAna->GeneratePerCountryHistogramsShifted( 1, n_future_days );
  
  if ( verbose ) std::cout << "histograms plotted" << std::endl;

  //----------------------------------------------------------//
  //              Get Most Effected Countries
  //----------------------------------------------------------//
  
  if ( dAna->Data_Countries->find("US")              != dAna->Data_Countries->end() ) {
    Country_USA         = dAna->Data_Countries->at("US");
  }
  else {
    std::cout << "Error: Can't Find USA" << std::endl;
    return;
  }
  if ( dAna->Data_Countries->find("China")           != dAna->Data_Countries->end() ) {
    Country_China       = dAna->Data_Countries->at("China");
  }
  else {
    std::cout << "Error: Can't Find China" << std::endl;
    return;
  }
  if ( dAna->Data_Countries->find("Italy")           != dAna->Data_Countries->end() ) {
    Country_Italy       = dAna->Data_Countries->at("Italy");
  }
  else {
    std::cout << "Error: Can't Find Italy" << std::endl;
    return;
  }
  if ( dAna->Data_Countries->find("Iran")            != dAna->Data_Countries->end() ) {
    Country_Iran        = dAna->Data_Countries->at("Iran");
  }
  else {
    std::cout << "Error: Can't Find Iran" << std::endl;
    return;
  }
  if ( dAna->Data_Countries->find("Germany")         != dAna->Data_Countries->end() ) {
    Country_Germany     = dAna->Data_Countries->at("Germany");
  }
  else {
    std::cout << "Error: Can't Find Germany" << std::endl;
    return;
  }
  if ( dAna->Data_Countries->find("France")          != dAna->Data_Countries->end() ) {
    Country_France      = dAna->Data_Countries->at("France");
  }
  else {
    std::cout << "Error: Can't Find France" << std::endl;
    return;
  }

  if ( dAna->Data_Countries->find("United Kingdom")              != dAna->Data_Countries->end() ) {
    Country_UK          = dAna->Data_Countries->at("United Kingdom");
  }
  else {
    std::cout << "Error: Can't Find UK" << std::endl;
    return;
  }

  if ( dAna->Data_Countries->find("Spain")           != dAna->Data_Countries->end() ) {
    Country_Spain       = dAna->Data_Countries->at("Spain");
  }
  else {
    std::cout << "Error: Can't Find Spain" << std::endl;
    return;
  }

  if ( dAna->Data_Countries->find("Switzerland")     != dAna->Data_Countries->end() ) {
    Country_Switzerland = dAna->Data_Countries->at("Switzerland");
  }
  else {
    std::cout << "Error: Can't Find Switzerland" << std::endl;
    return;
  }

  if ( dAna->Data_Countries->find("\"Korea South\"") != dAna->Data_Countries->end() ) {
    Country_SouthKorea  = dAna->Data_Countries->at("\"Korea South\"");
  }
  else {
    std::cout << "Error: Can't Find S. Korea" << std::endl;
    return;
  }
  
  //Country_SouthKorea->name_country = "South Korea";
  //Country_UK->name_country         = "United Kingdom";
  //Country_USA->name_country        = "United States";
  
  // ------------------------------------------------------------------------------- //

  InitHistos();

  SetCountryPlotStyle( Country_China,       kOrange+2 );
  SetCountryPlotStyle( Country_France,      kBlack );
  SetCountryPlotStyle( Country_Germany,     kBlue );
  SetCountryPlotStyle( Country_Iran,        kViolet+2 );
  SetCountryPlotStyle( Country_Italy,       kCyan+1 );
  SetCountryPlotStyle( Country_SouthKorea,  kGreen+2 );
  SetCountryPlotStyle( Country_Spain,       kOrange+2 );
  SetCountryPlotStyle( Country_Switzerland, 28 ); // Brown
  SetCountryPlotStyle( Country_UK,          kRed-4 );
  SetCountryPlotStyle( Country_USA,         kMagenta );
  
  DrawHist();

  // ------------------------------------------------------------------------------- //

  connect(ui->China_ShowData,        SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  connect(ui->France_ShowData,       SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  connect(ui->Germany_ShowData,      SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  connect(ui->Iran_ShowData,         SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  connect(ui->Italy_ShowData,        SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  connect(ui->SouthKorea_ShowData,   SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  connect(ui->Spain_ShowData,        SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  connect(ui->Switzerland_ShowData,  SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  connect(ui->UnitedKingdom_ShowData,SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  connect(ui->USA_ShowData,          SIGNAL(stateChanged(int)),this,SLOT(DrawHist()));
  
  // ------------------------------------------------------------------------------- //
  
  connect(ui->China_ShiftNDays,        SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));
  connect(ui->France_ShiftNDays,       SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));
  connect(ui->Germany_ShiftNDays,      SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));
  connect(ui->Iran_ShiftNDays,         SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));
  connect(ui->Italy_ShiftNDays,        SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));
  connect(ui->SouthKorea_ShiftNDays,   SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));
  connect(ui->Spain_ShiftNDays,        SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));
  connect(ui->Switzerland_ShiftNDays,  SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));
  connect(ui->UnitedKingdom_ShiftNDays,SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));
  connect(ui->USA_ShiftNDays,          SIGNAL(valueChanged(int)),this,SLOT(ShiftNDays()));

}

MainWindow::~MainWindow() {
  delete c_main;
  delete h_main_plot;

}

// ------------------------------------------------------------------------------------ //

void MainWindow::ShiftNDays() {

  if ( ui->China_ShiftNDays         == QObject::sender() ) {
    ShiftCountryNDays( Country_China,       ui->China_ShiftNDays->value(), n_future_days );
  }
  if ( ui->France_ShiftNDays        == QObject::sender() ) {
    ShiftCountryNDays( Country_France,      ui->France_ShiftNDays->value(), n_future_days );
  }
  if ( ui->Germany_ShiftNDays       == QObject::sender() ) {
    ShiftCountryNDays( Country_Germany,     ui->Germany_ShiftNDays->value(), n_future_days );
  }
  if ( ui->Iran_ShiftNDays          == QObject::sender() ) {
    ShiftCountryNDays( Country_Iran,        ui->Iran_ShiftNDays->value(), n_future_days );
  }
  if ( ui->Italy_ShiftNDays         == QObject::sender() ) {
    ShiftCountryNDays( Country_Italy,       ui->Italy_ShiftNDays->value(), n_future_days );
  }
  if ( ui->SouthKorea_ShiftNDays    == QObject::sender() ) {
    ShiftCountryNDays( Country_SouthKorea,  ui->SouthKorea_ShiftNDays->value(), n_future_days );
  }
  if ( ui->Spain_ShiftNDays         == QObject::sender() ) {
    ShiftCountryNDays( Country_Spain,       ui->Spain_ShiftNDays->value(), n_future_days );
  }
  if ( ui->Switzerland_ShiftNDays   == QObject::sender() ) {
    ShiftCountryNDays( Country_Switzerland, ui->Switzerland_ShiftNDays->value(), n_future_days );
  }
  if ( ui->UnitedKingdom_ShiftNDays == QObject::sender() ) {
    ShiftCountryNDays( Country_UK,          ui->UnitedKingdom_ShiftNDays->value(), n_future_days );
  }
  if ( ui->USA_ShiftNDays           == QObject::sender() ) {
    ShiftCountryNDays( Country_USA,         ui->USA_ShiftNDays->value(), n_future_days );
  }

  DrawHist();
  
}

void MainWindow::ShiftCountryNDays( Data_Per_Country * Country, int shift_ndays, int n_next_days ) {

  if ( verbose ) {
    std::cout << "attempting to shift " << Country->name_country
	      << " by " << shift_ndays << " days "
	      << " with max " << n_next_days << " predicted into the future " << std::endl;
  }
  
  Country->GeneratePerCountryTotalHistogramsShifted(shift_ndays, n_next_days);

}

// ------------------------------------------------------------------------------------ //

void MainWindow::DrawHist() {

  c_main->cd(1);

  h_main_plot->Draw();

  MaximumCases = 0;
  
  if ( ui->China_ShowData->isChecked() )         DrawCountry ( Country_China );
  if ( ui->France_ShowData->isChecked() )        DrawCountry ( Country_France );
  if ( ui->Germany_ShowData->isChecked() )       DrawCountry ( Country_Germany );
  if ( ui->Iran_ShowData->isChecked() )          DrawCountry ( Country_Iran );
  if ( ui->Italy_ShowData->isChecked() )         DrawCountry ( Country_Italy );
  if ( ui->SouthKorea_ShowData->isChecked() )    DrawCountry ( Country_SouthKorea );
  if ( ui->Spain_ShowData->isChecked() )         DrawCountry ( Country_Spain );
  if ( ui->Switzerland_ShowData->isChecked() )   DrawCountry ( Country_Switzerland );
  if ( ui->UnitedKingdom_ShowData->isChecked() ) DrawCountry ( Country_UK );
  if ( ui->USA_ShowData->isChecked() )           DrawCountry ( Country_USA );

  h_main_plot->SetMaximum( 1.5 * MaximumCases );
  h_main_plot->SetMinimum( 0 );
  
  UpdatePads();
  
}

void MainWindow::DrawCountry( Data_Per_Country *Country ) {
  
  Data_Per_Province * Country_Total = Country->Data_Total_All_Provinces;

  if ( ui->Plot_ShowConfirmed->isChecked() ) {
    Country_Total->h_cumulative_confirmed_shifted->Draw("same");
    MaximumCases = TMath::Max( (double) MaximumCases, Country_Total->h_cumulative_confirmed_shifted->GetMaximum() );
  }
  if ( ui->Plot_ShowDeaths   ->isChecked() ) {
    Country_Total->h_cumulative_deaths_shifted   ->Draw("same");
    MaximumCases = TMath::Max( (double) MaximumCases, Country_Total->h_cumulative_deaths_shifted->GetMaximum() );
  }
  if ( ui->Plot_ShowRecovered->isChecked() ) {
    Country_Total->h_cumulative_recovered_shifted->Draw("same");
    MaximumCases = TMath::Max( (double) MaximumCases, Country_Total->h_cumulative_recovered_shifted->GetMaximum() );
  }
  
}

// ------------------------------------------------------------------------------------ //
QString MainWindow::GetCurrentTimeStr() {

  QTime time = QTime::currentTime();

  int day, month, year;
  QDate date = QDate::currentDate();
  date.getDate(&year,&month,&day);

  QString timestr = time.toString("hh:mm");

  stringstream m_sx;
  m_sx.str("");
  m_sx << day << "/" << month << "/" << year << " ";

  QString datestr = QString::fromStdString(m_sx.str());

  return datestr+timestr;

}
/// CANVAS SETUP ------------------------------------------------------------------
void MainWindow::CreateCanvas()
{

  c_main = new QMainCanvas();
  c_main->resize(c_main->sizeHint());

  auto layout4 = new QGridLayout();
  layout4->setHorizontalSpacing(0);
  layout4->setVerticalSpacing(0);
  layout4->setContentsMargins(0,0,0,0);
  layout4->addWidget(c_main);
  ui->Plot_Main->setLayout(layout4);
  //c_time->resize(ui->timeFrame->size());

  //-----------------------------------------------------//
  c_legend = new QMainCanvas();
  c_legend->resize(c_legend->sizeHint());
  //    c_vmm->setWindowTitle("vmm-mon Statistics");
  //    c_vmm->setGeometry( 100, 100, 700, 500 );
  //    c_vmm->show();

  auto layout1 = new QGridLayout();
  layout1->setHorizontalSpacing(0);
  layout1->setVerticalSpacing(0);
  layout1->setContentsMargins(0,0,0,0);
  layout1->addWidget(c_legend);
  ui->Plot_Legend->setLayout(layout1);

}

void MainWindow::SetupCanvas()
{
    ///Divide dimensions are init'd here, and adjusted in treeSelectionChanged()
    //the canvas will have a line for every chip,board,or chamber
    //from the selected ones
    //    canvas_size_in_y = chips.size();
    //this is probably 4 for chips,boards, or chambers...to be seen
    //    canvas_size_in_x = Chip::getNoOfStatisticsHistos();

    //the histograms are created in the Chip class
    //since each of them refers to a chip
    //no functionality yet for histograms per chamber

    //let's make one canvas to rule them all
    //this canvas will be defined by what is selected in the gui
    //each line can be a chip, a board, a chamber

    //chip   :         hitmap,pdo,tdo,bcid + eventDisplay on itself
    //board  :combined hitmap,pdo,tdo,bcid + eventDisplay on itself
    //chamber:combined hitmap,pdo,tdo,bcid + eventDisplay on itself
    //so, each item=line will have 4 histos (dual, with the eventDisplay)

    c_main->Divide(1,1);
    c_legend->Divide(1,1);

    
}

void MainWindow::InitHistos()
{

  std::stringstream m_ss_date;
  
  h_main_plot = new TH1D("h_main_plot", "COVID 19 Cases vs Time For Each Country", 1,-0.5,0.5);
  h_main_plot->SetXTitle("Date");
  h_main_plot->SetYTitle("Number of Cases");
  h_main_plot->SetStats(kFALSE);     
  h_main_plot->SetBins( Country_USA->Data_Total_All_Provinces->Data_New_Cases_Per_Day->size()+10,0,
			Country_USA->Data_Total_All_Provinces->Data_New_Cases_Per_Day->size()+10 );

  //-----------------------------------------------//
  //          Set Label as Date
  //-----------------------------------------------//

  for ( uint iday=0; iday<Country_USA->Data_Total_All_Provinces->Data_Cumulative->size()+10; iday++ ) {

    if ( iday < Country_USA->Data_Total_All_Provinces->Data_Cumulative->size() ) {

      m_ss_date.str("");
      m_ss_date << Country_USA->Data_Total_All_Provinces->Data_Cumulative->at(iday)->day << "/"
		<< Country_USA->Data_Total_All_Provinces->Data_Cumulative->at(iday)->month;

      h_main_plot->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
    }
    else {

      m_ss_date.str("");

      h_main_plot->GetXaxis()->SetBinLabel( iday+1, m_ss_date.str().c_str() );
    }

  }

  //-------------------------------------------------//
  
}

void MainWindow::SetCountryPlotStyle( Data_Per_Country *Country, int kColor ) {

  Data_Per_Province * Country_Total = Country->Data_Total_All_Provinces;

  if ( ui->Plot_ShowConfirmed->isChecked() ) {
    Country_Total->h_cumulative_confirmed_shifted->SetLineColor( kColor );
    Country_Total->h_cumulative_confirmed_shifted->SetMarkerColor( kColor );
    Country_Total->h_cumulative_confirmed_shifted->SetMarkerSize( 0.5 );
    Country_Total->h_cumulative_confirmed_shifted->SetMarkerStyle( kFullSquare );
  }
  if ( ui->Plot_ShowDeaths   ->isChecked() ) {
    Country_Total->h_cumulative_deaths_shifted->SetLineColor( kColor );
    Country_Total->h_cumulative_deaths_shifted->SetMarkerColor( kColor );
    Country_Total->h_cumulative_deaths_shifted->SetMarkerSize( 0.5 );
    Country_Total->h_cumulative_deaths_shifted->SetMarkerStyle( kFullTriangleDown );
  }
  if ( ui->Plot_ShowRecovered->isChecked() ) {
    Country_Total->h_cumulative_recovered_shifted->SetLineColor( kColor );
    Country_Total->h_cumulative_recovered_shifted->SetMarkerColor( kColor );
    Country_Total->h_cumulative_recovered_shifted->SetMarkerSize( 0.5 );
    Country_Total->h_cumulative_recovered_shifted->SetMarkerStyle( kFullCircle );
  }

}
void MainWindow::UpdatePads()
{
    c_main->ModAndUpd_Pads();
    c_legend->ModAndUpd_Pads();
}
