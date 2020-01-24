//
//  MyGame.hpp
//  BaseProject
//
//  Created by Daniel Harvey on 03/11/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#ifndef MyGame_hpp
#define MyGame_hpp
#include <iostream>
#include <stdio.h>
#include "Game.hpp"
#include "Piece.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include <array>
class Frustration : public Game
{
public:
    Frustration(ige::FileLogger *LOG,SettingsManager *SM) ;
    void Start() override;
    void Input(std::queue<sf::Event> &events) override;
    void Render(Window *window) override;
    void UI() override;
    void EarlyUpdate() override;
    void LateUpdate() override;
    
private:
    std::array<Piece, 16> pieces;
    std::array<Player, 4> Players;
    Frustration_Board board; 
    int currentPlayer = 0;
    bool gotonextplayer = false;
    int roll = 0;
    int temproll = 0;
    std::array<sf::Texture,6> diceTextures; 
};

#endif /* MyGame_hpp */
