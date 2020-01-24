//
//  Player.hpp
//  MyPetridish
//
//  Created by Daniel Harvey on 08/01/2020.
//  Copyright © 2020 Daniel Harvey. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp
#include "Dice.hpp"
class Piece;

class Frustration_Board;
class Player
{
public:
    Player();
    ~Player();
    void Setup(Piece* P1, Piece* P2, Piece* P3, Piece* P4, Frustration_Board *b);
    bool ChoosePiece(unsigned int choice);
    int RollDice();
    bool GetRolled();
    void SetRolled(bool b);
    void Itsyourturn(); 
    bool AllPiecesNotInPlay();
    bool getBPIP();
    
private:
    Dice dice;
    Piece* Piece_1 = nullptr;
    Piece* Piece_2 = nullptr;
    Piece* Piece_3 = nullptr;
    Piece* Piece_4 = nullptr;
    Frustration_Board* board = nullptr;
    
    bool RolledThisTurn = false;
    unsigned int lastDiceRoll = -1; 
    bool PieceOnexit = false;
    int selectedPiece = 0;
    bool BroughtPieceIntoPlay = false;
};

#endif /* Player_hpp */
