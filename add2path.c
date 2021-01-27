#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{	

	//process without argument
    if(argv[1] == NULL) {
		//get current path directory
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("Path of current directory: %s\n\n", cwd);
			//execute add2path with current dir
			add2path(cwd);
        }
        else
        {
            perror("Error getting path directory");
            return -1;
        }
    } else {
        DIR *dir = opendir(argv[1]);
		//check if path is valid
        if(dir) {
			printf("Current path directory: %s\n\n", argv[1]);
            add2path(argv[1]);
        } else if(ENOENT == errno) {
			perror("Invalid directory.");
            return -1;
        } else {
			perror("opendir() failed");
			return -1;
		}
    }
    return 0;
}

int add2path(char const *path) {
	
    char name[PATH_MAX] = "";
    char buf[BUFSIZ] = "";

	//declare buffer and size
    char *buffer;
    size_t bufsize = 100;
	
	//allocate memory
    buffer = (char *)malloc(bufsize * sizeof(char));

	//prepare command .bashrc
    sprintf (name, "%s/%s", getenv("HOME"), ".bashrc");

    FILE *file = fopen(name, "r");
    FILE *temp =  fopen("temp", "w"); 
	
	//check if file is valid
    if(file != NULL) {
        int exportFound = 0;
        while(getline(&buffer, &bufsize, file) > 0) {
             if(exportFound = strstr(buffer, "export PATH=")!=NULL) {
                char *firstOccur = (char *) malloc(strlen(path)*sizeof(char));
                firstOccur = strstr(buffer, path);
                if(firstOccur==NULL) { 
                    //add dir to path
                    //removing two lat chars 
                    buffer[strlen(buffer)-2] = '\0';

                    strcat(buffer, ":");
                    strcat(buffer, path);
                    strcat(buffer, "\"\n");

                    fputs(buffer, temp);
                    printf("Added %s to PATH! \n", path);
                    break;
                } else { 
                    int start = firstOccur - buffer-1;
                    int end = firstOccur - buffer + strlen(path);

                    char *beforeCurrentPath = (char*) malloc((strlen(buffer)-strlen(path))*sizeof(char));
                    char *afterCurrentPath = (char*) malloc(strlen(buffer)*sizeof(char));
                    memcpy(beforeCurrentPath, buffer, start);
                    memcpy(afterCurrentPath, buffer+end, strlen(buffer));

                    strcat(beforeCurrentPath, afterCurrentPath);
                    fputs(beforeCurrentPath, temp);
                    printf("Removed %s from PATH! \n", path);
                }
                
            } else {
                fputs(buffer, temp);
            }
        }

        if(exportFound == 0) {
            char pathString[300] = "export PATH=\"$PATH:";
            strcat(pathString, path);
            strcat(pathString, "\"\n");
            fputs(pathString, temp);
            printf("Created PATH with %s in .bashrc! \n", path);
        }

        fclose(file);
        fclose(temp);


        /* Delete original source file */
        remove(name);

        /* Rename temporary file as original file */
        rename("temp", name);
    } else {
        perror("cannot open .bashrc");
        return 1;
    }

}

