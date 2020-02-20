//
//  Piece.cpp
//  MyPetridish
//
//  Created by Daniel Harvey on 08/01/2020.
//  Copyright © 2020 Daniel Harvey. All rights reserved.
//

#include "Piece.hpp"
Piece::Piece()
{
    
}
void Piece::Setup(PieceColour pt, int vp)
{
    inPlay = false;
    type = pt;
    VecPos = vp; 
    switch (pt)
    {
        case PT_red:
            Colour = sf::Color::Red; 
            break;
        case PT_green:
            Colour = sf::Color(0,128,0);
            break;
        case PT_blue:
            Colour = sf::Color::Blue;
            break;
        case PT_yellow:
            Colour = sf::Color::Yellow;
            break;
    }
    
    P_circle.setPointCount(10);
    P_circle.setRadius(radius);
    P_circle.setOrigin(radius/2, radius/2);
    P_circle.setFillColor(Colour);
    P_circle.setOutlineThickness(2.5);
    P_circle.setOutlineColor(sf::Color(64,64,64));    
}
PieceColour Piece::getType()
{
    return type;
}
int Piece::getVecPos()
{
    return VecPos;
}
sf::CircleShape* Piece::getCircle()
{
    return &P_circle;
}
void Piece::SetPosition(sf::Vector2f pos,bool SetStart)
{
    P_circle.setPosition(pos);
    if(SetStart)
        StartPosition = pos; 
}
void Piece::BringIntoPlay(bool play)
{
    RecentlyInplay = play; 
    inPlay = play;
    if(!play)
        SetPosition(StartPosition);
}
