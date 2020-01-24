//
//  WindowManager.cpp
//  BaseProject
//
//  Created by Daniel Harvey on 17/10/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#include "WindowManager.hpp"
#include <iostream>

void WindowManager::Start()
{
    Settings.Setup();
    window.Start(" ",Settings.MenuSize);
    ChangeWindow(windowType::eMenu);
}
void WindowManager::Update()
{
    while(currentWindow->Continue)
    {
        Settings.Setup();
        currentWindow->Update();
        if(currentWindow->Continue)
        {
            if(CurrentWindowType == windowType::eMenu)
                ChangeWindow(windowType::eGame);
            else if(CurrentWindowType == windowType::eGame)
                ChangeWindow(windowType::eMenu);
        }
    }
    log << "Exiting";
}
void WindowManager::Exit()
{
    currentWindow->Exit();
}
void WindowManager::Render(Window *window)
{

}
void WindowManager::Input(std::queue<sf::Event> &events)
{

}
void WindowManager::ChangeWindow(windowType wt)
{
    if(CurrentWindowType != wt)
    {
        CurrentWindowType = wt;
        UpdateFromSettings();
        switch (wt)
        {
            case windowType::eGame:
                currentWindow = std::make_shared<Frustration>(&log,&Settings);
                log << "Changing to Game";
                break;
            case windowType::eMenu:
                currentWindow = std::make_shared<MyMenu>(&log,&Settings);
                log << "Changing to Menu";
                break;
            default:
                break;
        }
        currentWindow->GiveWindow(&window);
        currentWindow->Start();
    }
}
void WindowManager::UpdateFromSettings()
{
    if(CurrentWindowType == windowType::eGame)
    {
        window.Start("Game", Settings.GameSize, Settings.Fullscreen);
    }
    else if(CurrentWindowType == windowType::eMenu)
    {
        window.Start("Menu", Settings.MenuSize);
    }
    window.SetVsync(Settings.Vsync);
}
