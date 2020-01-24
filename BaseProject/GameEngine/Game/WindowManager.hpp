//
//  WindowManager.hpp
//  BaseProject
//
//  Created by Daniel Harvey on 17/10/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#ifndef WindowManager_hpp
#define WindowManager_hpp
#include "Base.hpp"
#include <SFML/Graphics.hpp>
#include "Window.hpp"
#include "Game.hpp"
#include "Menu.hpp"
#include "Frustration.hpp"
#include "MyMenu.hpp"
#include <thread>
#include "FileLogger.hpp"
#include "SettingsManager.hpp"

class WindowManager : public Base
{
public:
    enum class windowType
    {
        eNull,
        eGame,
        eMenu
    };
    
    WindowManager() : log("0.5","GameLog.txt"), window(&log) {};
    ~WindowManager(){};
    
    void Start() override;
    void Update() override;
    void Exit() override;
    void Render(Window *window) override;
    void Input(std::queue<sf::Event> &events) override;
    
    void ChangeWindow(windowType wt);
    void UpdateWindow();
    void UpdateFromSettings(); 
    
private:
    std::shared_ptr<WindowHolder> currentWindow;
    Window window;
    windowType CurrentWindowType = windowType::eNull;
    ige::FileLogger log;
    SettingsManager Settings; 
};

#endif /* WindowManager_hpp */
