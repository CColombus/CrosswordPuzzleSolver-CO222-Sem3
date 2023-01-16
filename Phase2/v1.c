/*
 * The program to solve a crossword puzzle.
 * The program will read a puzzle grid and a set of words
 * from STDIN and will solve and print the solved puzzle
 * to STDOUT.
 * Any letter input will be CAPITALIZED and
 * taken as a valid input.
 * Any invalid input or unsolveable puzzle will result in
 * printing IMPOSSIBLE to STDOUT.
 *
 * @title C0222 - Semester 3 Project - Crossword Puzzle Solver - Phase I
 * @group 05
 * @authors E/19/057    COLOMBAGE C.O.
 * @authors E/19/409    UDUGAMASOORIYA D.P.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// structure for storing information of spaces to solve
typedef struct sspace
{
    int place;         // to store the index of the space when stored in space[]
    int index[1][2];   // starting index of a space
    int length;        // length of the space
    int direct;        // direction of the space(horizontal=1 , vertical=0)
    char setup[100];   // format of the space
    char current[100]; // to store the assigned words for the relevant space block
    char conwho[100];  // with which spaces does a certain space intersect with
    char conid[100];   // if the space intersect with another space which indexes of that space does it intersect with
} sspace;

size_t space_count; // amount of spaces in grid
sspace **space;

/*
    keep[0] = formatted grid to obtain "conwho" details of horizontal blocks
    keep[1] = formatted grid to obtain "conwho" details of vertical blocks
    keep[2] = formatted grid to obtain "conid" details of horizontal blocks
    keep[3] = formatted grid to obtain "conid" details of vertical blocks
    keep[4] = exact copy of the original grid for printing answers
    keep[5] = formatted grid for finding single width spaces (edge case)
*/

char *d_keep_0, *d_keep_1, *d_keep_2, *d_keep_3, *d_keep_4;

// structure for storing words given
typedef struct sword
{
    char *text;
    size_t length;
} sword;

size_t word_count; // amount of words given
sword **word;
// sword word[50];

// Function Prototype (Reader)
int spaceHrz(char grid[][100], int rows, int cols, int n); // funtion to get the horizontal block information
int spaceVrt(char grid[][100], int rows, int cols, int m); // function to get the vertical block information
void interHrz(int m, int rows, int cols);                  // function which obtain the "conwho" details of horizontal blocks
void interVrt(int m, int n, int rows, int cols);           // function which obtain the "conwho" details of vertical blocks
void conHrz(int m, int n, int rows, int cols);             // function which obtain the "conid" details of horizontal blocks
void conVrt(int m, int n, int rows, int cols);             // function which obtain the "conid" details of vertical blocks
int gridRead(char grid[][100], int rows, int cols);        // function which initiate the grid reading process
int space_edge_case_single(int rows, int cols, int mn);    // function for finding single width spaces

// Function prototype (Solver)
int solve(int rows, int cols);                                 // solve puzzle
void getComb(int[], int, int, int, int[], int rows, int cols); // get each combination and check wether it matches
int singleSolveLength(int who, int where);                     // try to solve a space by length
int singleSolveFit(int who, int where);                        // try to solve a space by setup
int solveConflict();                                           // try to solve every space by conflicts
// void loadtest();                           // test function for testing

// Function prototype (Printer)
void printPuzzle(int mn, int rows, int cols);
void freeMemory();

