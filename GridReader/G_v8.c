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

sspace space[100]; // array which store the horizontal block information
sgrid keep[10];

int main()
{
    char grid[100][100]; // to store the given ccrossword puzzle grid
    int rows, cols;

    printf("Enter the grid: \n");

    for (rows = 0; fgets(grid[rows], 100, stdin)[0] != '\n'; rows++)
        for (int j = strcspn(grid[rows], "\n"); j < 100; j++)
            grid[rows][j] = '\0';

    cols = strlen(grid[0]);

    // add one additional row and column as failsafe
    rows++;
    cols++;

    int space_count = gridRead(grid, rows, cols);

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
/*
    // printing the stored values in the stucture created for horizontal blocks
    for (int i = 0; i < m; ++i)
    {
        printf("[%d] The horizontal space at index [%d][%d] is %d spaces long and the format of the block is %s\n", i,space[i].index[0][0], space[i].index[0][1], space[i].length, space[i].setup);
        printf("\twholist = ");
        for (int j = 0; j < 10; ++j)
        {
            printf("%2d, ", space[i].conwho[j]);
        }
        printf("\n");
        printf("\tidlist = ");
        for (int j = 0; j < 10; ++j)
        {
            printf("%2d, ", space[i].conid[j]);
        }
        printf("\n");
    }

    printf("\n");

    // printing the stored values in the stucture created for vertical blocks
    for (int i = m; i < m + n; ++i)
    {
        printf("[%d] The vertical space at index [%d][%d] is %d spaces long and the format of the block is %s\n", i,space[i].index[0][0], space[i].index[0][1], space[i].length, space[i].setup);
        printf("\twholist = ");
        for (int j = 0; j < 10; ++j)
        {
            printf("%2d, ", space[i].conwho[j]);
        }
        printf("\n");
        printf("\tidlist = ");
        for (int j = 0; j < 10; ++j)
        {
            printf("%2d, ", space[i].conid[j]);
        }
        printf("\n");
    }
*/
    for (int i = 0; i < m + n; ++i) printf("%s\n",space[i].setup);

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
        int temp = 1;
        for (int j = space[i].index[0][0]; j < space[i].index[0][0] + space[i].length; ++j)
        {
            keep[2].element[j][space[i].index[0][1]] = 0 - temp;
            ++temp;
        }
    }

    for (int i = 0; i < m; ++i)
    {
        int temp = 0;
        for (int j = space[i].index[0][1]; j < space[i].index[0][1] + space[i].length; ++j)
        {
            if (keep[2].element[space[i].index[0][0]][j] == i)
            {
                space[i].conid[temp] = -1;
            }
            else
            {
                space[i].conid[temp] = (0 - keep[2].element[space[i].index[0][0]][j]) -1;
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
        int temp = 1;
        for (int j = space[i].index[0][1]; j < space[i].index[0][1] + space[i].length; ++j)
        {
            keep[3].element[space[i].index[0][0]][j] = 0 - temp;
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
                space[i].conid[temp] = (0 - keep[3].element[j][space[i].index[0][1]]) -1;
            }
            ++temp;
        }
    }
}