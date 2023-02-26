#pragma once

#include <fx.h>

class MainWindow : public FX::FXMainWindow {
	FXDECLARE(MainWindow)
protected:
	MainWindow(); // not in use
public:

    // Messages for our class
    enum {
        ID_FIRST = FXMainWindow::ID_LAST,
    };

public:
    long onCmdFirst(FXObject*, FXSelector, void*);

    // ScribbleWindow's constructor
    MainWindow(FXApp* a);

    // Initialize
    virtual void create();

    virtual ~MainWindow();
};
