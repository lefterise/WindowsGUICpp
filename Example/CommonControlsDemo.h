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
#include <WinGui\ImageList.h>
#include <WinGui\Treeview.h>
#include <WinGui\TabControl.H>
#include <memory>
#include "resource.h"

class CommonControlsDemo : public std::enable_shared_from_this<CommonControlsDemo> {
    WindowClass windowClass;
    Window window;
    ImageList imagelist;
    Toolbar toolbar;
    Slider slider;
    Progressbar progressbar;
    Treeview treeview;
    ContextMenu contextMenu;
    Font segoe;
    Label label;
    TabControl tabcontrol;
    std::shared_ptr<CommonControlsDemo> keepThisAliveUntilWindowIsDestroyed;

public:
    CommonControlsDemo()
        : windowClass(L"CommonControlsDemo")
        , window(windowClass, L"Common Controls demo", 240, 360)
        , imagelist(32, 32)
        , toolbar(window, 0, 0, 0, 64)
        , slider(window, 10, 70, 210, 40)
        , progressbar(window, 10, 120, 200, 14)
        , treeview(window, 20, 172, 180, 120)
        , segoe(L"Segoe UI")
        , label(window, L"This will show treeview selection", 10, 300, 210, 16)
        , tabcontrol(window,10,140,200,160)
    {           
        contextMenu.addItem(window, L"Hello", [&](int e) { MessageBoxW(window.hwnd, L"Hello", L"Information", MB_OK); });
        contextMenu.addItem(window, L"World", [&](int e) { MessageBoxW(window.hwnd, L"World", L"Information", MB_OK); });        

        toolbar.buttonBuilder(window)
            .addButton(0, Toolbar::ButtonBuilder::Style::Button, L"Press", [&](int e) { MessageBoxW(window.hwnd, L"Press", L"Information", MB_OK); })
            .addButton(2, Toolbar::ButtonBuilder::Style::Check, L"Check", [&](int e) { MessageBoxW(window.hwnd, L"Check", L"Information", MB_OK); })
            .addDropdownMenu(1, L"Menu", 
                [&](int x, int y) {
                    contextMenu.track(window, x, y);
                }
            )            
            .addSeparator()
            .addButton(3, Toolbar::ButtonBuilder::Style::ButtonGroup, L"Choice1", [&](int e) { MessageBoxW(window.hwnd, L"Choice1", L"Information", MB_OK); })
            .addButton(4, Toolbar::ButtonBuilder::Style::ButtonGroup, L"Choice2", [&](int e) { MessageBoxW(window.hwnd, L"Choice2", L"Information", MB_OK); })
            .build();

        auto hInstance = GetModuleHandle(NULL);
        //icons from: https://shlyapnikova.deviantart.com
        imagelist.addBitmap(hInstance, IDB_BELL, 0xFFFFFF);
        imagelist.addBitmap(hInstance, IDB_BRIEFCASE, 0xFFFFFF);
        imagelist.addBitmap(hInstance, IDB_PEOPLE, 0xFFFFFF);
        imagelist.addBitmap(hInstance, IDB_BULB, 0xFFFFFF);
        imagelist.addBitmap(hInstance, IDB_BULB_OFF, 0xFFFFFF);
        
        treeview.setImageList(imagelist);
        toolbar.setImageList(imagelist);


        auto root = treeview.addItem(NULL, L"Root", 2, 2);
        auto child1 = treeview.addItem(root, L"Child 1", 1, 1);
        auto child2 = treeview.addItem(root, L"Child 2", 4, 3);

        treeview.expand(root, true);
        treeview.setSelectionChangeListener(window, [this, root, child1, child2](LPNMTREEVIEW l) { //note we capture the handles by value since they are local and would get out of scope
            auto selectedItem = treeview.getSelectedItem();

            if (l->itemNew.hItem == root)
                label.setText(L"Root");

            if (l->itemNew.hItem == child1)
                label.setText(L"child1");

            if (l->itemNew.hItem == child2)
                label.setText(L"child2");
        });

        tabcontrol.setFont(segoe);
        tabcontrol.addTab(L"Tab 1",0);
        tabcontrol.addTab(L"Tab 2",1);
        tabcontrol.setActiveTab(0);

        tabcontrol.setTabChangeListener(window, [this]() {
            int tab = tabcontrol.getCurrentTab();

            treeview.setVisible(tab == 0);
        });

        label.setFont(segoe);

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