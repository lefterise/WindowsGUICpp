#include "Window.h"
#include "Button.h"
#include "Listbox.h"
#include "Textbox.h"
#include "Label.h"
#include "Font.h"

//https://learn.microsoft.com/en-us/windows/win32/winmsg/windowing

Window::Window(WindowClass& windowClass, std::wstring title, int width, int height)
: app(app)
, windowClass(windowClass)
{
    auto handler = [this](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return windowHandler(hwnd, uMsg, wParam, lParam); };

    Application::setCreateHandler(handler);
    hwnd = CreateWindowExW(
        0,
        windowClass.getClassName(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        windowClass.getInstanceHandle(),
        NULL
    );
    Application::setCreateHandler(nullptr);

    if (hwnd == NULL) {
        throw std::exception("Window creation failed");
    }

    Application::registerWindowHandler(hwnd, handler);
}

void Window::show() {
    ShowWindow(hwnd, SW_SHOWNORMAL);
}

void Window::setOnDestroyAction(std::function<void()>&& action) {
    onDestroy = std::move(action);
}

std::shared_ptr<Button> Window::addButton(const char* label, int x, int y, int w, int h) {
    auto button = std::make_shared<Button>(*this, ++ids, label, x, y, w, h);
    return button;
}

std::shared_ptr<Listbox> Window::addListbox(int x, int y, int w, int h) {
    auto listbox = std::make_shared<Listbox>(*this, ++ids, x, y, w, h);
    return listbox;
}

std::shared_ptr<Label> Window::addLabel(const char* labeltext, int x, int y, int w, int h) {
    auto label = std::make_shared<Label>(*this, ++ids, labeltext, x, y, w, h);
    return label;
}

std::shared_ptr<Textbox> Window::addTextbox(const char* text, int x, int y, int w, int h) {
    auto textbox = std::make_shared<Textbox>(*this, ++ids, text, x, y, w, h);
    return textbox;
}

void Window::setMenuCommand(size_t id, std::function<void(int e)>&& action) {
    commands[id] = std::move(action);
}

LRESULT Window::windowHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
    {
        break;
    }
    case WM_COMMAND:
    {
        int commandId = LOWORD(wParam);
        int notificationCode = HIWORD(wParam);
        auto it = commands.find(commandId);
        if (it != commands.end()) {
            it->second(notificationCode);
            return 0;
        }
        break;
    }

    case WM_DESTROY:
        if (onDestroy) {
            onDestroy();
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}