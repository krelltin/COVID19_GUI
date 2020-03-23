#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//local

#include "canvas.h"
#include "Data_Analyzer.h"
#include "Data_Per_Country.h"

#include <TMath.h>
#include <TString.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TPad.h>

//QT includes
#include <QMainWindow>
#include <QUdpSocket>
#include <QWidget>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDate>
#include <QFileDialog>
//C++
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

#include <cstdint>
#include <stdint.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Data_Analyzer * dAna;
    
    Data_Per_Country * Country_USA;
    Data_Per_Country * Country_China;
    Data_Per_Country * Country_Italy;
    Data_Per_Country * Country_Iran;
    Data_Per_Country * Country_Japan;
    Data_Per_Country * Country_Germany;
    Data_Per_Country * Country_France;
    Data_Per_Country * Country_UK;
    Data_Per_Country * Country_Spain;
    Data_Per_Country * Country_Switzerland;
    Data_Per_Country * Country_SouthKorea;
    
private:

    bool verbose;
    
    void CreateCanvas();
    void SetupCanvas();
    
    QString GetCurrentTimeStr();

    void DrawCountry( Data_Per_Country *Country );
    
    void InitReader();
    void InitHistos();

    void SetCountryPlotStyle( Data_Per_Country * Country, int kColor );
    void UpdatePads();

    void ShiftCountryNDays( Data_Per_Country * Country, int shift_ndays, int n_future_days );

    void AddLegend( TLegend * legend, Data_Per_Country * country,
		    int shift_ndays,
		    bool plot_confirmed,
		    bool plot_deaths,
		    bool plot_recovered );
    
    void DrawLegends();
    
    TH1D *h_main_plot;

    TLegend * Legend_China;
    TLegend * Legend_France;
    TLegend * Legend_Germany;
    TLegend * Legend_Iran;
    TLegend * Legend_Italy;
    TLegend * Legend_Japan;
    TLegend * Legend_SKorea;
    TLegend * Legend_Spain;
    TLegend * Legend_Swiss;
    TLegend * Legend_UK;
    TLegend * Legend_USA;
    
    int MaximumCases;
    
    //--------------------------------

    Ui::MainWindow *ui;

    QMainCanvas* c_main;
    QMainCanvas* c_legend;

private slots:

    /// DATA HANDLING
  void DrawHist();
  void ShiftNDays();
    //void AssumeExpoNDays();

    /// DRAWING FUNCTIONS


};
#endif // MAINWINDOW_H
