#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;

int main() {


    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir("."); //opening current directory

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL) {
        printf("%s\n", de->d_name);
        printf("%d\n", de->d_type);
        printf("\n");

    }

    closedir(dr);




    return 0;
}
