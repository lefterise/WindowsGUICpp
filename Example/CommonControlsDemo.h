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
        contextMenu.addItem(window, L"Hello", [&](int e) { MessageBoxA(window.hwnd, "Hello", "Information", MB_OK); });
        contextMenu.addItem(window, L"World", [&](int e) { MessageBoxA(window.hwnd, "World", "Information", MB_OK); });        

        toolbar.buttonBuilder(window)
            .addButton(0, Toolbar::ButtonBuilder::Style::Button, "Press", [&](int e) { MessageBoxA(window.hwnd, "Press", "Information", MB_OK); })
            .addButton(0, Toolbar::ButtonBuilder::Style::Check, "Check", [&](int e) { MessageBoxA(window.hwnd, "Check", "Information", MB_OK); })
            .addDropdownMenu(0, "Menu", 
                [&](int x, int y) {
                    contextMenu.track(window, x, y);
                }
            )            
            .addSeparator()
            .addButton(0, Toolbar::ButtonBuilder::Style::ButtonGroup, "Choice1", [&](int e) { MessageBoxA(window.hwnd, "Choice1", "Information", MB_OK); })
            .addButton(0, Toolbar::ButtonBuilder::Style::ButtonGroup, "Choice2", [&](int e) { MessageBoxA(window.hwnd, "Choice2", "Information", MB_OK); })
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