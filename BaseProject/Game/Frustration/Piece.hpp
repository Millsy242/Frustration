//
//  Piece.hpp
//  MyPetridish
//
//  Created by Daniel Harvey on 08/01/2020.
//  Copyright © 2020 Daniel Harvey. All rights reserved.
//

#ifndef Piece_hpp
#define Piece_hpp
#include <SFML/Graphics.hpp>
enum PieceColour : int
{
    PT_red = 0, PT_green = 1, PT_blue = 2, PT_yellow = 3
};
class Piece
{ 
public:
    Piece();
    ~Piece(){};
    void Setup(PieceColour pt, int vp);
    sf::CircleShape* getCircle();
    void SetPosition(sf::Vector2f pos, bool SetStart = false);
    void BringIntoPlay(bool play);
    int distanceToend = -1;
    PieceColour getType();
    int getVecPos();
    bool inPlay = false;
    bool RecentlyInplay = false;
private:
    //sf::Texture P_texture;
    //sf::Sprite P_sprite;
    sf::CircleShape P_circle; 
    sf::Color Colour;
    PieceColour type = PT_red;
    bool atend = false;
    bool StartSet = false;
    sf::Vector2f StartPosition; 
    int radius = 14;
    int VecPos = -1;
    
};

#endif /* Piece_hpp */
