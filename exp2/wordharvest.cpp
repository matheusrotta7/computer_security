#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;



void visit_directory (DIR* dr, char base_dir[]) {

    struct dirent* de;  // Pointer for directory entry

    char aux[200];
    // strcpy(aux, base_dir);


    while ((de = readdir(dr)) != NULL) {
        printf("%s\n", de->d_name);
        if ((int)de->d_type == 4) { //it's a directory
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) { //if it's not the current directory nor the previous one
                char next_dir_str[200];
                strcpy(aux, base_dir); //use aux so as to not ruin base_dir value
                strcpy(next_dir_str, strcat(strcat(aux, "/"), de->d_name)); //next_dir_str = base_dir/next_dir
                DIR* next_dir = opendir(next_dir_str);
                if (next_dir == NULL) {
                    printf("Could not open current directory: %s\n", next_dir_str);
                    return;
                }
                printf("making recursive call to visit directory: %s\n", next_dir_str);
                visit_directory (next_dir, next_dir_str);
            }
        }
        else if ((int)de->d_type == 8) { //it's a file
            //we must check their extensions before harvesting words
            char* pch;
            char save_pch[200];
            int counter = 0;
            pch = strtok (de->d_name, ".");
            while (pch != NULL) {
                counter++;
                strcpy(save_pch, pch);
                // printf ("%s\n", pch);
                pch = strtok (NULL, ".");
            }
            printf("save_pch: %s\n", save_pch);
            printf("counter: %d\n", counter);
            if (counter >= 2) { //if counter < 2, there was no '.' in the file name, so we don't want it
                
            }
        }
    }
}

int main() {




    // opendir() returns a pointer of DIR type.
    DIR* dr = opendir("."); //opening current directory

    if (dr == NULL) {  // opendir returns NULL if couldn't open directory
        printf("Could not open current directory" );
        return 0;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    char initial_base_dir[100];
    strcpy(initial_base_dir, ".");
    visit_directory (dr, initial_base_dir);

    // vector<struct dirent*> to_be_visitted;


    closedir(dr);




    return 0;
}
