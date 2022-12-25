//This version can read grids with partially filled blocks with letters
#include <stdio.h>
#include <string.h>

//structure for store horizontal block information
typedef struct _{
    int index[1][2];
    int length;
    char setup[100];
}h_space;

//structure for store vertical block information
typedef struct  _{
    int index[1][2];
    int length;
    char setup[100];
}v_space;



int horizonSpace(char grid[][100], int k, h_space vals[]); //funtion to get the horizontal block information
int vertSpace(char transpose[][100], int k, v_space v_vals[]); //function to get the vertical block information
void intersect(char grid[][100], int k); //function to get the intersection points of the grid

int main(){

    int m, n; //wadiya hithanna epa :)
    h_space vals[100]; //array which store the horizontal block information
    v_space v_vals[100]; //array which store the vertical block information

    char grid[100][100]; //to store the given ccrossword puzzle grid
    char transpose[100][100]; //to read the grid columnwise
    int k = 0;

    printf("Enter the grid: \n");
    while (1 == scanf("%[^\n]%*c", grid[k])){
        ++k;
    }

    // obtaining the transpose of the grid
    for (int i=0; i<=k; ++i){
        for (int j=0; j<100; ++j){
            transpose[j][i] = grid[i][j];
        }
    }

    //calling funtions
    m = horizonSpace(grid, k, vals);
    n = vertSpace(transpose, k, v_vals);
    intersect(grid, k);

    printf("\n");

    //printing the stored values in the stucture created for horizontal blocks
    for (int i = 0; i<m; ++i){
        printf("The horizontal space at index [%d][%d] is %d spaces long and the format of the block is %s\n",vals[i].index[0][0], vals[i].index[0][1], vals[i].length, vals[i].setup);
    }
    printf("\n");

    //printing the stored values in the stucture created for vertical blocks
    for (int i = 0; i<n; ++i){
        printf("The vertical space at index [%d][%d] is %d spaces long and the format of the block is %s\n",v_vals[i].index[0][0], v_vals[i].index[0][1], v_vals[i].length, v_vals[i].setup);
    }

}

//funtion to get the horizontal block information
int horizonSpace(char grid[][100], int k, h_space vals[]){
    int m = 0;
    char temp[100];
    for (int i = 0; i<=k; ++i){
        int count_h = 0, I, J;
        for (int j = 0; j <100; ++j){
            if (grid[i][j] == '#' || (grid[i][j] >=65 && grid[i][j] <=90)){ // This should also consider partially filled letters
                ++count_h;
                if (count_h == 1){
                    I = i;
                    J = j;
                }
            }

            if (count_h > 1 && (grid[i][j] != '#' && (grid[i][j] < 65 || grid[i][j] > 90))){
                //printf("THe horizontal space at index [%d][%d] is %d spaces long\n",I, J, count_h); //use this printf() statement for direct reading purposes
                vals[m].length = count_h;
                vals[m].index[0][0] = I;
                vals[m].index[0][1] = J;
                int c = 0;
                for (int j=J; j<J+count_h; ++j){
                    temp[c] = grid[I][j];
                    ++c;
                }
                strcpy(vals[m].setup, temp);
                memset(&temp[0], 0, sizeof(temp)); //emptying the temp array for the next iteration
                count_h = 0;
                ++m;
            }

            if((grid[i][j] != '#' && (grid[i][j] < 65 || grid[i][j] > 90))){
                count_h = 0;
            }
        }
    }
    return vals, m;
    
}

//function to get the vertical block information
int vertSpace(char transpose[][100], int k, v_space v_vals[]){
    int n =0;
    char temp_n[100];
    for (int i = 0; i<100; ++i){
        int count_v = 0, X, Y;
        for (int j = 0; j <100; ++j){
            if (transpose[i][j] == '#' || (transpose[i][j] >=65 && transpose[i][j] <=90)){
                ++count_v;
                if (count_v == 1){
                    Y = i;
                    X = j;
                }
            }

            if (count_v > 1 && (transpose[i][j] != '#' && (transpose[i][j] < 65 || transpose[i][j] > 90))){
                //printf("The vertical space at index [%d][%d] is %d spaces long\n", X, Y, count_v); //use this for direct printing purposes
                v_vals[n].length = count_v;
                v_vals[n].index[0][0] = X;
                v_vals[n].index[0][1] = Y;
                int c = 0;
                for (int j=X; j<X+count_v; ++j){
                    temp_n[c] = transpose[Y][j];
                    ++c;
                }
                strcpy(v_vals[n].setup, temp_n);
                memset(&temp_n[0], 0, sizeof(temp_n)); //emptying the temp array for the next iteration
                count_v = 0;
                ++n;
            }

            if((transpose[i][j] != '#' && (transpose[i][j] < 65 || transpose[i][j] > 90)) ){
                count_v = 0;
            }
        }
    }
    return v_vals, n;
}

//funtion to get the intersection points of the grid
void intersect(char grid[][100], int k){
    for (int i = 0; i<=k; ++i){
        for (int j = 0; j <100; ++j){
            if (grid[i][j] == '#' || (grid[i][j] >=65 && grid[i][j] <=90)){
                if (((grid[i][j-1] == '#' || (grid[i][j-1]>=65 && grid[i][j-1]<=90)) && (grid[i-1][j] == '#' || (grid[i-1][j]>=65 && grid[i-1][j]<=90)) && (grid[i+1][j] == '#' || (grid[i+1][j]>=65 && grid[i+1][j]<=90))) || 
                    ((grid[i][j+1] == '#' || (grid[i][j+1]>=65 && grid[i][j+1]<=90)) && (grid[i-1][j] == '#' || (grid[i-1][j]>=65 && grid[i-1][j]<=90)) && (grid[i+1][j] == '#' || (grid[i+1][j]>=65 && grid[i+1][j]<=90))) || 
                    ((grid[i][j-1] == '#' || (grid[i][j-1]>=65 && grid[i][j-1]<=90)) && (grid[i][j+1] == '#' || (grid[i][j+1]>=65 && grid[i][j+1]<=90)) && (grid[i-1][j] == '#' || (grid[i-1][j]>=65 && grid[i-1][j]<=90))) || 
                    ((grid[i][j-1] == '#' || (grid[i][j-1]>=65 && grid[i][j-1]<=90)) && (grid[i][j+1] == '#' || (grid[i][j+1]>=65 && grid[i][j+1]<=90)) && (grid[i+1][j] == '#' || (grid[i+1][j]>=65 && grid[i+1][j]<=90)))){
                    printf("Intersection point at index [%d][%d]\n", i, j);
                }
            }
        }
    }
}
