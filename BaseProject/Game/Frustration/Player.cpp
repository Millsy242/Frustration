//
//  Player.cpp
//  MyPetridish
//
//  Created by Daniel Harvey on 08/01/2020.
//  Copyright © 2020 Daniel Harvey. All rights reserved.
//

#include "Player.hpp"
#include "Piece.hpp"
#include "Board.hpp"
#include <iostream>
Player::Player()
{
    
}
Player::~Player()
{

};
bool Player::ChoosePiece(unsigned int choice)
{
    bool SUCCESS = false;
    if(choice < 4)
    {
        std::cout<<"piece chosen correctly " << choice << "\n";
        selectedPiece = choice;
        
        switch (selectedPiece)
        {
            case 0:
               SUCCESS = board->TryMovePiece(Piece_1, lastDiceRoll);
                break;
            case 1:
               SUCCESS = board->TryMovePiece(Piece_2, lastDiceRoll);
                break;
            case 2:
               SUCCESS = board->TryMovePiece(Piece_3, lastDiceRoll);
                break;
            case 3:
               SUCCESS = board->TryMovePiece(Piece_4, lastDiceRoll);
                break;
            default:
                break;
        }
        if(Piece_1->RecentlyInplay || Piece_2->RecentlyInplay || Piece_3->RecentlyInplay || Piece_4->RecentlyInplay)
        {
            BroughtPieceIntoPlay = true;
            needstorolldice = true;
            
            
            Piece_1->RecentlyInplay = Piece_2->RecentlyInplay = Piece_3->RecentlyInplay = Piece_4->RecentlyInplay = false;
        }
    }
    else
    {
        std::cout<<"ERROR chosen Piece does not exist\n";
    }
    
    needstoChoosePiece = !SUCCESS;
    
    return SUCCESS;
}
void Player::Itsyourturn()
{
    BroughtPieceIntoPlay = Piece_1->RecentlyInplay = Piece_2->RecentlyInplay = Piece_3->RecentlyInplay = Piece_4->RecentlyInplay = false;
    numrollsleft = 1;
    needstorolldice = true;
}
bool Player::getBroughtPieceIntoPlay()
{
    return BroughtPieceIntoPlay;
}
bool Player::AllPiecesNotInPlay()
{
    //return the opposite of if(all pieces are in play)
    return !(Piece_1->inPlay || Piece_2->inPlay || Piece_3->inPlay || Piece_4->inPlay);
}
bool Player::GetRolled()
{
    return RolledThisTurn;
}
void Player::SetRolled(bool b)
{
    RolledThisTurn = b; 
}
int Player::RollDice()
{
    lastDiceRoll = dice.Roll();
    
    needstorolldice = false;
    if(lastDiceRoll == 6)
    {
        needstoChoosePiece = true;
        needstorolldice = true;
    }
    else if (!AllPiecesNotInPlay())
        needstoChoosePiece = true;
    else
        needstoChoosePiece = false;

    
    //Return the dice roll result to show the players
    return lastDiceRoll;
}
void Player::Setup(Piece* P1, Piece* P2, Piece* P3, Piece* P4, Frustration_Board *b)
{
    Piece_1 = P1;
    Piece_2 = P2;
    Piece_3 = P3;
    Piece_4 = P4;
    
    board = b;
    
    Piece_1->distanceToend = 99;
    Piece_2->distanceToend = 99;
    Piece_3->distanceToend = 99;
    Piece_4->distanceToend = 99;
    

}
