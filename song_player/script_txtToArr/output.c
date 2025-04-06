#include <stdio.h>
#include <stdbool.h>

int main(void){
    FILE *arrFile = fopen("song_int_array.txt", "r"); // read mode
    FILE *formattedArr = fopen("formatted_song_arr.txt", "w");

    bool first = true;
    char line[256];
    int a = 0;
	
    fputc('{', formattedArr);

	if(arrFile != NULL){
		while(fgets(line, sizeof(line), arrFile) != NULL){
            if(!first){
                int c = (int)',';
                fputc(c, formattedArr);
            } else {
                first = false;
            }
            
            fprintf(formattedArr, line);   
		}
	}

    //fseek(formattedArr, -2, SEEK_CUR);
    fputc('}', formattedArr);

    fclose(arrFile);
    fclose(formattedArr);

	printf("finished counting \n");

    return 0;
}