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
    

    fp = fopen("8k-16bWhenIWasYourMan.csv","r");
    fptr = fopen("8k-16bWhenIWasYourMan.txt", "w");

    fprintf(fptr, "{");

    while (feof(fp) != true){
        fscanf(fp, "%s", row);
        //printf("Row: %s", row);

        //grab new row of items
        //token = strtok(row, ",0x");

        //while(token != NULL){
        fprintf(fptr, "%s, ", row);
        //fprintf(fptr, *row);
        items++;
        //}

    }

    fprintf(fptr, "};");
    
    fclose(fptr);
    fclose(fp);

    printf("there are %d items", items);
    //after conversion, do find and replace:
    // replace every , with ,0x also remember to append 0x to first and remove from last entry

    return 0;

}
