//
//  Board.cpp
//  MyPetridish
//
//  Created by Daniel Harvey on 08/01/2020.
//  Copyright © 2020 Daniel Harvey. All rights reserved.
//

#include "Board.hpp"
#include <iostream>
#include <math.h>
#include "Window.hpp"
#include "SfMath.hpp"
/*
 switch (p->getType())
 {
     case PT_blue:
         break;
     case PT_red:
         break;
     case PT_green:
        break;
     case PT_yellow:
        break;
     default:
         break;
 }
 */
Frustration_Board::~Frustration_Board()
{
    delete[] pieces; 
}
void Frustration_Board::GivePieces(std::array<Piece,16> *p)
{
    pieces = p;
}
bool Frustration_Board::MovePiece(Piece *p,int slotNum)
{
    GameBoardSlots[slotNum].currentPiece = p->getVecPos();
    p->SetPosition(GameBoardSlots[slotNum].Position);
    if(p->getType() == PT_red)
    {
        p->distanceToend = GameBoardSlots[slotNum].DistanceToEnd_R;
    }
    else if(p->getType() == PT_green)
    {
        p->distanceToend = GameBoardSlots[slotNum].DistanceToEnd_G;
    }
    else if(p->getType() == PT_blue)
    {
        p->distanceToend = GameBoardSlots[slotNum].DistanceToEnd_B;
    }
    else if(p->getType() == PT_yellow)
    {
        p->distanceToend = GameBoardSlots[slotNum].DistanceToEnd_Y;
    }
    return true;
}
bool Frustration_Board::TryMovePiece(Piece *p, int diceroll)
{
    bool SUCCESS = false;
    if(!p->inPlay)
    {
        std::cout<<"piece not in play\n";
        if(diceroll != 6)
            return false;
        
        p->BringIntoPlay(true);

        for(int i{0}; i<GameBoardSlots.size();i++)
        {
            if(GameBoardSlots[i].isStartArrow && GameBoardSlots[i].type == p->getType())
            {
                std::cout<<"piece not in play - found arrow to move to of the correct type\n";
                if(GameBoardSlots[i].currentPiece == -1)
                {
                    
                    SUCCESS = MovePiece(p,i);
                }
                else
                {
                    //Slot is taken, remove other piece if it is not the players own
                    if(pieces->at(GameBoardSlots[i].currentPiece).getType() != p->getType())
                    {
                        pieces->at(GameBoardSlots[i].currentPiece).BringIntoPlay(false);
                        SUCCESS = MovePiece(p,i);
                    }
                    else
                    {
                        //the piece is from the player that was trying to move it
                        //reject the move
                        SUCCESS = false;
                    }
                }
                break;
            }
        }
        if(SUCCESS)
        {
            for(int s{0}; s<StartBoardSlots.size(); s++)
            {
                if(StartBoardSlots[s].currentPiece == p->getVecPos())
                {
                    StartBoardSlots[s].currentPiece = -1;
                }
            }
        }
    } 
    else
    {
        std::cout<<"piece is in play\n";
        int dist = p->distanceToend - diceroll;
        bool R{0},G{0},B{0},Y{0};
        int oldslotnum = 0;

        if(dist >= 0 && dist <= GameBoardSlots.size())
        {
            for(int i{0}; i<GameBoardSlots.size();i++)
            {
                if(GameBoardSlots[i].currentPiece == p->getVecPos())
                {
                    oldslotnum = i;
                }
                 if((p->getType() == PT_blue && GameBoardSlots[i].DistanceToEnd_B == dist) ||
                    (p->getType() == PT_green && GameBoardSlots[i].DistanceToEnd_G == dist) ||
                    (p->getType() == PT_red && GameBoardSlots[i].DistanceToEnd_R == dist) ||
                    (p->getType() == PT_yellow && GameBoardSlots[i].DistanceToEnd_Y == dist) )
                 {
                     if(GameBoardSlots[i].currentPiece == -1)
                     {
                         //slot is empty, move p to slot
                         SUCCESS = MovePiece(p,i);
                     }
                     else
                     {
                         //Slot is taken, remove other piece if it is not the players own
                         if(pieces->at(GameBoardSlots[i].currentPiece).getType() != p->getType())
                         {
                             pieces->at(GameBoardSlots[i].currentPiece).BringIntoPlay(false);
                             SUCCESS = MovePiece(p,i);
                         }
                         else
                         {
                             //the piece is from the player that was trying to move it
                             //reject the move
                             SUCCESS = false;
                         }
                     }
                 }
            }
        }
        else
            std::cout<<"dist = " << dist <<"\n";
        
        if(SUCCESS)
        {
            GameBoardSlots[oldslotnum].currentPiece = -1;
        }
    }
    return SUCCESS;
}
void Frustration_Board::Setup(int numSlotsPerPlayer,int numPlayerPieces, int numPlayers, sf::Vector2f topLeftPosition)
{
    DiceBackground.setPosition(topLeftPosition);
    DiceBackground.setSize({32*6,32*6});
    DiceBackground.setOrigin(32*3, 32*3);
    DiceBackground.setFillColor(sf::Color::White);
    
    
    int totalnumslots = numSlotsPerPlayer*numPlayers;
    for(int i{0}; i<totalnumslots; i++)
    {
        Slot s;
        s.currentPiece = -1;
        s.isStartArrow = (i%numSlotsPerPlayer == 0);
        //not ideal to hardcode these values :/
        if(i == 0)
        {
            s.type = PT_blue;
        }
        else if (i == numSlotsPerPlayer)
        {
             s.type = PT_yellow;
        }
        else if (i == numSlotsPerPlayer * 2)
        {
             s.type = PT_green;
        }
        else if (i == numSlotsPerPlayer * 3)
        {
             s.type = PT_red;
        }
        else if(i < numSlotsPerPlayer)
        {
            s.type = PT_yellow;
            if(i == 0 + 1 )
            {
                s.isLastSlotBeforeHomeB = true;
            }
        }
        else if (i < numSlotsPerPlayer*2)
        {
            s.type = PT_green;
            if(i == numSlotsPerPlayer + 1)
            {
                s.isLastSlotBeforeHomeY = true;
            }
        }
        else if (i < numSlotsPerPlayer*3)
        {
            s.type = PT_red;
            if(i == numSlotsPerPlayer*2 + 1)
            {
                s.isLastSlotBeforeHomeG = true;
            }
        }
        else if (i < numSlotsPerPlayer*4)
        {
            s.type = PT_blue;
            if(i == numSlotsPerPlayer*3 + 1)
            {
                s.isLastSlotBeforeHomeR = true;
            }
        }
        else
            std::cout<<"AAAAA ERROR YIKES " << i <<std::endl;
        
        s.DistanceToEnd_B = 99;
        s.DistanceToEnd_G = 99;
        s.DistanceToEnd_R = 99;
        s.DistanceToEnd_Y = 99;
        
        float posx = topLeftPosition.x + ((float)totalnumslots * 15.0f) * sinf((float)i / (float)totalnumslots * 3.14159f * 2.0f);// + CanvasDimensions.width / 2;
        float posy = topLeftPosition.y + ((float)totalnumslots * 15.0f) * cosf((float)i / (float)totalnumslots * 3.14159f * 2.0f);// + CanvasDimensions.height / 2 ;
        
        sf::Vector2f position = {posx,posy};
        sf::rotateAroundOrigin(position, topLeftPosition, -numSlotsPerPlayer); //must be -numSlotsPerPlayer as we reverse the vector
        
        s.Position = position;
        
        
        GameBoardSlots.push_back(s);
    }
    //circle generates anticlockwise, so reversing makes it clockwise!
    std::reverse(GameBoardSlots.begin(), GameBoardSlots.end());
    for(int x{0}; x<totalnumslots; x++)
    {
        int z = x;
        int count = 0;
        bool R,G,B,Y;
        R=G=B=Y = true;
        for(int i{x}; i<totalnumslots*2; i++)
        {
            if( i == totalnumslots)
            {
               z = 0;
            }
            if(GameBoardSlots[z].isLastSlotBeforeHomeB && B)
            {
                GameBoardSlots[x].DistanceToEnd_B = count ;
                B = false;
            }
            if(GameBoardSlots[z].isLastSlotBeforeHomeG && G)
            {

                GameBoardSlots[x].DistanceToEnd_G = count ;
                G = false;
            }
            if(GameBoardSlots[z].isLastSlotBeforeHomeR && R)
            {

                GameBoardSlots[x].DistanceToEnd_R = count ;
                R = false;
            }
            if(GameBoardSlots[z].isLastSlotBeforeHomeY && Y)
            {
                GameBoardSlots[x].DistanceToEnd_Y = count ;
                Y = false;
            }
            count++;
             z++;
        }
     /*
      std::cout << "\nSlot: " << x
                << "\n    Distance to R = " << GameBoardSlots[x].DistanceToEnd_R
                << "\n    Distance to G = " << GameBoardSlots[x].DistanceToEnd_G
                << "\n    Distance to B = " << GameBoardSlots[x].DistanceToEnd_B
                << "\n    Distance to Y = " << GameBoardSlots[x].DistanceToEnd_Y;
      */
        }

        
    
    for(int p{0}; p< numPlayers; p++)
    {
        for(int s{0}; s< numPlayerPieces; s++)
        {
            //StartBoardSlots
            Slot HomeSlot;
            HomeSlot.currentPiece=-1;
            HomeSlot.isStartArrow = false;
            float posx{0}, posy{0};
            
            if(p == 0)
            {
                HomeSlot.type = PT_blue;
                posx = 400 + (70*s);
                posy = 960;
            }
            else if (p == 1)
            {
                HomeSlot.type = PT_green;
                posx = 600 - (70*s);
                posy = 40;
            }
            else if (p == 2)
            {
                HomeSlot.type = PT_yellow;
                posy = 400 + (70*s);
                posx = 960;
            }
            else if (p == 3)
            {
                HomeSlot.type = PT_red;
                posy = 600 - (70*s);
                posx = 40;
            }
            HomeSlot.currentPiece = p*4+s;
            pieces->at(p*4 + s).SetPosition({posx,posy},true);
            HomeSlot.DistanceToEnd_B = 99;
            HomeSlot.DistanceToEnd_G = 99;
            HomeSlot.DistanceToEnd_R = 99;
            HomeSlot.DistanceToEnd_Y = 99;
            HomeSlot.Position = {posx,posy};

            StartBoardSlots.push_back(HomeSlot);
            
            //EndBoardSlots
            Slot EndSlot;
            EndSlot.currentPiece=-1;
            EndSlot.isStartArrow = false;

            if(p == 0)
            {
                EndSlot.type = PT_blue;
                posx = topLeftPosition.x;
                posy = topLeftPosition.y + (140 + (70 * s));
            }
            else if (p == 1)
            {
                EndSlot.type = PT_green;
                posx = topLeftPosition.x;
                posy = topLeftPosition.y - (140 + (70 * s));
            }
            else if (p == 2)
            {
                 EndSlot.type = PT_yellow;
                posy = topLeftPosition.y;
                posx = topLeftPosition.x + (140 + (70 * s));
            }
            else if (p == 3)
            {
                EndSlot.type = PT_red;
                posy = topLeftPosition.y;
                posx = topLeftPosition.x - (140 + (70 * s));
            }
            EndSlot.Position = {posx,posy};
            EndSlot.DistanceToEnd_B = 99;
            EndSlot.DistanceToEnd_G = 99;
            EndSlot.DistanceToEnd_R = 99;
            EndSlot.DistanceToEnd_Y = 99;
            EndBoardSlots.push_back(EndSlot);
        }
    }
}
void Frustration_Board::Render(Window *window)
{
    sf::CircleShape circle(16);
    circle.setOutlineThickness(3);
    circle.setFillColor(sf::Color::White);
    circle.setOrigin(8, 8);
    for(auto &s : GameBoardSlots)
    {
      
       // if(s.isStartArrow)
         //   circle.setOutlineColor(sf::Color::Black);
         if(s.type == PT_yellow)
            circle.setOutlineColor(sf::Color::Yellow);
        else if(s.type == PT_blue)
            circle.setOutlineColor(sf::Color::Blue);
        else if(s.type == PT_green)
            circle.setOutlineColor(sf::Color(0,128,0));
        else if(s.type == PT_red)
            circle.setOutlineColor(sf::Color::Red);
        
          /*
        if(s.isLastSlotBeforeHomeB)
            circle.setFillColor(sf::Color::Blue);
        else if(s.isLastSlotBeforeHomeG)
            circle.setFillColor(sf::Color(0,128,0));
        else if(s.isLastSlotBeforeHomeR)
            circle.setFillColor(sf::Color::Red);
        else if(s.isLastSlotBeforeHomeY)
            circle.setFillColor(sf::Color::Yellow);
        else
            circle.setFillColor(sf::Color::White);
        */
        circle.setPosition(s.Position);
        window->draw(circle); 
    }
    circle.setOutlineColor(sf::Color::White);
    for(auto &s : EndBoardSlots)
    {
        if(s.type == PT_yellow)
            circle.setOutlineColor(sf::Color::Yellow);
        else if(s.type == PT_blue)
            circle.setOutlineColor(sf::Color::Blue);
        else if(s.type == PT_green)
            circle.setOutlineColor(sf::Color(0,128,0));
        else if(s.type == PT_red)
            circle.setOutlineColor(sf::Color::Red);
        
        circle.setPosition(s.Position);
        window->draw(circle);
    }
    circle.setOutlineColor(sf::Color::White);
    for(auto &s : StartBoardSlots)
    {
        if(s.type == PT_yellow)
            circle.setFillColor(sf::Color::Yellow);
        else if(s.type == PT_blue)
            circle.setFillColor(sf::Color::Blue);
        else if(s.type == PT_green)
            circle.setFillColor(sf::Color(0,128,0));
        else if(s.type == PT_red)
            circle.setFillColor(sf::Color::Red);
        
        circle.setPosition(s.Position);
        window->draw(circle);
    }
    
    //window->draw(DiceBackground);
}
