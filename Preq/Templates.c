<<<<<<< HEAD
=======
///Depricated as of Solver/ver2.c

>>>>>>> e6eb10ad6d611ae95ade751a6113e9e669014a3a
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _{
    //each h_space will have a positive index
<<<<<<< HEAD
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
=======
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
>>>>>>> e6eb10ad6d611ae95ade751a6113e9e669014a3a
*/

int main(){
    return 0;
}