#include <WinGui\Application.h>

#include "DrawDemo.h"
#include "ControlsDemo.h"
#include "CommonControlsDemo.h"

int main(int argc, char* argv[]) {
    Application::init();

    WindowClass mainWindowClass(L"mainWindow");
    Window window(mainWindowClass, L"main window", 185, 170, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU );
    Font segoe(L"Segoe UI");
    Button btnStandardControls(window, L"Standard Controls",10, 10, 150,30);
    Button btnCommonControls(  window, L"Common Controls",  10, 50, 150, 30);
    Button btnDraw(            window, L"Draw",             10, 90, 150, 30);    

    btnStandardControls.setFont(segoe);
    btnCommonControls.setFont(segoe);
    btnDraw.setFont(segoe);

    btnStandardControls.setCommand(window,
        [&](int e) {
            if (e == BN_CLICKED) {
                auto controlsDemo = std::make_shared<ControlsDemo>();
                controlsDemo->keepAlive();
                controlsDemo->show();
            }
        }
    );

    btnCommonControls.setCommand(window,
        [&](int e) {
            if (e == BN_CLICKED) {
                auto commonControlsDemo = std::make_shared<CommonControlsDemo>();
                commonControlsDemo->keepAlive();
                commonControlsDemo->show();
            }
        }
    );

    btnDraw.setCommand(window,
        [&](int e) {
            if (e == BN_CLICKED) {
                auto drawDemo = std::make_shared<DrawDemo>();
                drawDemo->keepAlive();
                drawDemo->show();
            }
        }
    );

    window.setOnDestroyAction([]() { PostQuitMessage(0); });
    window.show();

    Application::run();

    return 0;
}