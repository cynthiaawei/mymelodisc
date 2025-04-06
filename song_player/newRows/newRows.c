#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define MAXCHAR 1000

// note: need to manually find & replace all double commas ,,

int main(){

    FILE *fp, *fptr;
    char row[MAXCHAR];
    char *token;
    bool first = true;
    int items = 0;
    
    // int start = 928000; // APT: 1:56-2:34
    // int end = 1232000; // APT

    int start = 0;          // Beauty and a beat
    int end = (60+15)*8000; // Beauty and a beat

    // int start = (2*60+13)*8000; // Birds of a feather
    // int end = (2*60+50)*8000;   // Birds of a feather

    // int start = 0;              // Good luck babe
    // int end = (60+10)*8000;     // Good luck babe

    // int start = 0;              // When I was your man
    // int end = (60+17)*8000;     // When I was your man
    

    fp = fopen("8k-16bBeautyAndTheBeat.txt","r");
    fptr = fopen("8k-16bBeautyAndTheBeat_rows.txt", "w");

    //fprintf(fptr, "{");
    bool cont = true;
    while (cont && feof(fp) != true){
        fscanf(fp, "%s,", row);
        //printf("Row: %s", row);

        //grab new row of items
        token = strtok(row, ",");

        while(token != NULL){
            //cropping audio
            if(items >= start && items < end){
                fprintf(fptr, "0x%s,\n", token);
            }
            token = strtok(NULL, ",");

            //fprintf(fptr, row);
            items++;
        }
        // if(items >= 60000){
        //     cont = false;
        // }

    }

    //fprintf(fptr, "};");
    
    fclose(fptr);
    fclose(fp);

    printf("there are %d total items in song  \n", items);
    printf("there are %d total items in array \n", end - start);
    //after conversion, do find and replace:
    // replace every , with ,0x also remember to append 0x to first and remove from last entry

    return 0;

}
