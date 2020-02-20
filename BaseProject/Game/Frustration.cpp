//
//  MyGame.cpp
//  BaseProject
//
//  Created by Daniel Harvey on 03/11/2019.
//  Copyright © 2019 Daniel Harvey. All rights reserved.
//

#include "Frustration.hpp"

Frustration::Frustration(ige::FileLogger *LOG,SettingsManager *SM) : Game(LOG, SM)
{
    
}

void Frustration::Start()
{
    *log << "Game Start";
    
    sf::Texture DiceTexture;
    
    DiceTexture.loadFromFile("Dice/dice1.png");
    diceTextures[0] = DiceTexture;
    DiceTexture.loadFromFile("Dice/dice2.png");
    diceTextures[1] = DiceTexture;
    DiceTexture.loadFromFile("Dice/dice3.png");
    diceTextures[2] = DiceTexture;
    DiceTexture.loadFromFile("Dice/dice4.png");
    diceTextures[3] = DiceTexture;
    DiceTexture.loadFromFile("Dice/dice5.png");
    diceTextures[4] = DiceTexture;
    DiceTexture.loadFromFile("Dice/dice6.png");
    diceTextures[5] = DiceTexture;
    
    
    for(int i {0}; i<pieces.size(); i++)
    {
        Piece p;
        if(i < 4)
            p.Setup(PT_blue,i);
        else if(i < 8)
            p.Setup(PT_green,i);
        else if(i < 12)
            p.Setup(PT_yellow,i);
        else
            p.Setup(PT_red,i);
        pieces[i] = p;
    }
    Player red,blue,green,yellow;
    
    blue.Setup(&pieces[0], &pieces[1], &pieces[2], &pieces[3], &board);
    green.Setup(&pieces[4], &pieces[5], &pieces[6], &pieces[7], &board);
    yellow.Setup(&pieces[8], &pieces[9], &pieces[10], &pieces[11], &board);
    red.Setup(&pieces[12], &pieces[13], &pieces[14], &pieces[15], &board);
    
    Players[0] = blue;
    Players[1] = red;
    Players[2] = green;
    Players[3] = yellow;
    
    Players[0].Itsyourturn();
    
    board.GivePieces(&pieces);
    board.Setup(7, 4, 4,{500,500});
    
}
void Frustration::Input(std::queue<sf::Event> &events)
{
    *log << "Game Input";
    ImGui::Begin("Dice");
    ImGui::Text("Current Player %i",currentPlayer);
    
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoNav;
    //window_flags |= ImGuiWindowFlags_NoInputs;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowPos({500-64,500-64});
    ImGui::Begin("DiceRoll",NULL,window_flags);
    
    auto nextPlayer = [&]()
    {
        currentPlayer++;
        if(currentPlayer == 4)
            currentPlayer = 0;
        
        gotonextplayer = false;
         Players[currentPlayer].Itsyourturn();
        
        roll = 0;
    };
   
    if(temproll < 0)
        temproll = 0;
    if(ImGui::ImageButton(diceTextures[temproll], {128,128}))
        {
            if(Players[currentPlayer].needstorolldice && !Players[currentPlayer].needstoChoosePiece)
            {
                roll = Players[currentPlayer].RollDice();

                Players[currentPlayer].SetRolled(true);
                
                temproll = roll -1;
            }
        }
           ImGui::End();
    if(Players[currentPlayer].needstoChoosePiece)
    {
        if(ImGui::Button("1")) gotonextplayer = Players[currentPlayer].ChoosePiece(0);
        if(ImGui::Button("2")) gotonextplayer = Players[currentPlayer].ChoosePiece(1);
        if(ImGui::Button("3")) gotonextplayer = Players[currentPlayer].ChoosePiece(2);
        if(ImGui::Button("4")) gotonextplayer = Players[currentPlayer].ChoosePiece(3);
        
        if(ImGui::Button("NextPlayer") || gotonextplayer)
        {
            Players[currentPlayer].SetRolled(false);
            if(!Players[currentPlayer].needstoChoosePiece && !Players[currentPlayer].needstorolldice)
            {
                nextPlayer();
            }
        }
        
    }
    else if ( !Players[currentPlayer].needstorolldice)
        nextPlayer();
   
    
    ImGui::End();
    if(roll > 0)
    {
        ImGui::Begin("Dice");
        ImGui::Text("Player %i rolled a %i", currentPlayer, roll);
        ImGui::End();
    }
    
    

}
void Frustration::Render(Window *window)
{
    *log << "Game Render";
    window->BeginDraw(sf::Color(0,50,50));
    board.Render(window);
    for(int i {0}; i<pieces.size(); i++)
    {
        window->draw(*pieces[i].getCircle());
    }
    
    
    window->EndDraw();
}
void Frustration::UI()
{
    *log << "UI";
    //GameUI();
}
void Frustration::EarlyUpdate()
{
    *log << "Game Early Update";
}
void Frustration::LateUpdate()
{
    *log << "Game Late Update";
}
/*
 if(temproll < 0)
            temproll = 0;
        if(ImGui::ImageButton(diceTextures[temproll], {128,128}))
        {
            if(!Players[currentPlayer].GetRolled())
            {
                roll = Players[currentPlayer].RollDice();
                if(roll != 6 && Players[currentPlayer].AllPiecesNotInPlay())
                {
                    gotonextplayer = true;
                }
                Players[currentPlayer].SetRolled(true);
                
                temproll = roll -1;
            }
        }
           ImGui::End();
    if(Players[currentPlayer].GetRolled() )
    {
        if(ImGui::Button("1")) gotonextplayer = Players[currentPlayer].ChoosePiece(0);
        if(ImGui::Button("2")) gotonextplayer = Players[currentPlayer].ChoosePiece(1);
        if(ImGui::Button("3")) gotonextplayer = Players[currentPlayer].ChoosePiece(2);
        if(ImGui::Button("4")) gotonextplayer = Players[currentPlayer].ChoosePiece(3);
        
        if(ImGui::Button("NextPlayer") || gotonextplayer)
        {
            Players[currentPlayer].SetRolled(false);
            if(!Players[currentPlayer].getBroughtPieceIntoPlay())
            {
                currentPlayer++;
                if(currentPlayer == 4)
                    currentPlayer = 0;
                
                gotonextplayer = false;
            }
             
            roll = 0;
            Players[currentPlayer].Itsyourturn();
        }
    }
    ImGui::End();
    if(roll > 0)
    {
        ImGui::Begin("Dice");
        ImGui::Text("Player %i rolled a %i", currentPlayer, roll);
        ImGui::End();
    }
 */
