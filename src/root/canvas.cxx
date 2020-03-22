#include <QLayout>
#include <QTimer>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QMessageBox>

#include <stdlib.h>

#include <TCanvas.h>
#include <TVirtualX.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1.h>
#include <TFrame.h>
#include <TTimer.h>
#include "canvas.h"

//______________________________________________________________________________
QRootCanvas::QRootCanvas(QWidget *parent) : QWidget(parent, 0), fCanvas(0)
{
    // QRootCanvas constructor.

    // set options needed to properly update the canvas when resizing the widget
    // and to properly handle context menus and mouse move events
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setMinimumSize(300, 200);
    setUpdatesEnabled(kFALSE);
    setMouseTracking(kTRUE);

    // register the QWidget in TVirtualX, giving its native window id
    int wid = gVirtualX->AddWindow((ULong_t)winId(), 600, 400);
    // create the ROOT TCanvas, giving as argument the QWidget registered id
    fCanvas = new TCanvas("Root Canvas", width(), height(), wid);
}

//______________________________________________________________________________
void QRootCanvas::mouseMoveEvent(QMouseEvent *e)
{
    // Handle mouse move events.

    if (fCanvas) {
        if (e->buttons() & Qt::LeftButton) {
            fCanvas->HandleInput(kButton1Motion, e->x(), e->y());
        } else if (e->buttons() & Qt::MidButton) {
            fCanvas->HandleInput(kButton2Motion, e->x(), e->y());
        } else if (e->buttons() & Qt::RightButton) {
            fCanvas->HandleInput(kButton3Motion, e->x(), e->y());
        } else {
            fCanvas->HandleInput(kMouseMotion, e->x(), e->y());
        }
    }
}

//______________________________________________________________________________
void QRootCanvas::mousePressEvent( QMouseEvent *e )
{
    // Handle mouse button press events.

    if (fCanvas) {
        switch (e->button()) {
        case Qt::LeftButton :
            fCanvas->HandleInput(kButton1Down, e->x(), e->y());
            break;
        case Qt::MidButton :
            fCanvas->HandleInput(kButton2Down, e->x(), e->y());
            break;
        case Qt::RightButton :
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true)
            // seems to cure the problem
            fCanvas->HandleInput(kButton3Down, e->x(), e->y());
            break;
        default:
            break;
        }
    }
}

//______________________________________________________________________________
void QRootCanvas::mouseReleaseEvent( QMouseEvent *e )
{
    // Handle mouse button release events.

    if (fCanvas) {
        switch (e->button()) {
        case Qt::LeftButton :
            fCanvas->HandleInput(kButton1Up, e->x(), e->y());
            break;
        case Qt::MidButton :
            fCanvas->HandleInput(kButton2Up, e->x(), e->y());
            break;
        case Qt::RightButton :
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true)
            // seems to cure the problem
            fCanvas->HandleInput(kButton3Up, e->x(), e->y());
            break;
        default:
            break;
        }
    }
}

//______________________________________________________________________________
void QRootCanvas::resizeEvent( QResizeEvent * )
{
    // Handle resize events.

    if (fCanvas) {
        fCanvas->Resize();
        fCanvas->Update();
    }
}

//______________________________________________________________________________
void QRootCanvas::paintEvent( QPaintEvent * )
{
    // Handle paint events.

    if (fCanvas) {
//        std::cout << "TEST"<<std::endl;
        fCanvas->Resize();
        fCanvas->Update();
    }
}

//------------------------------------------------------------------------------

//______________________________________________________________________________
QMainCanvas::QMainCanvas(QWidget *parent) : QWidget(parent)
{
    //make simple layout for the single canvas
    QVBoxLayout *l = new QVBoxLayout(this);
    //add canvas widget to the layout
    l->addWidget(canvas = new QRootCanvas(this));
    //this timer is very important
    //every time it times-out, it tells the
    //root engine to process the events.
    fRootTimer = new QTimer( this );
    QObject::connect( fRootTimer, SIGNAL(timeout()), this, SLOT(handle_root_events()) );
    fRootTimer->start( 20 );
    //set dimensions of stats box (~legend) in hists
    gStyle->SetStatW(0.3);
    gStyle->SetStatH(0.4);
    //make histo title larger
//    gStyle->SetTitleFontSize(0.1);
//    gStyle->SetLabelSize(0.9);
}
//______________________________________________________________________________
void QMainCanvas::handle_root_events()
{
    //call the inner loop of ROOT
    gSystem->ProcessEvents();
}
//______________________________________________________________________________
void QMainCanvas::changeEvent(QEvent * e)
{
    if (e->type() == QEvent ::WindowStateChange) {
        QWindowStateChangeEvent * event = static_cast< QWindowStateChangeEvent * >( e );
        if (( event->oldState() & Qt::WindowMaximized ) ||
                ( event->oldState() & Qt::WindowMinimized ) ||
                ( event->oldState() == Qt::WindowNoState &&
                  this->windowState() == Qt::WindowMaximized )) {
            if (canvas->getCanvas()) {
                canvas->getCanvas()->Resize();
                canvas->getCanvas()->Update();
            }
        }
    }
}
//______________________________________________________________________________
void QMainCanvas::ModAndUpd()
{
    canvas->getCanvas()->Modified();
    canvas->getCanvas()->Update();
}

void QMainCanvas::ModAndUpd_Pads()
{
    //This method calls Mod and Upd
    //For each pad on the canvas.
    ///To be used only for canvases that have been Divide'd
    for(int i=1;i<=noOfPads;i++)
    {
        canvas->getCanvas()->cd(i);//pads or i
        gPad->Modified();
        gPad->Update();

    }
}

void QMainCanvas::Divide(int x,int y)
{
    canvas->getCanvas()->Divide(x,y);
    noOfPads=x*y;


}

void QMainCanvas::cd(int x)
{
    canvas->getCanvas()->cd(x);
}

void QMainCanvas::clear()
{
    canvas->getCanvas()->Clear();
}

void QMainCanvas::SaveAs(QString sname)
{
    canvas->getCanvas()->SaveAs(sname.toUtf8().data());
}
