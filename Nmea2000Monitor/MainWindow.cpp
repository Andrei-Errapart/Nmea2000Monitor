
#include "MainWindow.h"


#include <math.h>

// Message Map for the Scribble Window class
FXDEFMAP(MainWindow) MainWindowMap[] = {

	//________Message_Type_____________________ID____________Message_Handler_______
    FXMAPFUNC(SEL_COMMAND,  MainWindow::ID_FIRST,             MainWindow::onCmdFirst),
};

FXIMPLEMENT(MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap))


MainWindow::MainWindow()
{
}


long MainWindow::onCmdFirst(FXObject*, FXSelector, void*)
{
	return 1;
}


MainWindow::MainWindow(FXApp* a)
	: FXMainWindow(a, "Nmea 2000 Monitor", nullptr, nullptr, DECOR_ALL, 0, 0, 800, 600)
{

}

// Initialize
void MainWindow::create()
{
	// Create the windows
	FXMainWindow::create();
	// Make the main window appear
	show(PLACEMENT_SCREEN);

}

MainWindow::~MainWindow()
{
}
