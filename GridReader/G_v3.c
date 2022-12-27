#include <stdio.h>
#include <string.h>

//structure for store horizontal block information
typedef struct _{
    int place;
    int index[1][2];
    int length;
    int direct;
    char setup[100];
    char conwho[100];
    char conid[100];
}h_space;

typedef struct __{
    char conwhoHorizon[100][100];
}stepGrid;

//structure for store vertical block information
// typedef struct  __{
//     int index[1][2];
//     int length;
//     char setup[100];
// }v_space;



int horizonSpace(char grid[][100], int k, h_space vals[], int n, stepGrid keep[10]); //funtion to get the horizontal block information
int vertSpace(char grid[][100], int k, h_space vals[], int m, stepGrid keep[10]); //function to get the vertical block information
void intersect(char grid[][100], int k); //function to get the intersection points of the grid

int interHorizon(stepGrid keep[], h_space vals[], int m){
    for (int i=0; i<m; ++i){
        int temp = 0;
        for (int j=vals[i].index[0][1]; j < vals[i].index[0][1] + vals[i].length; ++j){
            if (keep[0].conwhoHorizon[vals[i].index[0][0]][j] == i){
                vals[i].conwho[temp] = -1;
            }
            else{
                vals[i].conwho[temp] = keep[0].conwhoHorizon[vals[i].index[0][0]][j];
            }
            ++temp;
            
        }
        for (int j=0; j<temp; ++j){
            printf("%2d, ",vals[i].conwho[j]);
        }
        printf("\n");
    }
    //return vals;
}

int interVert (stepGrid keep[], h_space vals[], int m, int n){
    for (int i=0; i<100; ++i){
        for (int j=0; j<100; ++j){
            keep[1].conwhoHorizon[i][j] = keep[0].conwhoHorizon[i][j];
        }
    }

    for (int i=0; i<m; ++i){
        for (int j=vals[i].index[0][1]; j < vals[i].index[0][1] + vals[i].length; ++j){
            keep[1].conwhoHorizon[vals[i].index[0][0]][j] = vals[i].place;
        }
    }

    for (int i=m; i<m+n; ++i){
        int temp = 0;
        for (int j=vals[i].index[0][0]; j < vals[i].index[0][0] + vals[i].length; ++j){
            if (keep[1].conwhoHorizon[j][vals[i].index[0][1]] == i){
                vals[i].conwho[temp] = -1;
            }
            else{
                vals[i].conwho[temp] = keep[1].conwhoHorizon[j][vals[i].index[0][1]];
            }
            ++temp;
            
        }

        for (int j=0; j<temp; ++j){
            printf("%2d, ",vals[i].conwho[j]);
        }
        printf("\n");
    }
}

int conHorizon(stepGrid keep[], h_space vals[], int m, int n){
    for (int i=0; i<100; ++i){
        for (int j=0; j<100; ++j){
            keep[2].conwhoHorizon[i][j] = keep[0].conwhoHorizon[i][j];
        }
    }

    for (int i=m; i<m+n; ++i){
        int temp = 0;
        for (int j=vals[i].index[0][0]; j < vals[i].index[0][0] + vals[i].length; ++j){
            keep[2].conwhoHorizon[j][vals[i].index[0][1]] = temp;
            ++temp;
        }
    }

    for (int i=0; i<m; ++i){
        int temp = 0;
        for (int j=vals[i].index[0][1]; j < vals[i].index[0][1] + vals[i].length; ++j){
            if (keep[0].conwhoHorizon[vals[i].index[0][0]][j] == i){
                vals[i].conid[temp] = -1;
            }
            else{
                vals[i].conid[temp] = keep[2].conwhoHorizon[vals[i].index[0][0]][j];
            }
            ++temp;
            
        }
        for (int j=0; j<temp; ++j){
            printf("%2d, ",vals[i].conid[j]);
        }
        printf("\n");
    }

}

