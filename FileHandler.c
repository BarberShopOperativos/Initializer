#include <stdio.h>

/// <summary>
/// Method to delete and write on a specified file
/// </summary>
void writeFile(char *pText){
    char *fileName = "output.txt";
    FILE *file = fopen(fileName, "w");
    if (file == NULL){
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(file, "%s", pText);
    fclose(file);
}

/// <summary>
/// Method to append text on a specified file
/// </summary>
void writeFileAppend(char *pText){
    char *fileName = "output.txt";
    FILE *file = fopen(fileName, "a");
    if (file == NULL){
        printf("Error opening file!\n");
        exit(1);
    }
    //if(ftrylockfile(file) == 0){
        fprintf(file, "%s\n", pText);
        //funlockfile(file);
        fclose(file);
   // }

}
