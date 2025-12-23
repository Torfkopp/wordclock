#ifndef GRIDMAP_H
#define GRIDMAP_H

#include <Arduino.h>
#include <vector>

struct Word {
    uint8_t start;
    uint8_t end;
};

//
// Attention: The led strip starts upper right corner
// And between every line (every 12th) 
// there is a useless (folded) led (easier than soldering :P)
//
// D A T W I S M H A L V
// K L O C K A R T E E R
// T W I N T I G F I E V
// T E I H N O V Ö R N A
// H A L V S A Y V E E R 
// N E G E N D R E E N K
// S E S S A B A T W E E
// T E I H N R S Ö V E N
// E L V E N F I E V S C
// T W A L V E N A C H T
//

namespace Words {
    // Row 0
    const Word DAT = {8, 10};
    const Word IS = {5, 6};
    const Word HALV1 = {0, 3};
    const Word AL = {1, 2};
    
    // Row 1
    const Word KLOCK = {12, 16};
    const Word KARTEER = {16, 22};

    // Row 2
    const Word TWINTIG = {28, 34};
    const Word FIEV_MIN = {24, 27};

    // Row 3
    const Word TEIHN_MIN = {36, 40};
    const Word VOER = {42, 44};
    const Word NA = {45, 46};
    
    // Row 4
    const Word HALV2 = {55, 58};
    
    // Hours
    const Word EEN = {67, 69};
    const Word TWEE = {72, 75};
    const Word DREE = {65, 68};
    const Word VEER = {48, 51};
    const Word FIEV = {98, 101};
    const Word SESS = {79, 82};
    const Word SOEVEN = {90, 94};
    const Word ACHT = {115, 118};
    const Word NEGEN = {60, 64};
    const Word TEIHN = {84, 88};
    const Word ELVEN = {102, 106};
    const Word TWALVEN = {108, 114};

}

#endif
