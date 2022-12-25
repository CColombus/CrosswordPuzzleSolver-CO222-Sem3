///Depricated as of Solver/ver2.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _{
    //each h_space will have a positive index
    int direct;
    size_t length; //length of block
    char setup[50]; //initial setup of blocks
    char current[50]; //current setup of blocks
    char conwho[50]; //conflict by who
    char conid[50]; //index of who at conflict
} sspace;

/*
consider a grid like this,

|*|*|*|*|_|*|*|
|_|_|A|_|_|_|*| <--- 2ACROSS(3)
|*|*|*|*|_|*|*|
         ^
         |
        1DOWN(5)

Will read as,
    space.direct=0;
    space.length=6;
    space.setup="00A000" <-0 as in NULL (not sure about this yet)
    space.conwho={0,0,0,0,5,0}
    space.conid={0,0,0,0,2,0}

    space.rect=1;
    space.length=3;
    space.setup="000"
    space.conwho={0,3,0}
    space.conid={0,4,0}
*/

int main(){
    return 0;
}