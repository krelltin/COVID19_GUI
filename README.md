# COVID19_GUI
---
contact: **krell.tin@gmail.com and ssun@cern.ch** 
based on vmm-mon by Amilianos Koulouris 
https://gitlab.cern.ch/aikoulou/vmm-mon
aikoulou@cern.ch

Qt application to plot and fit COVID19 Data

## Pre-requisites:

0. Linux! 
1. Qt 5.5 or greater (recommended 5.7/5.9.9, see [here](https://download.qt.io/archive/qt/5.7/5.7.0/))
2. ROOT 6.16 (find it [here](https://root.cern.ch/content/release-61600))
3. COVID19 data from https://github.com/CSSEGISandData/COVID-19.git

```
cd COVID19_GUI
qmake COVID19_GUI.pro
make
./COVID19_GUI

```
