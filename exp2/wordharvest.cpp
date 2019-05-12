#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;



void visit_directory (DIR* dr, char base_dir[], unordered_set<string> &desired_extensions, unordered_set<string> &harvested_words, char output_file[]) {

    struct dirent* de;  // Pointer for directory entry

    char aux[200];
    // strcpy(aux, base_dir);


    while ((de = readdir(dr)) != NULL) {
        // printf("%s\n", de->d_name);
        if ((int)de->d_type == 4) { //it's a directory
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) { //if it's not the current directory nor the previous one
                char next_dir_str[200];
                strcpy(aux, base_dir); //use aux so as to not ruin base_dir value
                strcpy(next_dir_str, strcat(strcat(aux, "/"), de->d_name)); //next_dir_str = base_dir/next_dir
                DIR* next_dir = opendir(next_dir_str);
                if (next_dir == NULL) {
                    printf("Could not open current directory: %s\n", next_dir_str);
                    continue;
                }
                // printf("making recursive call to visit directory: %s\n", next_dir_str);
                visit_directory (next_dir, next_dir_str, desired_extensions, harvested_words, output_file);
            }
        }
        else if ((int)de->d_type == 8) { //it's a file
            //we must check their extensions before harvesting words
            //something bad is happening, strtok is trimming the de->d_name string
            //let's use an aux string to fix this
            char aux_dname[200];
            strcpy (aux_dname, de->d_name);
            char* pch;
            char save_pch[200];
            int counter = 0;
            // printf("%s\n", de->d_name);
            pch = strtok (aux_dname, ".");
            // printf("%s\n", de->d_name);
            while (pch != NULL) {
                counter++;
                strcpy(save_pch, pch);
                // printf ("%s\n", pch);
                pch = strtok (NULL, ".");
            }
            // printf("save_pch: %s\n", save_pch);
            // printf("counter: %d\n", counter);
            if (counter >= 2) { //if counter < 2, there is no '.' in the file name, so we don't want it
                //at this point, save_pch should have the file extension
                //check if it is a desired one
                if (desired_extensions.find(save_pch) != desired_extensions.end()) {
                    //then it is a desired one
                    //let's only report it for this time:
                    printf("The file %s is a desired one, because it has the extension %s\n", de->d_name, save_pch);
                    //let's open the file and parse it
                    //first we need to assemble the full path (full_name)
                    char aux[200];
                    strcpy(aux, base_dir); //use aux so as to not ruin base_dir value
                    char full_name[200];
                    strcpy(full_name, strcat(strcat(aux, "/"), de->d_name));
                    FILE* fp_in;
                    FILE* fp_out;
                    fp_in = fopen (full_name, "r");
                    if (fp_in == NULL) {
                        printf("tried to open file %s, but it failed\n", full_name);
                        printf("ABORTING...\n");
                        exit(0);
                    }
                    fp_out = fopen (output_file, "a");
                    char cur_str[200];
                    while (fscanf(fp_in, "%s", cur_str) != EOF) {
                        //iterate over current file
                        //use same token logic to separate strings
                        char* pch;
                        pch = strtok (cur_str, "'! ,.-():;?_\"@");
                        while (pch != NULL) {
                            //if there are still strings, see if they can be inserted in the result file
                            if (harvested_words.find(pch) == harvested_words.end()) { //then we know it's not on the hash
                                fprintf(fp_out, "%s\n", pch);
                                harvested_words.insert(pch); //don't forget to insert :)
                            }
                            pch = strtok (NULL, "'! ,.-():;?_\"@");
                        }


                    }
                    fclose(fp_in);
                    fclose(fp_out);
                }
                else {
                    printf("The file %s is NOT a desired one, because it has the extension %s\n", de->d_name, save_pch);
                }
            }
        }
    }
}


int main(int argc, char *argv[]) {

    cout << "Number of arguments: " << argc << '\n';

    //we should receive 5 or 7 arguments, nothing more, nothing less
    if (!(argc == 5 || argc == 7)) {
        end:
        cout << "Usage: wordharvest -d search_dir -o output_file\n";
        cout << "Optional: wordharvest -e file_extension1:file_extension2:file_extension3... -d search_dir -o output_file\n";
        cout << "Default file extensions: .txt and .text\n";
        return 0;
    }
    // for (int i = 0; i < argc; i++) {
    //
    // }
    int num_flags = (argc - 1)/2;


    bool provided_d = false;
    bool provided_o = false;

    //let's build a hash with the desired extensions
    unordered_set<string> desired_extensions;
    // desired_extensions.insert("text");
    // desired_extensions.insert("txt");
    // desired_extensions.insert("doc");
    // desired_extensions.insert("asc");

    char directory[200];
    char output_file[200];

    for (int i = 0; i < num_flags; i++) {
        if (strlen(argv[2*i+1]) == 2) {
            if (argv[2*i+1][1] == 'e') {
                if (strlen(argv[2*i+2]) > 1) {
                    char* pch;
                    pch = strtok (argv[2*i+2], ":");
                    while (pch != NULL) {
                        desired_extensions.insert(pch);
                        pch = strtok (NULL, ".");
                    }
                }
                else {
                    goto end;
                }
            }
            else if (argv[2*i+1][1] == 'd') {
                strcpy(directory, argv[2*i+2]);
                provided_d = true;
            }
            else if (argv[2*i+1][1] == 'o') {
                strcpy(output_file, argv[2*i+2]);
                provided_o = true;
            }
            else {
                goto end;
            }
        }
        else {
            goto end;
        }
    }
    if (!(provided_d && provided_o)) {
        goto end;
    }
    if (desired_extensions.size() == 0) {
        goto end;
    }


    char touch_str[200];
    strcpy(touch_str, "touch ");
    system(strcat(touch_str, output_file));

    char rm_str[200];
    strcpy(rm_str, "rm ");
    system(strcat(rm_str, output_file));



    //let's build another hash with the words we have already encountered
    //it will be used as criteria to know whether to add word to result file or not
    unordered_set<string> harvested_words;



    // opendir() returns a pointer of DIR type.
    DIR* dr = opendir(directory); //opening desired directory

    if (dr == NULL) {  // opendir returns NULL if couldn't open directory
        printf("Could not open directory %s\n", directory);
        printf("ABORTING...\n");
        return 0;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    visit_directory (dr, directory, desired_extensions, harvested_words, output_file);

    // vector<struct dirent*> to_be_visitted;


    closedir(dr);




    return 0;
}