/*
 if(temproll < 0)
        temproll = 0;
    if(ImGui::ImageButton(diceTextures[temproll], {128,128}))
    {
        if(Players[currentPlayer].numrollsleft == 1)
        {
            roll = Players[currentPlayer].RollDice();
            if(Players[currentPlayer].numrollsleft == 0)
            {
                gotonextplayer = true;
            }
            Players[currentPlayer].SetRolled(true);
            
            temproll = roll -1;
        }
    }
    ImGui::End();
    if(Players[currentPlayer].GetRolled())
    {
        if(ImGui::Button("1")) gotonextplayer = Players[currentPlayer].ChoosePiece(0);
        if(ImGui::Button("2")) gotonextplayer = Players[currentPlayer].ChoosePiece(1);
        if(ImGui::Button("3")) gotonextplayer = Players[currentPlayer].ChoosePiece(2);
        if(ImGui::Button("4")) gotonextplayer = Players[currentPlayer].ChoosePiece(3);
        
        if(ImGui::Button("NextPlayer") || gotonextplayer)
        {
            Players[currentPlayer].SetRolled(false);
            if(Players[currentPlayer].numrollsleft == 0)
            {
                currentPlayer++;
                if(currentPlayer == 4)
                    currentPlayer = 0;
            }
             gotonextplayer = false;
            roll = 0;
            Players[currentPlayer].Itsyourturn();
        }
    }
    ImGui::End();
    if(roll > 0)
    {
        ImGui::Begin("Dice");
        ImGui::Text("Player %i rolled a %i", currentPlayer, roll);
        ImGui::End();
    }
 */
