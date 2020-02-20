//
//  Dice.hpp
//  MyPetridish
//
//  Created by Daniel Harvey on 08/01/2020.
//  Copyright © 2020 Daniel Harvey. All rights reserved.
//

#ifndef Dice_hpp
#define Dice_hpp
#include "grand.h"
class Dice
{
public:
    Dice(){};
    ~Dice(){};
    
    unsigned int Roll()
    {
       // return 6;
       return 1+random.i(6);
    }
private: 
    GRand random; 
};
#endif /* Dice_hpp */