int main()
{
    char grid[100][100]; // to store the given ccrossword puzzle grid
    int rows, cols;

    // get each row of puzzle grid one by one
    for (rows = 0; fgets(grid[rows], 100, stdin)[0] != '\n'; rows++)
    {
        for (int j = strcspn(grid[rows], "\n"); j < 100; j++)
            grid[rows][j] = '\0';
        for (size_t i = 0; i < strlen(grid[rows]); i++)
        {                                                                    // input validation
            if (!((grid[rows][i] >= 65) && (grid[rows][i] <= 90)))           // is not simple?
                if (!((grid[rows][i] >= 97) && (grid[rows][i] <= 122)))      // is not capital?
                    if (!((grid[rows][i] == '*') || (grid[rows][i] == '#'))) // is not # or *?
                    {
                        printf("INVALID INPUT\n");
                        exit(0);
                    }
            // simple to capital
            grid[rows][i] = ((grid[rows][i] >= 97) && (grid[rows][i] <= 122)) ? grid[rows][i] - 32 : grid[rows][i];
        }
    }

    // how many columns?
    cols = strlen(grid[0]);

    // grid reading function returns the total number of spaces found
    space_count = gridRead(grid, rows, cols);

    /////////////////////GRID READING DONE ////////////////////////////

    char **wordlist;
    wordlist = (char **) malloc(rows * cols * sizeof(char *));

    for (size_t i = 0; i < rows * cols; i++) {
        wordlist[i] = (char *) malloc(100 * sizeof(char));
    }

    // get each word one by one
    for (word_count = 0; fgets(wordlist[word_count], 100, stdin)[0] != '\n'; word_count++)
    { // input validation
        for (int j = strcspn(wordlist[word_count], "\n"); j < 100; j++)
            wordlist[word_count][j] = '\0';
        for (size_t i = 0; i < strlen(wordlist[word_count]); i++)
        {
            if (!((wordlist[word_count][i] >= 65) && (wordlist[word_count][i] <= 90)))      // is not simple?
                if (!((wordlist[word_count][i] >= 97) && (wordlist[word_count][i] <= 122))) // is not capital?
                {
                    printf("INVALID INPUT\n");
                    exit(0);
                }
            // simple to capital
            wordlist[word_count][i] = ((wordlist[word_count][i] >= 97) && (wordlist[word_count][i] <= 122)) ? wordlist[word_count][i] - 32 : wordlist[word_count][i];
        }
    }

    // asigning words to a word structure
    word = (sword **)malloc(word_count * sizeof(sword *));

    for (size_t i = 0; i < word_count; i++)
    {
        word[i] = (sword *)malloc(sizeof(sword));
    }

    for (size_t i = 0; i < word_count; i++)
    {
        word[i]->text = malloc(strlen(wordlist[i]) * sizeof(char));
        strcpy(word[i]->text, wordlist[i]);
        word[i]->length = strlen(wordlist[i]);
    }

    free(wordlist);

    // if not enough words are given to fill the spaces no point in solving
    if (word_count < space_count)
    {
        printf("IMPOSSIBLE\n");
        freeMemory();
        exit(0);
    }

    // calling solver
    solve(rows, cols);

    return 0;
}

int gridRead(char grid[][100], int rows, int cols) // function which initiate the grid reading process
{

    int m = 0, n = 0, s = 0;

    space = (sspace **)malloc(rows * cols * sizeof(sspace *));

    // calling funtions
    m = spaceHrz(grid, rows, cols, n);
    n = spaceVrt(grid, rows, cols, m);

    interHrz(m, rows, cols);
    interVrt(m, n, rows, cols);
    conHrz(m, n, rows, cols);
    conVrt(m, n, rows, cols);

    int p = 0;
    d_keep_4 = malloc(sizeof(char) * rows * cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            d_keep_4[p] = grid[i][j];
            p++;
        }
    }

    // special function for solving edge cases with single width spaces
    s = space_edge_case_single(rows, cols, m + n);

    return m + n + s;
}

