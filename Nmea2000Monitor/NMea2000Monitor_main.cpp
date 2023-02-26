#include <fx.h>

#include "MainWindow.h"

int main(int argc,
	char** argv)
{
	// Make application
	FXApp application("Nmea2000Monitor", "Andrei Errapart");

	// Start app
	application.init(argc, argv);

	// Scribble window
	new MainWindow(&application);

	// Create the application's windows
	application.create();

	// Run the application
	return application.run();
}
