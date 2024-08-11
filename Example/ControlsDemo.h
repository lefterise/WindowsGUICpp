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

class ControlsDemo : public std::enable_shared_from_this<ControlsDemo>{
    Font segoe;
    WindowClass wc;
    Window dlgDemo;
    Button btnGetListItem;
    Button btnGetComboItem;    
    Textbox txtName;
    Listbox lbShape;
    Combobox cbColor;
    Label label;
    Scrollbar scrollbar;
    std::shared_ptr<ControlsDemo> keepThisAliveUntilWindowIsDestroyed;

public:
    ControlsDemo() 
    : segoe(L"Segoe UI")
    , wc(L"ControlsDemo")
    , dlgDemo(wc, L"Controls demo", 250, 280)
    , cbColor(dlgDemo, 10, 15, 100, 130)
    , btnGetComboItem(dlgDemo, "Get combo Item", 120, 10, 100, 30)    
    , lbShape(dlgDemo, 10, 50, 210, 100)
    , btnGetListItem(dlgDemo, "Get listbox Item", 10, 150, 100, 30)
    , txtName(dlgDemo, "", 120, 152, 100, 25)
    , label(dlgDemo, "--------Scroll------", 10, 190, 100, 18)
    , scrollbar(dlgDemo, 10, 210, 210, 20)
    {            
        scrollbar.setScrollInfo(0, 99, 30, 10);

        scrollbar.setScrollHandler(dlgDemo,
            [&](short notificationCode, short value) {
                int position = scrollbar.getPosition();                
                label.setText(std::to_string(position).c_str());               
            }

        );

        btnGetListItem.setFont(segoe);
        btnGetComboItem.setFont(segoe);
        label.setFont(segoe);
        txtName.setFont(segoe);
        lbShape.setFont(segoe);
        cbColor.setFont(segoe);

        lbShape.addItem("Circle");
        lbShape.addItem("Square");
        lbShape.setSelectedIndex(0);

        cbColor.addItem("Red");
        cbColor.addItem("Green");
        cbColor.setSelectedIndex(1);        

        btnGetListItem.setCommand(dlgDemo,
            [&](int e) {
                if (e == BN_CLICKED) {
                    size_t selIndex = lbShape.getSelectedIndex();
                    if (selIndex != -1) {
                        auto selectedItemText = lbShape.getItem(selIndex);
                        MessageBoxA(dlgDemo.hwnd, selectedItemText.c_str(), "Information", MB_OK);
                    }
                }
            }
        );

        btnGetComboItem.setCommand(dlgDemo,
            [&](int e) {
                if (e == BN_CLICKED) {
                    size_t selIndex = cbColor.getSelectedIndex();
                    if (selIndex != -1) {
                        auto selectedItemText = cbColor.getItem(selIndex);
                        MessageBoxA(dlgDemo.hwnd, selectedItemText.c_str(), "Information", MB_OK);
                    }
                }
            }
        );

        lbShape.setCommand(dlgDemo,
            [&](int e) {
                if (e == LBN_SELCHANGE) {
                    long index = lbShape.getSelectedIndex();
                    if (index != -1) {
                        auto selectedItemText = lbShape.getItem(index);
                        txtName.setText(selectedItemText.c_str());
                    }
                }
            }
        );

        cbColor.setCommand(dlgDemo,
            [&](int e) {
                if (e == CBN_SELCHANGE) {
                    long index = cbColor.getSelectedIndex();
                    if (index != -1) {
                        auto selectedItemText = cbColor.getItem(index);
                        txtName.setText(selectedItemText.c_str());
                    }
                }
            }
        );
    }

    void keepAlive() {
        keepThisAliveUntilWindowIsDestroyed = shared_from_this();
        dlgDemo.setOnDestroyAction([&]() { keepThisAliveUntilWindowIsDestroyed.reset(); });
    }

    void show() {
        dlgDemo.show();
    }

};


/*
HMENU hMenu = CreateMenu();
// Create the File submenu
HMENU hFileMenu = CreatePopupMenu();
AppendMenu(hFileMenu, MF_STRING, dlgDemo.getNextId(), L"&Open");
AppendMenu(hFileMenu, MF_STRING, dlgDemo.getNextId(), L"&Save");
AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
AppendMenu(hFileMenu, MF_STRING, dlgDemo.getNextId(), L"E&xit");

// Add the File submenu to the main menu
AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");

// Set the menu for the window
SetMenu(dlgDemo.hwnd, hMenu);
*/