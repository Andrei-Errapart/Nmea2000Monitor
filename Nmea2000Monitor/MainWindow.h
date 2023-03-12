#pragma once

#include <filesystem>   // std::filesystem
#include <stdint.h>     // uint8_t
#include <string>       // std::string

#include <fx.h>

#include "MonitorSettings.h"

class MainWindow : public FX::FXMainWindow {
	FXDECLARE(MainWindow)
protected:
	MainWindow(); // not in use

private:
    std::filesystem::path   m_startup_path;
    std::string             m_settings_path;
    MonitorSettings m_settings;

public:

    // Messages for our class
    enum {
        ID_CLOSE_ME = FXMainWindow::ID_LAST,
    };

    FXTextField*            m_port_textfield;
    FXDataTarget            m_port_target;

public:
    long onCmdCloseMe(FXObject*, FXSelector, void*);

    // ScribbleWindow's constructor
    MainWindow(FXApp* a);

    // Initialize
    virtual void create();

    virtual ~MainWindow();
};
