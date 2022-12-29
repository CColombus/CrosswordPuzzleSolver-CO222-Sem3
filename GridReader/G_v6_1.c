#include <stdio.h>
#include <string.h>

//structure for store horizontal block information
typedef struct _{
    int place; //to store the index of the space when stored in space[]
    int index[1][2]; //starting index of a space
    int length; //length of the space
    int direct; //direction of the space(horizontal=1 , vertical=0)
    char setup[100]; //format of the space
    char conwho[100]; //with which spaces does a certain space intersect with
    char conid[100]; //if the space intersect with another space which indexes of that space does it intersect with
}sspace;

typedef struct stepGrid{
    char conwhoHorizon[100][100];
}stepGrid;

int horizonSpace(char grid[][100], int k, sspace vals[], int n, stepGrid keep[10]); //funtion to get the horizontal block information
int vertSpace(char grid[][100], int k, sspace vals[], int m, stepGrid keep[10]); //function to get the vertical block information
int interHorizon(stepGrid keep[], sspace vals[], int m); //function which obtain the "conwho" details of horizontal blocks
int interVert (stepGrid keep[], sspace vals[], int m, int n); //function which obtain the "conwho" details of vertical blocks
int conHorizon(stepGrid keep[], sspace vals[], int m, int n); //function which obtain the "conid" details of horizontal blocks
int conVert (stepGrid keep[], sspace vals[], int m, int n);   //function which obtain the "conid" details of vertical blocks

sspace space[100]; //array which store the block information


int main(){

    int m=0, n=0; 
    

    stepGrid keep[10];
    /*
    keep[0] = formatted grid for obtain "conwho" details of horizontal blocks
    keep[1] = formatted grid for obtain "conwho" details of vertical blocks
    keep[2] = formatted grid for obtain "conid" details of horizontal blocks
    keep[3] = formatted grid for obtain "conid" details of vertical blocks
    */

    char grid[100][100]; //to store the given ccrossword puzzle grid
    int k = 0;

    printf("Enter the grid: \n");
    while (1 == scanf("%[^\n]%*c", grid[k])){
        ++k;
    }

    //calling funtions
    m = horizonSpace(grid, k, space, n, keep );
    n = vertSpace(grid, k, space, m, keep);

    interHorizon(keep, space, m); 
    interVert(keep, space, m, n);
    conHorizon(keep, space, m, n);
    conVert(keep, space, m, n);

    //printing the stored values in the stucture created for horizontal blocks
    for (int i = 0; i<m; ++i){
        printf("The horizontal space at index [%d][%d] is %d spaces long and the format of the block is %s\n",space[i].index[0][0], space[i].index[0][1], space[i].length, space[i].setup);
        printf("The conwholist = ");
        for (int j=0; j<10; ++j){
            printf("%2d, ",space[i].conwho[j]);
        }
        printf("\n");
        printf("The conidlist = ");
        for (int j=0; j<10; ++j){
            printf("%2d, ",space[i].conid[j]);
        }
        printf("\n");
    }

    printf("\n");

    //printing the stored values in the stucture created for vertical blocks
    for (int i = m; i<m+n; ++i){
        printf("The vertical space at index [%d][%d] is %d spaces long and the format of the block is %s\n",space[i].index[0][0], space[i].index[0][1], space[i].length, space[i].setup);
        printf("The conwholist = ");
        for (int j=0; j<10; ++j){
            printf("%2d, ",space[i].conwho[j]);
        }
        printf("\n");
        printf("The conidlist = ");
        for (int j=0; j<10; ++j){
            printf("%2d, ",space[i].conid[j]);
        }
        printf("\n");
    }
}

//funtion to get the horizontal block information
int horizonSpace(char grid[][100], int k, sspace vals[], int n, stepGrid keep[10]){
    int m = 0;
    char temp[100];
    for (int i = 0; i<=k; ++i){
        int count_h = 0, I, J;
        for (int j = 0; j <100; ++j){
            if (grid[i][j] == '#' || (grid[i][j] >=65 && grid[i][j] <=90)){ 
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
    return vals, keep, m ;
    
}

//function to get the vertical block information
int vertSpace(char grid[][100], int k, sspace vals[], int m, stepGrid keep[10]){
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
    return keep, vals, n;
}

int interHorizon(stepGrid keep[], sspace vals[], int m){
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
    }
}

int interVert (stepGrid keep[], sspace vals[], int m, int n){
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
    }
}

int conHorizon(stepGrid keep[], sspace vals[], int m, int n){
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
            if (keep[2].conwhoHorizon[vals[i].index[0][0]][j] == i && keep[2].conwhoHorizon[vals[i].index[0][0] + 1][j] == 0){
                vals[i].conid[temp] = -1;
            }
            else{
                vals[i].conid[temp] = keep[2].conwhoHorizon[vals[i].index[0][0]][j];
            }
            ++temp;    
        }
    }
}

int conVert (stepGrid keep[], sspace vals[], int m, int n){
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
            if (keep[3].conwhoHorizon[j][vals[i].index[0][1]] == i && (keep[3].conwhoHorizon[j][vals[i].index[0][1]+1] == 0)){
                vals[i].conid[temp] = -1;
            }
            else{
                vals[i].conid[temp] = keep[3].conwhoHorizon[j][vals[i].index[0][1]];
            }
            ++temp;    
        }
    }
}
