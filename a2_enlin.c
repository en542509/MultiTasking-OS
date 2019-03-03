#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define ROWS 9
#define COLS 9
#define num_thread 27

int matrix[ROWS][COLS] = {0};

void *rows_check (void *param);
void *cols_check (void *param);
void *subgrids_check (void *param);

void getMatrix(){
    int i =0, j=0;
    FILE* fp = fopen("os_26.txt", "r");
    char  *token, buffer[100];

    if (fp == NULL){
        printf("Error.\n");
        exit(1);
    }

    while (fscanf(fp, "%s", buffer) != EOF ) {
        //fscanf(fp, "%s", buffer);
        token = strtok(buffer, " ");

            if (token){
                do{
                    matrix[i][j] = atoi(token);
                    j++;
                } while ( (token = strtok(NULL, " ")) !=NULL);

            }
        }
        fclose(fp);
    }


int validate_entry[num_thread] = {0};

/* structure for passing data to threads */
typedef struct{
    int row;
    int col;
}parameters;

int main() {
    int i,j;
    pthread_t tid[num_thread];
    int count = 0;
    //store the input file matrix to global matrix
    getMatrix();

    for(i = 0; i<ROWS; i++){
        for(j = 0; j< COLS; j++){
            //row checking
            if(j == 0 ){
                parameters *data = (parameters *) malloc(sizeof(parameters));
                data->row = i;
                //printf("%d\t",i);
                data->col = j;
                //printf("%d\n",j);
                pthread_create(&tid[count],NULL, rows_check, (void *)data);
                //printf("%d", pthread_create(&tid[count],NULL, rows_check, data));
                count++;
                //printf("%d\t",count);
            }
            //col checking
            if(i == 0 ){
                parameters *data = (parameters *) malloc(sizeof(parameters));
                data->row = i;
                //printf("%d",i);
                data->col = j;
                //printf("%d\n",j);
                pthread_create(&tid[count],NULL, cols_check, (void *)data);
                //printf("%d", pthread_create(&tid[count],NULL, cols_check, data));
                count ++;
                //printf("%d\t",count);
            }
            if(i % 3 == 0 && j % 3 == 0){
                parameters *data = (parameters *) malloc(sizeof(parameters));
                data->row = i;
                //printf("%d",i);
                data->col = j;
                //printf("%d\n",j);
                pthread_create(&tid[count],NULL, subgrids_check,(void *)data);
                //printf("%d", pthread_create(&tid[count],NULL, subgrids_check, data));
                count ++;
                //printf("%d\t",count);
            }
        }
    }
    //join all threads at the end
    for (int k = 0 ; k< num_thread ; k++){
        pthread_join(tid[k], NULL);
    }

    //checking value of validate_entry
    for (int l = 0; l< num_thread; l++) {
        printf("%d", validate_entry[l]);
    }

    //result checking
    for (int l = 0; l< num_thread; l++){
        if(validate_entry[l] == 0){
            printf("\nInvalid.\n");
            exit(1);
        }
    }
    printf("\nValid.\n");
    return 0;
}

//first 9 threads to check each row
void *rows_check (void *param) {
    parameters *data = (parameters*) param;
    int row = data->row;
    int col = data->col;

    // Check if numbers 1-9 only appear once in the row
    if (row <= 8 && col <= 8) {
        int validation[9] = {0};
        for (int i = 0; i < 9; i++) {
            // If the corresponding index for the number is set to 1, and the number is encountered again,
            // the valid array will not be updated and the thread will exit.
            int behind = matrix[row][i];
            if (validation[behind - 1] == 1) {
                pthread_exit(NULL);
            } else {
                validation[behind - 1] = 1;
            }
        }
    }
    // If reached this point, row subsection is valid.
    validate_entry[row] = 1;
    pthread_exit(NULL);
}


//next 9 threads to check each col
void *cols_check (void *param){
    parameters *data = (parameters *) param;
    int row = data->row;
    int col = data->col;
    //error checking
    // Check if numbers 1-9 only appear once in the row
    if (row <= 8 && col <= 8) {
        int validation[9] = {0};
        for (int i = 0; i < 9; i++) {
            int behind = matrix[i][col];
            // If the corresponding index for the number is set to 1, and the number is encountered again,
            // the valid array will not be updated and the thread will exit.
            if (validation[behind - 1] == 1) {
                pthread_exit(NULL);
            } else {
                validation[behind - 1] = 1;
            }
        }
    }
    // If reached this point, row subsection is valid.
    validate_entry[9+col] = 1;
    pthread_exit(NULL);
}

/*the other 9 threads to check each subgrids */
/* 0---3---6---8  */
/* |(1)|(2)|(3)|  */
/* 3---3---6---3  */
/* |(4)|(5)|(6)|  */
/* 6---3---6---6  */
/* |(7)|(8)|(9)|  */
/* 8---3---6---8  */
/* The index of rows and cols divided by 3 should be equal to 0 */
void *subgrids_check (void *param){
    parameters *data = (parameters *) param;
    int row = data->row;
    int col = data->col;
    //error checking (row and col must be 0, 3 or 6)
    if (col % 3 == 0 && row % 3 == 0 && col <=6 && row <=6){
        int validation[9] = {0};
        for(int i = row; i< row+3;i++){
            for(int j = col; j< col+3;j++){
                int behind = matrix[i][j];
                // If the corresponding index for the number is set to 1, and the number is encountered again,
                // the valid array will not be updated and the thread will exit.
                if (validation[behind - 1] == 1 ) {
                   pthread_exit(0);
                }else{
                    validation[behind - 1] = 1;
                }
            }
        }
    }
    /* (0,0), (0,3),(0,6), (3,0), (3,3), .. (6,6) */
    validate_entry[18 + (row + col/3)] = 1;
    pthread_exit(0);
}
