#pragma once
#include <WinGui\Application.h>
#include <WinGui\Window.h>
#include <WinGui\Button.h>
#include <WinGui\Listbox.h>
#include <WinGui\Textbox.h>
#include <WinGui\Combobox.h>
#include <WinGui\Label.h>
#include <WinGui\Progressbar.h>
#include <WinGui\Slider.h>
#include <WinGui\Scrollbar.h>
#include <WinGui\Toolbar.h>
#include <WinGui\ContextMenu.h>
#include <WinGui\DeviceContext.h>
#include <memory>

class CommonControlsDemo : public std::enable_shared_from_this<CommonControlsDemo> {
    WindowClass windowClass;
    Window window;
    Toolbar toolbar;
    Slider slider;
    Progressbar progressbar;
    ContextMenu contextMenu;
    std::shared_ptr<CommonControlsDemo> keepThisAliveUntilWindowIsDestroyed;

public:
    CommonControlsDemo() 
    : windowClass(L"CommonControlsDemo")
    , window(windowClass, L"Common Controls demo", 280, 180)
    , toolbar(window, 0, 0, 0, 0)
    , slider(window, 10, 50, 210, 40)
    , progressbar(window, 10, 100, 210, 14)
    {           
        contextMenu.addItem(window, L"Hello", [&](int e) { MessageBoxW(window.hwnd, L"Hello", L"Information", MB_OK); });
        contextMenu.addItem(window, L"World", [&](int e) { MessageBoxW(window.hwnd, L"World", L"Information", MB_OK); });        

        toolbar.buttonBuilder(window)
            .addButton(0, Toolbar::ButtonBuilder::Style::Button, L"Press", [&](int e) { MessageBoxW(window.hwnd, L"Press", L"Information", MB_OK); })
            .addButton(0, Toolbar::ButtonBuilder::Style::Check, L"Check", [&](int e) { MessageBoxW(window.hwnd, L"Check", L"Information", MB_OK); })
            .addDropdownMenu(0, L"Menu", 
                [&](int x, int y) {
                    contextMenu.track(window, x, y);
                }
            )            
            .addSeparator()
            .addButton(0, Toolbar::ButtonBuilder::Style::ButtonGroup, L"Choice1", [&](int e) { MessageBoxW(window.hwnd, L"Choice1", L"Information", MB_OK); })
            .addButton(0, Toolbar::ButtonBuilder::Style::ButtonGroup, L"Choice2", [&](int e) { MessageBoxW(window.hwnd, L"Choice2", L"Information", MB_OK); })
            .build();

        slider.setRange(0, 20);
        slider.setPosition(10);

        slider.setScrollHandler(window,
            [&](short notificationCode, short value) mutable {
                progressbar.setValue(slider.getPosition() * 5);
            }
        );

        progressbar.setRange(0, 100);
        progressbar.setValue(50);            
    }

    void keepAlive() {
        keepThisAliveUntilWindowIsDestroyed = shared_from_this();
        window.setOnDestroyAction([&]() { keepThisAliveUntilWindowIsDestroyed.reset(); });
    }

    void show() {
        window.show();
    }
};