int space_edge_case_single(int rows, int cols, int mn)
{ // special case for finding single width spaces
    char *keep_0[rows + 2];
    for (int i = 0; i < rows + 2; i++)
    {
        keep_0[i] = (char *)malloc((cols + 2) * sizeof(char));
    }

    char *keep_1[rows];
    for (int i = 0; i < rows; i++)
    {
        keep_1[i] = (char *)malloc(cols * sizeof(char));
    }

    int p = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            keep_1[i][j] = d_keep_4[p];
            p++;
        }
    }

    for (size_t i = 0; i < cols + 2; i++)
    { // recreate the puzzle grid with a border of * around it
        keep_0[0][i] = '*';
        keep_0[rows + 1][i] = '*';
    }

    for (size_t i = 1; i <= rows; i++)
    { // recreate the puzzle grid with a border of * around it
        keep_0[i][0] = '*';
        keep_0[i][cols + 1] = '*';

        for (size_t j = 1; j <= cols; j++)
        {
            keep_0[i][j] = keep_1[i - 1][j - 1];
        }
    }

    int counter = 0; // how many single width spaces we find?

    for (size_t i = 1; i <= rows; i++)
    { // check every row
        for (size_t j = 1; j <= cols; j++)
        { // check every column
            if ((keep_0[i][j] == '#' || (keep_0[i][j] >= 65 && keep_0[i][j] <= 90)))
            { // if we find a non * element
                if ((keep_0[i][j - 1] == '*') && (keep_0[i][j + 1] == '*') && (keep_0[i - 1][j] == '*') && (keep_0[i + 1][j] == '*'))
                { // check whether it is isolated by all sides

                    // filling a new entry for spaces
                    space[mn + counter] = (sspace *)malloc(sizeof(sspace));
                    space[mn + counter]->place = mn + counter;
                    space[mn + counter]->index[0][0] = i - 1;
                    space[mn + counter]->index[0][1] = j - 1;
                    space[mn + counter]->length = 1;
                    space[mn + counter]->direct = 1;
                    space[mn + counter]->conid[0] = -1;
                    space[mn + counter]->conwho[0] = -1;
                    space[mn + counter]->setup[0] = keep_0[i][j];

                    counter++;
                }
            }
        }
    }
    for (int i = 0; i < rows + 2; i++)
    { // freeing dynamic arrays
        free(keep_0[i]);
    }

    for (int i = 0; i < rows; i++)
    { // freeing dynamic arrays
        free(keep_1[i]);
    }

    return counter;
}

// funtion to get the horizontal block information
int spaceHrz(char grid[][100], int rows, int cols, int n)
{
    char *keep_0[rows];
    for (int i = 0; i < rows; i++)
    {
        keep_0[i] = (char *)malloc(cols * sizeof(char));
    }

    int m = 0;
    char temp[100]; // temmporary array which stores the format of the space
    for (int i = 0; i <= rows; ++i)
    {
        int count_h = 0, I = 0, J = 0;
        for (int j = 0; j <= cols; ++j)
        {
            if (grid[i][j] == '#' || (grid[i][j] >= 65 && grid[i][j] <= 90))
            {
                ++count_h;        // variable which stores the length of the space block
                if (count_h == 1) // to capture the starting index of the space block
                {
                    I = i;
                    J = j;
                }
            }

            if (count_h > 1 && (grid[i][j] != '#' && (grid[i][j] < 65 || grid[i][j] > 90))) // obtaining the requires structural data of the space block
            {
                space[m + n] = (sspace *)malloc(sizeof(sspace));
                space[m + n]->direct = 1;
                space[m + n]->place = m + n;
                space[m + n]->length = count_h;
                space[m + n]->index[0][0] = I;
                space[m + n]->index[0][1] = J;
                int c = 0;
                for (int j = J; j < J + count_h; ++j)
                {
                    temp[c] = grid[I][j];
                    keep_0[I][j] = m + n;
                    ++c;
                }
                strcpy(space[m + n]->setup, temp);
                memset(&temp[0], 0, sizeof(temp)); // emptying the temp array for the next iteration
                count_h = 0;
                ++m;
            }

            if ((grid[i][j] != '#' && (grid[i][j] < 65 || grid[i][j] > 90))) // identifying the end of the block
            {
                count_h = 0;
            }
        }
    }
    int p = 0;
    d_keep_0 = malloc(sizeof(char) * rows * cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            d_keep_0[p] = keep_0[i][j];
            p++;
        }
    }

    for (int i = 0; i < rows; i++)
    { // freeing dynamic arrays
        free(keep_0[i]);
    }
    return m;
}

