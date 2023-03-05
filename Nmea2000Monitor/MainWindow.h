#pragma once
#include <stdint.h>

#include <fx.h>

#include <candle.h>

class MainWindow : public FX::FXMainWindow {
	FXDECLARE(MainWindow)
protected:
	MainWindow(); // not in use

private:
    void open_device_if_needed();
public:

    // Messages for our class
    enum {
        ID_TIMER_1SEC = FXMainWindow::ID_LAST,
        ID_TIMER_READ,
    };

    FXLabel*            m_device_label;
    FXLabel*            m_read_count_label;

    candle_handle       m_candle_handle;
    candle_list_handle  m_candle_list;
    int                 m_read_count;
    FXString            m_read_count_string;

public:
    long onTimer1Sec(FXObject*, FXSelector, void*);
    long onTimerRead(FXObject*, FXSelector, void*);

    // ScribbleWindow's constructor
    MainWindow(FXApp* a);

    // Initialize
    virtual void create();

    virtual ~MainWindow();
};
