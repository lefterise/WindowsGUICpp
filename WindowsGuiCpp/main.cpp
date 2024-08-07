#include "Application.h"
#include "Window.h"
#include "Button.h"
#include "Listbox.h"
#include "Textbox.h"
#include "Label.h"

int main(int argc, char* argv[]) {    
    WindowClass wc(L"StandardWindow");
    Window window(wc, L"My window", 250, 240);
    Font segoe(L"Segoe UI");
    window.setOnDestroyAction([]() { PostQuitMessage(0); });

    auto b1 = window.addButton("Button 1", 10, 10, 100, 30);
    auto b2 = window.addButton("Button 2", 120, 10, 100, 30);
    auto label = window.addLabel("Label 1:", 10, 58, 100, 30);
    auto tb = window.addTextbox("", 120, 50, 100, 30);
    auto lb = window.addListbox(10, 90, 210, 100);

    label->setFont(segoe);
    b1->setFont(segoe);
    b2->setFont(segoe);    
    tb->setFont(segoe);    
    lb->setFont(segoe);

    lb->addItem("Item 1");
    lb->addItem("Item 2");
    lb->addItem("Item 3");

    b1->setCommand([&](int e) {
        if (e == BN_CLICKED) {
            size_t selIndex = lb->getSelectedItem();
            if (selIndex != -1) {
                auto selItem = lb->getItem(selIndex);
                MessageBoxA(window.hwnd, selItem.c_str(), "Information", MB_OK | MB_ICONINFORMATION);
            }
        }
    });
    
    lb->setCommand([&window](int e) {
        if (e == LBN_SELCHANGE) {
            std::cout << "Listbox selection changed"<< std::endl;
        }        
    });
    window.show();
    Application::run();

    return 0;
}