// function to get the vertical block information
int spaceVrt(char grid[][100], int rows, int cols, int m)
{

    char *keep_0[rows];
    for (int i = 0; i < rows; i++)
    {
        keep_0[i] = (char *)malloc(cols * sizeof(char));
    }

    int p = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            keep_0[i][j] = d_keep_0[p];
            p++;
        }
    }

    int n = 0;
    char temp_n[100]; // temmporary array which stores the format of the space
    for (int i = 0; i <= cols; ++i)
    {
        int count_v = 0, X = 0, Y = 0;
        for (int j = 0; j <= rows; ++j)
        {
            if (grid[j][i] == '#' || (grid[j][i] >= 65 && grid[j][i] <= 90))
            {
                ++count_v;        // variable which stores the length of the space block
                if (count_v == 1) // to capture the starting index of the space block
                {
                    Y = j;
                    X = i;
                }
            }

            if (count_v > 1 && (grid[j][i] != '#' && (grid[j][i] < 65 || grid[j][i] > 90))) // obtaining the requires structural data of the space block
            {
                space[m + n] = (sspace *)malloc(sizeof(sspace));
                space[m + n]->direct = 0;
                space[m + n]->length = count_v;
                space[m + n]->index[0][0] = Y;
                space[m + n]->index[0][1] = X;
                int c = 0;
                for (int j = Y; j < Y + count_v; ++j)
                {
                    temp_n[c] = grid[j][X];
                    keep_0[j][X] = m + n;
                    ++c;
                }
                strcpy(space[m + n]->setup, temp_n);
                memset(&temp_n[0], 0, sizeof(temp_n)); // emptying the temp array for the next iteration
                count_v = 0;
                ++n;
            }

            if ((grid[j][i] != '#' && (grid[j][i] < 65 || grid[j][i] > 90))) // identifying the end of the block
            {
                count_v = 0;
            }
        }
    }
    p = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            d_keep_0[p] = keep_0[i][j];
            p++;
        }
    }

    for (int i = 0; i < rows; i++)
    { // freeing dynamic arrays
        free(keep_0[i]);
    }
    return n;
}

void interHrz(int m, int rows, int cols) // function which obtain the "conwho" details of horizontal blocks
{
    char *keep_0[rows];
    for (int i = 0; i < rows; i++)
    {
        keep_0[i] = (char *)malloc(cols * sizeof(char));
    }

    int p = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            keep_0[i][j] = d_keep_0[p];
            p++;
        }
    }

    for (int i = 0; i < m; ++i)
    {
        int temp = 0;
        for (int j = space[i]->index[0][1]; j < space[i]->index[0][1] + space[i]->length; ++j) // obtainig "conwho" details from the formatted grid
        {
            if (keep_0[space[i]->index[0][0]][j] == i)
            {
                space[i]->conwho[temp] = -1;
            }
            else
            {
                space[i]->conwho[temp] = keep_0[space[i]->index[0][0]][j];
            }
            ++temp;
        }
    }
    for (int i = 0; i < rows; i++)
    { // freeing dynamic arrays
        free(keep_0[i]);
    }
}

void interVrt(int m, int n, int rows, int cols) // function which obtain the "conwho" details of vertical blocks
{

    char *keep_0[rows];
    for (int i = 0; i < rows; i++)
    {
        keep_0[i] = (char *)malloc(cols * sizeof(char));
    }

    int p = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            keep_0[i][j] = d_keep_0[p];
            p++;
        }
    }

    for (int i = 0; i < m; ++i)
    {
        for (int j = space[i]->index[0][1]; j < space[i]->index[0][1] + space[i]->length; ++j)
        {
            keep_0[space[i]->index[0][0]][j] = space[i]->place;
        }
    }

    for (int i = m; i < m + n; ++i) // obtainig "conwho" details from the formatted grid
    {
        int temp = 0;
        for (int j = space[i]->index[0][0]; j < space[i]->index[0][0] + space[i]->length; ++j)
        {
            if (keep_0[j][space[i]->index[0][1]] == i)
            {
                space[i]->conwho[temp] = -1;
            }
            else
            {
                space[i]->conwho[temp] = keep_0[j][space[i]->index[0][1]];
            }
            ++temp;
        }
    }
    p = 0;
    d_keep_1 = malloc(sizeof(char) * rows * cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            d_keep_1[p] = keep_0[i][j];
            p++;
        }
    }

    for (int i = 0; i < rows; i++)
    { // freeing dynamic arrays
        free(keep_0[i]);
    }
}

