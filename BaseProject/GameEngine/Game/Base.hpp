//
//  Base.hpp
//  BaseProject
//
//  Created by Daniel Harvey on 13/09/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#ifndef Base_hpp
#define Base_hpp
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "Window.hpp"
#include <queue>

class Base
{
public:
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0 ;
    virtual void Render(Window *window) = 0;
    virtual void Input(std::queue<sf::Event> &events)  = 0;
    virtual void UI(){};
};

#endif /* Base_hpp */
