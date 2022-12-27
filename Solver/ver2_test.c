#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct sspace
{
    int direct;
    size_t length;  // length of block
    char *setup;    // initial setup of blocks
    char *current;  // current setup of blocks
    int conwho[10]; // conflict by who
    int conid[10];  // index of who at conflict
} sspace;

size_t space_count;
sspace space[20]; // amount of spaces in grid

typedef struct sword
{
    char *text;
    size_t length;
} sword;

size_t word_count;
sword word[20]; // amount of words given

// Function prototype
int solve();                                                   // solve puzzle
int iterate(int arr[], int n, int stack, int nb, int starr[]); // get each combination
void setWords(int[]);                                          // put words in spaces according to a combination
int solveLength();                                             // try to solve by length
int solveFit();                                                // try to solve by setup
int solveConflict();                                           // try to solve by conflicts
void loadtest();                                               // test function for testing

int main()
{
    loadtest(); // to br replaced by grid_read
    solve();
    return 0;
}

int solve()
{
    // list of indexes representing words
    int wd_id_arr[word_count];
    for (size_t i = 0; i < word_count; i++)
    {
        wd_id_arr[i] = i;
    }

    int starr[word_count];                                // temporary array to put combinations in
    iterate(wd_id_arr, word_count, 0, word_count, starr); // solve for every combination

    // no possible combination found
    printf("IMPOSSIBLE :(\n");
    exit(0);
    return 0;
}

void setWords(int currcomb[])
{ // put each word in spaces according to current combination
    for (size_t i = 0; i < space_count; i++)
    {
        int getid = currcomb[i];

        space[i].current = word[getid].text;
    }
}

int solveLength()
{

    for (size_t i = 0; i < space_count; i++)
    {
        if (space[i].length != strlen(space[i].current))
            return 1; // diff lengths
    }

    return 0; // lengths match
}

int solveFit()
{

    for (size_t i = 0; i < space_count; i++)
    { // for each space

        for (size_t j = 0; j < space[i].length; j++)
        { // for each index in space

            if (space[i].setup[j] != '-' && space[i].setup[j] != space[i].current[j])
                return 1; // cannot fit
        }
    }

    return 0; // can be fitted
}

int solveConflict()
{ // return 1 if any conflict found
    for (size_t i = 0; i < space_count; i++)
    { // for each space

        for (size_t j = 0; j < space[i].length; j++)
        { // for each index in space

            if (space[i].conwho[j] != -1)
            {
                int who = space[i].conwho[j];
                int which = space[i].conid[j];
                if (space[who].current[which] != space[i].current[j])
                    return 1; // found conflict
            }
        }
    }

    return 0; //no conflicts all good
}

int iterate(int arr[], int n, int stack, int nb, int stackarr[])
{
    if (n == 1) // found a combination
    {
        stackarr[stack] = arr[0]; // complete the current combination

        // solve for the current combination
        setWords(stackarr); // put words in spaces

        // match by lengths
        if (solveLength())
            return 0; // length does not match
        // printf("possible by length\n");

        // match by setups
        if (solveFit())
            return 0; // setup does not match
        // printf("possible by fit\n");

        // match by setups
        if (solveConflict())
            return 0; // conflict exists
        // printf("possible by no conflict\n");

        // passed all test for the current combination thus solved the puzzle
        printf("SOLVED!!\n");
        exit(0);

        return 0;
    }
    for (size_t i = 0; i < n; i++)
    {
        stackarr[stack] = arr[i];
        int newarr[n - 1], index = 0;
        for (size_t j = 0; j < n; j++)
        {
            if (j == i)
                continue;
            else
            {
                newarr[index] = arr[j];
                index++;
            }
        }
        int st = nb - (n - 1);
        iterate(newarr, n - 1, st, nb, stackarr);
    }
}

void loadtest()
{
    /*
    ~~~~~Solution~~~
    **F***
    *GLUE*
    **Y***
    ******
    **B***
    *CAT**
    **N***
    **E***
    ***SIX
    ~~~~~Grid~~~~~
    **-***
    *----*
    **-***
    ******
    **-***
    *-A-**
    **-***
    **-***
    ***--X
    */
    space_count = 5;
    word_count = 5;
    int direct[] = {1, 1, 0, 0, 1}; // 1 for horizonal 0 for vertical
    char *spacelist[] = {"G---", "---", "---", "----", "--X"};
    char *wordlist[] = {"SIX", "FLY", "BANE", "GLUE", "CAT"};
    int conwholist[10][10] = {                 //-1 if no conflict
                              {-1, 2, -1, -1}, // GLUE(0) is conflicted by FLY(2)
                              {-1, 3, -1},     // CAT(1) is conf by BANE(3)
                              {-1, 0, -1},     // FLY(2) is conflicted by GLUE(0)
                              {-1, 1, -1, -1}, // BANE(3) is conf by CAT(1)
                              {-1, -1, -1}};
    int conidlist[10][10] = {                 //-1 if no conflict
                             {-1, 1, -1, -1}, // L in FLY(2) at 1
                             {-1, 1, -1},     // A in BANCE
                             {-1, 1, -1},     // L in GLUE(0) at 1
                             {-1, 1, -1, -1}, // A in CAT
                             {-1, -1, -1}};

    for (size_t i = 0; i < space_count; i++)
    {
        space[i].setup = spacelist[i];
        space[i].length = strlen(spacelist[i]);
        memcpy(space[i].conwho, conwholist[i], sizeof(int) * 10);
        memcpy(space[i].conid, conidlist[i], sizeof(int) * 10);
    }

    for (size_t i = 0; i < word_count; i++)
    {
        word[i].text = wordlist[i];
        word[i].length = strlen(wordlist[i]);
    }
}
