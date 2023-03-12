#include "MainWindow.h"

#include <math.h>


// static void constexpr*  invalid_handle_value = (void*)(-1LL);
template <typename T>
static T invalid_value()
{
	return (T)(static_cast<intptr_t>(-1));
}

static constexpr FXTime READ_TIMER_INTERVAL_NS = 100 * 1000 * 1000;
static constexpr FXTime ONE_SECOND_NS = 1000 * 1000 * 1000;

// Message Map for the Scribble Window class
FXDEFMAP(MainWindow) MainWindowMap[] = {

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_TIMEOUT,  MainWindow::ID_TIMER_1SEC,        MainWindow::onTimer1Sec),
	FXMAPFUNC(SEL_TIMEOUT,  MainWindow::ID_TIMER_READ,        MainWindow::onTimerRead),
};

FXIMPLEMENT(MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap))


MainWindow::MainWindow()
	: m_device_label(nullptr),
	m_read_count_label(nullptr),
	m_candle_handle(invalid_value<candle_handle>()),
	m_candle_list(invalid_value<candle_list_handle>()),
	m_read_count(0)
{
}

void MainWindow::open_device_if_needed() 
{
	if (m_candle_handle != invalid_value<candle_handle>()) {
		return;
	}

	try {
		candle_device_mode_flags_t device_mode_flags = CANDLE_MODE_HW_TIMESTAMP;
		const unsigned int device_number = 0;
		const unsigned int device_channel = 0;
		const unsigned int bitrate = 250000;
		const candle_err_t err = candle_init_single_device(device_number, device_channel, bitrate, device_mode_flags, &m_candle_list, &m_candle_handle);

		if (err != CANDLE_ERR_OK) {
			const char* text = candle_error_text(err);
			if (text == nullptr || text[0] == 0) {
				FXString s;
				s.format("Error code: %d", static_cast<int>(err));
			}
			else {
				m_device_label->setText(text);
			}
			m_candle_list = invalid_value<candle_handle>();
			m_candle_handle = invalid_value<candle_list_handle>();
			return;
		}

		m_device_label->setText("Opened");
	}
	catch (const std::exception& ex) {
		m_candle_list = invalid_value<candle_handle>();
		m_candle_handle = invalid_value<candle_list_handle>();
		m_device_label->setText(ex.what());
	}
}


long MainWindow::onTimer1Sec(FXObject*, FXSelector, void*)
{
	open_device_if_needed();
	getApp()->addTimeout(this, ID_TIMER_1SEC, ONE_SECOND_NS, nullptr);
	return 1;
}

long MainWindow::onTimerRead(FXObject*, FXSelector, void*)
{
	if (m_candle_handle != invalid_value<candle_handle>()) {
		try {
			candle_frame_t frame;

			unsigned int this_round_count = 0;
			while (candle_frame_read(m_candle_handle, &frame, 0)) {
				++this_round_count;
			}
			const candle_err_t err = candle_dev_last_error(m_candle_handle);
			if (err != CANDLE_ERR_READ_TIMEOUT && err != CANDLE_ERR_OK) {
				m_device_label->setText(candle_error_text(err));
			}

			if (this_round_count > 0u) {
				m_read_count += this_round_count;
				m_read_count_string.format("%d", m_read_count);
				m_read_count_label->setText(m_read_count_string);
			}
		}
		catch (const std::exception& ex) {
			m_device_label->setText(ex.what());
		}
	}
	getApp()->addTimeout(this, ID_TIMER_READ, READ_TIMER_INTERVAL_NS, nullptr);
	return 1;
}

MainWindow::MainWindow(FXApp* a)
	: FXMainWindow(a, "Nmea 2000 Monitor", nullptr, nullptr, DECOR_ALL, 0, 0, 800, 600),
	m_device_label(nullptr),
	m_read_count_label(nullptr),
	m_candle_handle(invalid_value<candle_handle>()),
	m_candle_list(invalid_value<candle_list_handle>()),
	m_read_count(0)
{
	// Our status.
	{
		FXMatrix* mtx = new FXMatrix(this, 2, MATRIX_BY_COLUMNS, 0, 0, 0, 0);

		new FXLabel(mtx, "Device:");
		m_device_label = new FXLabel(mtx, "not opened");
		new FXLabel(mtx, "Read count:");
		m_read_count_label = new FXLabel(mtx, "0");
	}



	// ALT-F4 is missing by default, so add it.
	getAccelTable()->addAccel("Ctl-Q", getApp(), FXSEL(SEL_COMMAND, FXApp::ID_QUIT));
	getAccelTable()->addAccel("Alt-F4", getApp(), FXSEL(SEL_COMMAND, FXApp::ID_QUIT));

	// Start the timer...
	a->addTimeout(this, ID_TIMER_READ, READ_TIMER_INTERVAL_NS, nullptr);
	a->addTimeout(this, ID_TIMER_1SEC, ONE_SECOND_NS, nullptr);
	open_device_if_needed();
}

// Initialize
void MainWindow::create()
{
	// Create the windows
	FXMainWindow::create();

	// Make the main window appear
	show(PLACEMENT_SCREEN);

	open_device_if_needed();
}

MainWindow::~MainWindow()
{
}
