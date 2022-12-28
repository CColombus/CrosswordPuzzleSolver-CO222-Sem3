#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// structure for store horizontal block information
typedef struct sspace
{
    int place;
    int index[1][2];
    int length;
    int direct;
    char setup[100];
    char current[100];
    char conwho[100];
    char conid[100];
} sspace;

typedef struct sgrid
{
    char element[100][100];
} sgrid;

int spaceHrz(char grid[][100], int rows, int cols, int n); // funtion to get the horizontal block information
int spaceVrt(char grid[][100], int rows, int cols, int m); // function to get the vertical block information
void interHrz(int m);
void interVrt(int m, int n);
void conHrz(int m, int n);
void conVrt(int m, int n);
int gridRead(char grid[][100], int rows, int cols);

size_t space_count;
sspace space[50]; // amount of spaces in grid
sgrid keep[10];

typedef struct sword
{
    char *text;
    size_t length;
} sword;

size_t word_count;
sword word[50]; // amount of words given

// Function prototype
int solve();                              // solve puzzle
int getComb(int[], int, int, int, int[]); // get each combination
void setWords(int[]);                     // put words in spaces according to a combination
int solveLength();                        // try to solve by length
int solveFit();                           // try to solve by setup
int solveConflict();                      // try to solve by conflicts
void loadtest();                          // test function for testing

int main()
{
    char grid[100][100]; // to store the given ccrossword puzzle grid
    int rows, cols;

    printf("Enter the grid: \n");

    for (rows = 0; fgets(grid[rows], 100, stdin)[0] != '\n'; rows++)
        for (int j = strcspn(grid[rows], "\n"); j < 100; j++)
            grid[rows][j] = '\0';

    cols = strlen(grid[0]);

    // add one additional row and column as good luck
    rows++;
    cols++;

    space_count = gridRead(grid, rows, cols);

    //////////////////GRID READING DONE ////////////////////////////

    char wordlist[50][20];

    for (word_count = 0; fgets(wordlist[word_count], 100, stdin)[0] != '\n'; word_count++)
        for (int j = strcspn(wordlist[word_count], "\n"); j < 100; j++)
            wordlist[word_count][j] = '\0';

    for (size_t i = 0; i < word_count; i++)
    {
        word[i].text = wordlist[i];
        word[i].length = strlen(wordlist[i]);
    }

    solve();

    return 0;
}

int gridRead(char grid[][100], int rows, int cols)
{
    int m = 0, n = 0;

    // calling funtions
    m = spaceHrz(grid, rows, cols, n);
    n = spaceVrt(grid, rows, cols, m);

    interHrz(m);
    interVrt(m, n);
    conHrz(m, n);
    conVrt(m, n);

    return m+n;
}

// funtion to get the horizontal block information
int spaceHrz(char grid[][100], int rows, int cols, int n)
{
    int m = 0;
    char temp[100];
    for (int i = 0; i <= rows; ++i)
    {
        int count_h = 0, I, J;
        for (int j = 0; j < cols; ++j)
        {
            if (grid[i][j] == '#' || (grid[i][j] >= 65 && grid[i][j] <= 90))
            { // This should also consider partially filled letters
                ++count_h;
                if (count_h == 1)
                {
                    I = i;
                    J = j;
                }
            }

            if (count_h > 1 && (grid[i][j] != '#' && (grid[i][j] < 65 || grid[i][j] > 90)))
            {
                int p;
                space[m + n].direct = 1;
                space[m + n].place = m + n;
                space[m + n].length = count_h;
                space[m + n].index[0][0] = I;
                space[m + n].index[0][1] = J;
                int c = 0;
                for (int j = J; j < J + count_h; ++j)
                {
                    temp[c] = grid[I][j];
                    keep[0].element[I][j] = m + n;
                    ++c;
                }
                strcpy(space[m + n].setup, temp);
                memset(&temp[0], 0, sizeof(temp)); // emptying the temp array for the next iteration
                count_h = 0;
                ++m;
            }

            if ((grid[i][j] != '#' && (grid[i][j] < 65 || grid[i][j] > 90)))
            {
                count_h = 0;
            }
        }
    }
    return m;
}

// function to get the vertical block information
int spaceVrt(char grid[][100], int rows, int cols, int m)
{
    int n = 0;
    char temp_n[100];
    for (int i = 0; i < cols; ++i)
    {
        int count_v = 0, X, Y;
        for (int j = 0; j < rows; ++j)
        {
            if (grid[j][i] == '#' || (grid[j][i] >= 65 && grid[j][i] <= 90))
            {
                ++count_v;
                if (count_v == 1)
                {
                    Y = j;
                    X = i;
                }
            }

            if (count_v > 1 && (grid[j][i] != '#' && (grid[j][i] < 65 || grid[j][i] > 90)))
            {
                space[m + n].direct = 0;
                space[m + n].length = count_v;
                space[m + n].index[0][0] = Y;
                space[m + n].index[0][1] = X;
                int c = 0;
                for (int j = Y; j < Y + count_v; ++j)
                {
                    temp_n[c] = grid[j][X];
                    keep[0].element[j][X] = m + n;
                    ++c;
                }
                strcpy(space[m + n].setup, temp_n);
                memset(&temp_n[0], 0, sizeof(temp_n)); // emptying the temp array for the next iteration
                count_v = 0;
                ++n;
            }

            if ((grid[j][i] != '#' && (grid[j][i] < 65 || grid[j][i] > 90)))
            {
                count_v = 0;
            }
        }
    }
    return n;
}

