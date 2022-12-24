#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _{
    //each h_space will have a positive index
    size_t length; //length of block
    char setup[100]; //initial or current setup of blocks
    char points[100]; //points of crossing(if any) and by whom
} h_space;

typedef struct _{
    //each v_space will have a negative index
    size_t length; //length of block
    char setup[100]; //initial or current setup of blocks
    char points[100]; //points of crossing(if any) and by whom
} v_space;

/*
consider a h_block like this,

|*|*|*|*|_|*|*|
|_|_|A|_|_|_|*| <--- 2ACROSS
|*|*|*|*|_|*|*|
         ^
         |
        1DOWN

Will read as,
    h_space.length=6;
    h_space.setup="00A000" <-0 as in NULL (not sure about this yet)
    h_space.points={0,0,0,0,-1,0}

    v_space.length=3;
    v_space.setup="000"
    v_space.points={0,2,0}
*/

int main(){
    return 0;
}