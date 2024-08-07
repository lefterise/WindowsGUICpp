#pragma once
#include "WindowClass.h"
#include <exception>
#include <vector>
#include "Control.h"
//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa#remarks
class Button;
class Listbox;
class Textbox;
class Label;
class Combobox;
class Progressbar;

class Window {
public:
    Window(WindowClass& windowClass, std::wstring title, int width, int height);
    void show();
    void setOnDestroyAction(std::function<void()>&& action);
    std::shared_ptr<Button> addButton(const char* label, int x, int y, int w, int h);
    std::shared_ptr<Label> addLabel(const char* label, int x, int y, int w, int h);
    std::shared_ptr<Textbox> addTextbox(const char* text, int x, int y, int w, int h);
    std::shared_ptr<Listbox> addListbox(int x, int y, int w, int h);        
    std::shared_ptr<Combobox> addCombobox(int x, int y, int w, int h);
    std::shared_ptr<Progressbar> addProgressbar(int x, int y, int w, int h);

    void setTitle(const char* text);
private:
    friend Button;
    friend Listbox;
    friend Textbox;
    friend Label;
    friend Combobox;

    LRESULT windowHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void setMenuCommand(size_t id, std::function<void(int e)>&& action);
    Application& app;
    WindowClass& windowClass;    
    std::map<size_t, std::function<void(int)>> commands;
    std::function<void()> onDestroy;
    size_t ids = 0;
public: 
    HWND hwnd;
};