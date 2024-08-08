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

int main(int argc, char* argv[]) {
    Application::init();
    Font segoe(L"Segoe UI");
    WindowClass wc(L"mywin");
    Window dlgDemo(wc, L"Demo window", 250, 350);

    Button btnGetListItem( dlgDemo, "Get listbox Item", 10, 10, 100, 30);
    Button btnGetComboItem(dlgDemo, "Get combo Item",  120, 10, 100, 30);
    Label label(dlgDemo, "Color:",           10, 58, 100, 30);
    Textbox txtName(dlgDemo, "", 140, 50, 80, 25);
    Listbox lbShape(dlgDemo, 10, 90, 210, 100);
    Combobox cbColor(dlgDemo, 60, 50, 70, 130);
    Progressbar progress(dlgDemo, 10, 190, 210, 14);
    Slider slider(dlgDemo, 10, 210, 210, 40);
    Scrollbar scrollbar(dlgDemo, 10, 260, 210, 20);

    scrollbar.setScrollInfo(0, 100, 30,10);
    

    scrollbar.setMessageHandler(dlgDemo, WM_HSCROLL,
        [&](WPARAM wParam) {
            scrollbar.defaultScrollHandler(wParam);
        }
    );

    slider.setRange(0, 20);
    slider.setValue(10);
  
    slider.setMessageHandler(dlgDemo, WM_HSCROLL,
        [&](WPARAM e) {
            int notificationCode = LOWORD(e); //eg TB_THUMBTRACK
            int value = HIWORD(e);
        }
    );

    progress.setRange(0, 100);
    progress.setValue(66);

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

    dlgDemo.setOnDestroyAction([]() { PostQuitMessage(0); });
    dlgDemo.show();


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
                std::cout << "Listbox selection changed" << std::endl;
            }
        }
    );

    cbColor.setCommand(dlgDemo,
        [&](int e) {
            if (e == CBN_SELCHANGE) {
                std::cout << "Combobox selection changed" << std::endl;
            }
        }
    );

    dlgDemo.show();
    Application::run();

    return 0;
}