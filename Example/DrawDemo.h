#pragma once
#include <WinGui\Application.h>
#include <WinGui\Window.h>
#include <WinGui\Font.h>
#include <WinGui\DeviceContext.h>
#include <string>
#include <memory>

class DrawDemo : public std::enable_shared_from_this<DrawDemo> {
    WindowClass wc;
    Window window;
    Font segoe;
    std::shared_ptr<DrawDemo> keepThisAliveUntilWindowIsDestroyed;

public:
    DrawDemo()
        : wc(L"DrawDemo")
        , window(wc, L"Demo window", 220, 220)
        , segoe(L"Segoe UI", 16)
    {
        //dlgDemo.setOnDestroyAction([]() { PostQuitMessage(0); });

        window.setMessageHandler(WM_PAINT,
            [this](WPARAM wparam, LPARAM lparam) {
                PAINTSTRUCT ps;                
                HDC hdc = BeginPaint(window, &ps);
                DeviceContext ctx(hdc);
                HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

                ctx.setBrush(hBrush);

                ctx.beginPath();
                ctx.moveTo(100, 10);
                ctx.lineTo(165, 110);
                ctx.lineTo(35, 110);
                ctx.closeFigure();
                ctx.endPath();                
                ctx.strokePath();

                ctx.setTextBackgroundTransparent(true);
                ctx.setFont(segoe);
                ctx.drawText(70, 70, L"Hello world");

                ctx.drawEllipse(35, 110, 55, 130);
                ctx.drawEllipse(145, 110, 165, 130);

                ctx.drawRectangle(20, 130, 180, 150);
                EndPaint(window, &ps);

                return true;
            }
        );
    }

    void keepAlive() {
        keepThisAliveUntilWindowIsDestroyed = shared_from_this();
        window.setOnDestroyAction([&]() { keepThisAliveUntilWindowIsDestroyed.reset(); });
    }

    void show() {      
        window.show();
    }
};