int conVert (stepGrid keep[], h_space vals[], int m, int n){
    for (int i=0; i<100; ++i){
        for (int j=0; j<100; ++j){
            keep[3].conwhoHorizon[i][j] = keep[1].conwhoHorizon[i][j];
        }
    }

    for (int i=0; i<m; ++i){
        int temp = 0;
        for (int j=vals[i].index[0][1]; j < vals[i].index[0][1] + vals[i].length; ++j){
            keep[3].conwhoHorizon[vals[i].index[0][0]][j] = temp;
            ++temp;
        }
    }

    for (int i=m; i<m+n; ++i){
        int temp = 0;
        for (int j=vals[i].index[0][0]; j < vals[i].index[0][0] + vals[i].length; ++j){
            if (keep[3].conwhoHorizon[j][vals[i].index[0][1]] == i){
                vals[i].conid[temp] = -1;
            }
            else{
                vals[i].conid[temp] = keep[3].conwhoHorizon[j][vals[i].index[0][1]];
            }
            ++temp;
            
        }

        for (int j=0; j<temp; ++j){
            printf("%2d, ",vals[i].conid[j]);
        }
        printf("\n");
    }
}

int main(){

    int m=0, n=0; //wadiya hithanna epa :)
    h_space vals[100]; //array which store the horizontal block information
    //v_space vals[100]; //array which store the vertical block information

    stepGrid keep[10];

    char grid[100][100]; //to store the given ccrossword puzzle grid
    char transpose[100][100]; //to read the grid columnwise
    int k = 0;

    printf("Enter the grid: \n");
    while (1 == scanf("%[^\n]%*c", grid[k])){
        ++k;
    }

    // obtaining the transpose of the grid
    // for (int i=0; i<=k; ++i){
    //     for (int j=0; j<100; ++j){
    //         transpose[j][i] = grid[i][j];
    //     }
    // }

    //calling funtions
    m = horizonSpace(grid, k, vals, n, keep );
    n = vertSpace(grid, k, vals, m, keep);
    intersect(grid, k);

    printf("\n");

    //printing the stored values in the stucture created for horizontal blocks
    for (int i = 0; i<m; ++i){
        printf("The horizontal space at index [%d][%d] is %d spaces long and the format of the block is %s\n",vals[i].index[0][0], vals[i].index[0][1], vals[i].length, vals[i].setup);
    }
    printf("\n");

    //printing the stored values in the stucture created for vertical blocks
    for (int i = m; i<m+n; ++i){
        printf("The vertical space at index [%d][%d] is %d spaces long and the format of the block is %s\n",vals[i].index[0][0], vals[i].index[0][1], vals[i].length, vals[i].setup);
    }

    for (int i=0; i<=k; ++i){
        for (int j=0; j<100; ++j){
            printf("%d", keep[0].conwhoHorizon[i][j]);
        }
        printf("\n");
    }

    interHorizon(keep, vals, m);

    // for (int i=0; i<m; ++i){
    //     for (int j=0; j<10; ++j){
    //         printf("%2d, ",vals[i].conwho[j]);
    //     }
    //     printf("\n");
    // }
    
    printf("************\n");
    interVert(keep, vals, m, n);
    printf("************\n");

    conHorizon(keep, vals, m, n);

    printf("************\n");

    conVert(keep, vals, m, n);

    for (int i=0; i<=k; ++i){
        for (int j=0; j<100; ++j){
            printf("%d", keep[1].conwhoHorizon[i][j]);
        }
        printf("\n");
    }

}