void interHrz(int m)
{
    for (int i = 0; i < m; ++i)
    {
        int temp = 0;
        for (int j = space[i].index[0][1]; j < space[i].index[0][1] + space[i].length; ++j)
        {
            if (keep[0].element[space[i].index[0][0]][j] == i)
            {
                space[i].conwho[temp] = -1;
            }
            else
            {
                space[i].conwho[temp] = keep[0].element[space[i].index[0][0]][j];
            }
            ++temp;
        }
    }
}

void interVrt(int m, int n)
{
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            keep[1].element[i][j] = keep[0].element[i][j];
        }
    }

    for (int i = 0; i < m; ++i)
    {
        for (int j = space[i].index[0][1]; j < space[i].index[0][1] + space[i].length; ++j)
        {
            keep[1].element[space[i].index[0][0]][j] = space[i].place;
        }
    }

    for (int i = m; i < m + n; ++i)
    {
        int temp = 0;
        for (int j = space[i].index[0][0]; j < space[i].index[0][0] + space[i].length; ++j)
        {
            if (keep[1].element[j][space[i].index[0][1]] == i)
            {
                space[i].conwho[temp] = -1;
            }
            else
            {
                space[i].conwho[temp] = keep[1].element[j][space[i].index[0][1]];
            }
            ++temp;
        }
    }
}

void conHrz(int m, int n)
{
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            keep[2].element[i][j] = keep[0].element[i][j];
        }
    }

    for (int i = m; i < m + n; ++i)
    {
        int temp = 0;
        for (int j = space[i].index[0][0]; j < space[i].index[0][0] + space[i].length; ++j)
        {
            keep[2].element[j][space[i].index[0][1]] = temp;
            ++temp;
        }
    }

    for (int i = 0; i < m; ++i)
    {
        int temp = 0;
        for (int j = space[i].index[0][1]; j < space[i].index[0][1] + space[i].length; ++j)
        {
            if (keep[0].element[space[i].index[0][0]][j] == i)
            {
                space[i].conid[temp] = -1;
            }
            else
            {
                space[i].conid[temp] = keep[2].element[space[i].index[0][0]][j];
            }
            ++temp;
        }
    }
}

void conVrt(int m, int n)
{
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            keep[3].element[i][j] = keep[1].element[i][j];
        }
    }

    for (int i = 0; i < m; ++i)
    {
        int temp = 0;
        for (int j = space[i].index[0][1]; j < space[i].index[0][1] + space[i].length; ++j)
        {
            keep[3].element[space[i].index[0][0]][j] = temp;
            ++temp;
        }
    }

    for (int i = m; i < m + n; ++i)
    {
        int temp = 0;
        for (int j = space[i].index[0][0]; j < space[i].index[0][0] + space[i].length; ++j)
        {
            if (keep[3].element[j][space[i].index[0][1]] == i)
            {
                space[i].conid[temp] = -1;
            }
            else
            {
                space[i].conid[temp] = keep[3].element[j][space[i].index[0][1]];
            }
            ++temp;
        }
    }
}


int solve()
{
    // list of indexes representing words
    int wd_id_arr[word_count];
    for (size_t i = 0; i < word_count; i++)
    {
        wd_id_arr[i] = i;
    }

    int starr[word_count]; // temporary array to put combinations in
    getComb(wd_id_arr, word_count, 0, word_count, starr); // solve for every combination

    // no possible combination found
    printf("IMPOSSIBLE :(\n");
    exit(0);
    return 0;
}

void setWords(int currcomb[])
{ // put each word in spaces according to current combination

    /* printf("current comb is ");
    for (size_t i = 0; i < space_count; i++) printf("%d, ",currcomb[i]);
    printf("\n"); */

    for (size_t i = 0; i < space_count; i++)
    {
        int getid = currcomb[i];

        //space[i].current = word[getid].text;
        strcpy(space[i].current,word[getid].text);
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

            if (space[i].setup[j] != '#' && space[i].setup[j] != space[i].current[j])
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

    return 0; // no conflicts all good
}

int getComb(int arr[], int n, int stack, int nb, int stackarr[])
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
        getComb(newarr, n - 1, st, nb, stackarr);
    }
}
