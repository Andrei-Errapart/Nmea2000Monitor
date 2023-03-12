#include <math.h>

#include <handyman/json.h>

#include "MainWindow.h"



// Message Map for the Scribble Window class
FXDEFMAP(MainWindow) MainWindowMap[] = {

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,  MainWindow::ID_CLOSE_ME,        MainWindow::onCmdCloseMe),
};

FXIMPLEMENT(MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap))


MainWindow::MainWindow()
	: m_port_textfield(nullptr)
{
}


long MainWindow::onCmdCloseMe(FXObject* sender, FXSelector, void* ptr)
{
	handyman::json::to_file(m_settings, m_settings_path);
	if (m_service) {
		m_service->stop();
	}
	getApp()->handle(sender, FXSEL(SEL_COMMAND, FXApp::ID_QUIT), ptr);
	return 1;
}

MainWindow::MainWindow(FXApp* a)
	: FXMainWindow(a, "Nmea 2000 Monitor", nullptr, nullptr, DECOR_ALL, 0, 0, 800, 600),
	m_startup_path(std::filesystem::current_path()),
	m_settings_path(std::filesystem::path(m_startup_path).append("Nmea2000Monitor.ini").string()),
	m_settings(handyman::json::from_file_or_default<MonitorSettings>(m_settings_path))
{
	m_port_target.connect(m_settings.Port);

	constexpr int	gb_opts = LAYOUT_SIDE_TOP | LAYOUT_TOP | LAYOUT_FILL_X | FRAME_RIDGE;

	// Our settings.
	{
		auto gb = new FXGroupBox(this, "Settings", gb_opts);
		FXMatrix* mtx = new FXMatrix(gb, 2, MATRIX_BY_COLUMNS, 0, 0, 0, 0);

		new FXLabel(mtx, "Port:");
		m_port_textfield = new FXTextField(mtx, 10, &m_port_target, FXDataTarget::ID_VALUE);
	}

	// ALT-F4 is missing by default, so add it.
	getAccelTable()->addAccel("Ctl-Q", this, FXSEL(SEL_COMMAND, ID_CLOSE_ME));
	getAccelTable()->addAccel("Alt-F4", this, FXSEL(SEL_COMMAND, ID_CLOSE_ME));

	// Start the timer...
	// a->addTimeout(this, ID_TIMER_READ, READ_TIMER_INTERVAL_NS, nullptr);
}

// Initialize
void MainWindow::create()
{
	// Create the windows
	FXMainWindow::create();

	// Make the main window appear
	show(PLACEMENT_SCREEN);

	try {
		m_service = std::make_unique<MonitorService>(m_settings);
	}
	catch (const std::exception& ex) {
		FXMessageBox::error(getApp(), MBOX_OK, "Error", ex.what());
	}
}

MainWindow::~MainWindow()
{
}