void conHrz(int m, int n, int rows, int cols) // function which obtain the "conid" details of horizontal blocks
{
    char *keep_0[rows];
    for (int i = 0; i < rows; i++)
    {
        keep_0[i] = (char *)malloc(cols * sizeof(char));
    }

    int p = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            keep_0[i][j] = d_keep_0[p];
            p++;
        }
    }

    for (int i = m; i < m + n; ++i)
    {
        // initializing a special indexing method to avoid conflicts from previously stored index values in the grid
        int temp = 1;
        for (int j = space[i]->index[0][0]; j < space[i]->index[0][0] + space[i]->length; ++j)
        {
            keep_0[j][space[i]->index[0][1]] = 0 - temp;
            ++temp;
        }
    }

    for (int i = 0; i < m; ++i) // obtainig "conid" details from the formatted grid
    {
        int temp = 0;
        for (int j = space[i]->index[0][1]; j < space[i]->index[0][1] + space[i]->length; ++j)
        {
            if (keep_0[space[i]->index[0][0]][j] == i)
            {
                space[i]->conid[temp] = -1;
            }
            else
            {
                space[i]->conid[temp] = (0 - keep_0[space[i]->index[0][0]][j]) - 1;
            }
            ++temp;
        }
    }
    p = 0;
    d_keep_2 = malloc(sizeof(char) * rows * cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            d_keep_2[p] = keep_0[i][j];
            p++;
        }
    }

    for (int i = 0; i < rows; i++)
    { // freeing dynamic arrays
        free(keep_0[i]);
    }
}

void conVrt(int m, int n, int rows, int cols) // function which obtain the "conid" details of vertical blocks
{
    char *keep_0[rows];
    for (int i = 0; i < rows; i++)
    {
        keep_0[i] = (char *)malloc(cols * sizeof(char));
    }

    int p = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            keep_0[i][j] = d_keep_0[p];
            p++;
        }
    }

    for (int i = 0; i < m; ++i)
    {
        // initializing a special indexing method to avoid conflicts from previously stored index values in the grid
        int temp = 1;
        for (int j = space[i]->index[0][1]; j < space[i]->index[0][1] + space[i]->length; ++j)
        {
            keep_0[space[i]->index[0][0]][j] = 0 - temp;
            ++temp;
        }
    }

    for (int i = m; i < m + n; ++i) // obtainig "conid" details from the formatted grid
    {
        int temp = 0;
        for (int j = space[i]->index[0][0]; j < space[i]->index[0][0] + space[i]->length; ++j)
        {
            if (keep_0[j][space[i]->index[0][1]] == i)
            {
                space[i]->conid[temp] = -1;
            }
            else
            {
                space[i]->conid[temp] = (0 - keep_0[j][space[i]->index[0][1]]) - 1;
            }
            ++temp;
        }
    }
    p = 0;
    d_keep_3 = malloc(sizeof(char) * rows * cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            d_keep_3[p] = keep_0[i][j];
            p++;
        }
    }

    for (int i = 0; i < rows; i++)
    { // freeing dynamic arrays
        free(keep_0[i]);
    }
}

int solve(int rows, int cols)
{
    // list of consecutive indexes representing each given word
    int wd_id_arr[word_count];
    for (size_t i = 0; i < word_count; i++)
    {
        wd_id_arr[i] = i;
    }

    // we will try getting different combinations of this list and
    // try assign them to each space by thier index and see if it matches

    int starr[word_count];                                            // temporary array to put combinations in
    getComb(wd_id_arr, word_count, 0, word_count, starr, rows, cols); // solve for every combination

    // no possible combination found
    printf("IMPOSSIBLE\n");
    exit(0);
    return 0;
}

