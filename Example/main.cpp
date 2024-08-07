#include <WinGui\Application.h>
#include <WinGui\Window.h>
#include <WinGui\Button.h>
#include <WinGui\Listbox.h>
#include <WinGui\Textbox.h>
#include <WinGui\Combobox.h>
#include <WinGui\Label.h>

int main(int argc, char* argv[]) {
    Font segoe(L"Segoe UI");
    WindowClass wc(L"mywin");
    Window dlgDemo(wc, L"Demo window", 250, 240);

    auto btnGetListItem = dlgDemo.addButton("Get listbox Item", 10, 10, 100, 30);
    auto btnGetComboItem = dlgDemo.addButton("Get combo Item", 120, 10, 100, 30);
    auto label = dlgDemo.addLabel("Color:", 10, 58, 100, 30);
    auto txtName = dlgDemo.addTextbox("", 140, 50, 80, 25);
    auto lbShape = dlgDemo.addListbox(10, 90, 210, 100);
    auto cbColor = dlgDemo.addCombobox(60, 50, 70, 130);

    btnGetListItem->setFont(segoe);
    btnGetComboItem->setFont(segoe);
    label->setFont(segoe);
    txtName->setFont(segoe);
    lbShape->setFont(segoe);
    cbColor->setFont(segoe);

    lbShape->addItem("Circle");
    lbShape->addItem("Square");

    cbColor->addItem("Red");
    cbColor->addItem("Green");

    dlgDemo.setOnDestroyAction([]() { PostQuitMessage(0); });
    dlgDemo.show();


    btnGetListItem->setCommand(
        [&](int e) {
            if (e == BN_CLICKED) {
                size_t selIndex = lbShape->getSelectedItem();
                if (selIndex != -1) {
                    auto selectedItemText = lbShape->getItem(selIndex);
                    MessageBoxA(dlgDemo.hwnd, selectedItemText.c_str(), "Information", MB_OK);
                }
            }
        }
    );

    btnGetComboItem->setCommand(
        [&](int e) {
            if (e == BN_CLICKED) {
                size_t selIndex = cbColor->getSelectedItem();
                if (selIndex != -1) {
                    auto selectedItemText = cbColor->getItem(selIndex);
                    MessageBoxA(dlgDemo.hwnd, selectedItemText.c_str(), "Information", MB_OK);
                }
            }
        }
    );

    lbShape->setCommand(
        [&](int e) {
            if (e == LBN_SELCHANGE) {
                std::cout << "Listbox selection changed" << std::endl;
            }
        }
    );

    cbColor->setCommand(
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