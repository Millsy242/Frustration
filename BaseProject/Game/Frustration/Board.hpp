//
//  Board.hpp
//  MyPetridish
//
//  Created by Daniel Harvey on 08/01/2020.
//  Copyright © 2020 Daniel Harvey. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp
#include <array>
#include <SFML/Graphics.hpp>
#include "Piece.hpp"
#include <vector>
struct Slot
{
public:
    unsigned int currentPiece = -1;
    unsigned int DistanceToEnd_R = 0;
    unsigned int DistanceToEnd_G = 0;
    unsigned int DistanceToEnd_B = 0;
    unsigned int DistanceToEnd_Y = 0;
    bool isStartArrow = false;
    bool isLastSlotBeforeHomeY = false;
    bool isLastSlotBeforeHomeB = false;
    bool isLastSlotBeforeHomeG = false;
    bool isLastSlotBeforeHomeR = false;
    PieceColour type;
    sf::Vector2f Position; 
};

class Window;
class Frustration_Board
{
public:
    Frustration_Board(){};
    ~Frustration_Board();
    
    void Setup(int numSlotsPerPlayer,int numPlayerPieces, int numPlayers, sf::Vector2f topLeftPosition = {0,0}); 
    void GivePieces(std::array<Piece,16> *p);
    void Render(Window *window);
    bool TryMovePiece(Piece *p, int diceroll); 
    
    std::vector<Slot> GameBoardSlots;
    std::vector<Slot> StartBoardSlots;
    std::vector<Slot> EndBoardSlots;
    
private:
    bool MovePiece(Piece *p,int slotNum);
    std::array<Piece,16> *pieces = nullptr;
    
    sf::RectangleShape DiceBackground;
};

#endif /* Board_hpp */