int singleSolveLength(int who, int where)
{                                                  // try to match a space with a word by comparing their lengths
    strcpy(space[where]->current, word[who]->text); // put word(who) in relevant space(where)
    if (space[where]->length != strlen(space[where]->current))
        return 1; // diff lengths
    return 0;     // same lengths
}

int singleSolveFit(int who, int where)
{ // try to match a space with a word by comparing the given letters inside the puzzle grid
    for (size_t j = 0; j < space[where]->length; j++)
    { // for each index in space
        // compare current with setup
        if (space[where]->setup[j] != '#' && space[where]->setup[j] != space[where]->current[j])
            return 1; // cannot fit
    }
    return 0; // can be fitted
}

int solveConflict()
{ // return 1 if any conflict found
    for (size_t i = 0; i < space_count; i++)
    { // for each space

        for (size_t j = 0; j < space[i]->length; j++)
        { // for each index in space

            if (space[i]->conwho[j] != -1)
            {
                int who = space[i]->conwho[j];
                int which = space[i]->conid[j];
                if (space[who]->current[which] != space[i]->current[j])
                    return 1; // found conflict
            }
        }
    }

    return 0; // no conflicts all good
}

void getComb(int arr[], int n, int stack, int nb, int stackarr[], int rows, int cols)
{ // get each combination and see if it matches, return if no match found
    if (n == 1)
    { // found a combination
        // complete the current combination
        stackarr[stack] = arr[0];

        if (singleSolveLength(arr[0], stack))
            return; // assign the last word into the space and compare lengths
        if (singleSolveFit(arr[0], stack))
            return; // compare the fit with setup

        // match by conflicting intersections
        if (solveConflict())
            return; // conflict exists

        // passed all test for the current combination thus solved the puzzle
        printPuzzle(nb, rows, cols);
        freeMemory();
        exit(0);

        return;
    }
    for (size_t i = 0; i < n; i++)
    {
        // if length or fit doesn't match for the current position no point in continuing this branch
        if (singleSolveLength(arr[i], stack))
            continue; // assign word into the space and compare lengths
        if (singleSolveFit(arr[i], stack))
            continue; // compare the fit with setup

        // add next combination to the branch
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

        // try to solve with the new branch recursively
        getComb(newarr, n - 1, st, nb, stackarr, rows, cols);
    }
}

void printPuzzle(int mn, int rows, int cols)
{ // replaces the # in puzzle with letters from solved spaces

    char *grid[rows];
    for (int i = 0; i < rows; i++)
    {
        grid[i] = (char *)malloc(cols * sizeof(char));
    }

    int p = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            grid[i][j] = d_keep_4[p];
            p++;
        }
    }

    for (size_t i = 0; i < mn; i++)
    {
        // get the starting points of each space relative to the puzzle grid
        int goto_row = space[i]->index[0][0];
        int goto_col = space[i]->index[0][1];

        // now to replace each grid element with thier respective letter
        if (space[i]->direct == 1) // horizontal print
        {
            for (size_t col = 0; col < space[i]->length; col++)
            {
                grid[goto_row][goto_col + col] = space[i]->current[col];
            }
        }
        else // vertical print
        {
            for (size_t row = 0; row < space[i]->length; row++)
            {
                grid[goto_row + row][goto_col] = space[i]->current[row];
            }
        }
    }

    // print the now complete grid to stdout
    for (size_t r = 0; r < rows; r++)
    {
        for (size_t q = 0; q < cols; q++)
        {
            printf("%c", grid[r][q]);
        }
        printf("\n");
    }

    for (int i = 0; i < rows; i++)
    { // freeing dynamic arrays
        free(grid[i]);
    }
}

void freeMemory(){
    for (size_t i = 0; i < word_count; i++)
    {
        free(word[i]);
    }
    free(word);

    for (size_t i = 0; i < space_count; i++)
    {
        free(space[i]);
    }
    free(space);
}