//funtion to get the horizontal block information
int horizonSpace(char grid[][100], int k, h_space vals[], int n, stepGrid keep[10]){
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
                int p;
                vals[m+n].direct = 1;
                vals[m+n].place = m+n;
                //printf("THe horizontal space at index [%d][%d] is %d spaces long\n",I, J, count_h); //use this printf() statement for direct reading purposes
                vals[m+n].length = count_h;
                vals[m+n].index[0][0] = I;
                vals[m+n].index[0][1] = J;
                int c = 0;
                for (int j=J; j<J+count_h; ++j){
                    temp[c] = grid[I][j];
                    keep[0].conwhoHorizon[I][j] = m+n;
                    ++c;
                }
                strcpy(vals[m+n].setup, temp);
                memset(&temp[0], 0, sizeof(temp)); //emptying the temp array for the next iteration
                count_h = 0;
                ++m;
            }

            if((grid[i][j] != '#' && (grid[i][j] < 65 || grid[i][j] > 90))){
                count_h = 0;
            }
        }
    }
    // for (int i=0; i<=k; ++i){
    //     for (int j=0; j<100; ++j){
    //         printf("%d", keep[0].conwhoHorizon[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    return vals, keep, m ;
    
}

//function to get the vertical block information
int vertSpace(char grid[][100], int k, h_space vals[], int m, stepGrid keep[10]){
    int n =0;
    char temp_n[100];
    for (int i = 0; i<100; ++i){
        int count_v = 0, X, Y;
        for (int j = 0; j <100; ++j){
            if (grid[j][i] == '#' || (grid[j][i] >=65 && grid[j][i] <=90)){
                ++count_v;
                if (count_v == 1){
                    Y = j;
                    X = i;
                }
            }

            if (count_v > 1 && (grid[j][i] != '#' && (grid[j][i] < 65 || grid[j][i] > 90))){
                vals[m+n].direct = 0;
                //printf("The vertical space at index [%d][%d] is %d spaces long\n", X, Y, count_v); //use this for direct printing purposes
                vals[m+n].length = count_v;
                vals[m+n].index[0][0] = Y;
                vals[m+n].index[0][1] = X;
                int c = 0;
                for (int j=Y; j<Y+count_v; ++j){
                    temp_n[c] = grid[j][X];
                    keep[0].conwhoHorizon[j][X] = m+n;
                    ++c;
                }
                strcpy(vals[m+n].setup, temp_n);
                memset(&temp_n[0], 0, sizeof(temp_n)); //emptying the temp array for the next iteration
                count_v = 0;
                ++n;
            }

            if((grid[j][i] != '#' && (grid[j][i] < 65 || grid[j][i] > 90)) ){
                count_v = 0;
            }
        }
    }
    // for (int i=0; i<=k; ++i){
    //     for (int j=0; j<100; ++j){
    //         printf("%d", keep[0].conwhoHorizon[i][j]);
    //     }
    //     printf("\n");
    // }

    return keep, vals, n;
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

int tarr(char grid[][100], int i, int j){
    if (grid[i][j] == '#' || (grid[i][j] >=65 && grid[i][j] <=90)){
                if (((grid[i][j-1] == '#' || (grid[i][j-1]>=65 && grid[i][j-1]<=90)) && (grid[i-1][j] == '#' || (grid[i-1][j]>=65 && grid[i-1][j]<=90)) && (grid[i+1][j] == '#' || (grid[i+1][j]>=65 && grid[i+1][j]<=90))) || 
                    ((grid[i][j+1] == '#' || (grid[i][j+1]>=65 && grid[i][j+1]<=90)) && (grid[i-1][j] == '#' || (grid[i-1][j]>=65 && grid[i-1][j]<=90)) && (grid[i+1][j] == '#' || (grid[i+1][j]>=65 && grid[i+1][j]<=90))) || 
                    ((grid[i][j-1] == '#' || (grid[i][j-1]>=65 && grid[i][j-1]<=90)) && (grid[i][j+1] == '#' || (grid[i][j+1]>=65 && grid[i][j+1]<=90)) && (grid[i-1][j] == '#' || (grid[i-1][j]>=65 && grid[i-1][j]<=90))) || 
                    ((grid[i][j-1] == '#' || (grid[i][j-1]>=65 && grid[i][j-1]<=90)) && (grid[i][j+1] == '#' || (grid[i][j+1]>=65 && grid[i][j+1]<=90)) && (grid[i+1][j] == '#' || (grid[i+1][j]>=65 && grid[i+1][j]<=90)))){
                    return -1;
                }
            }

}

