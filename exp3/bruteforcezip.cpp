#include <bits/stdc++.h>

using namespace std;

int main (int argc, char** argv) {

    //references: https://stackoverflow.com/questions/37305963/capturing-output-and-input-of-interactive-program-with-bash
    //            https://stackoverflow.com/questions/14392525/passing-arguments-to-an-interactive-program-non-interactively
    //your linux will need script and unzip commands in order to run this bruteforce
    //the alternative to this implementation is to reimplement the decryption used in unzip following their guidelines (https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT) see section 6.1
    //that would be much better because there would be no system call overhead, but
    //that would take quite a good amount of work, I will look into it :)
    //  ./bfzip

    char sys_call[400];
    char password[100];
    char cur_str[100];
    char zipfile[100];
    strcpy(zipfile, argv[4]);
    FILE* fp_dictionary = fopen(argv[2], "r");
    if (fp_dictionary == NULL) {
        cout << "The program was unable to open the dictionary file\nAborting...\n";
        return 1;
    }


    while (fscanf(fp_dictionary, "%s", password) != EOF) {
        sprintf(sys_call, "script -q -c \"echo \"A\" | unzip -P %s %s\" log.txt", password, zipfile);
        system(sys_call); //calls, on bash, the attempt to unzip with the current string from the dictionary file
        FILE* fp_log = fopen("log.txt", "r");
        if (fp_log == NULL) {
            cout << "The program was unable to open log.txt file\nAborting...\n";
            return 1;
        }

        /* the criteria used next is simple
          when the password is correct, it will necessarily extract the files,
          so we search for the word 'extracting'. If the password is incorrect,
          we can search for the word 'skipping', which only occurs in an incorrectly
          password log */


        while (fscanf(fp_log, "%s", cur_str) != EOF) { //loop through log.txt file to discover if the password is correct or incorrect
            // cout << "current string is: "<< cur_str << '\n';
            if (strcmp(cur_str, "extracting:") == 0) {
                goto found_password;
            }
            else if (strcmp(cur_str, "skipping:") == 0) {
                break;
            }
        }

    }
    cout << "\n\n\n***The correct password was not contained in the dictionary file\nThat's life...\n";
    goto skip_found_password;

    found_password:
    cout << "\n\n\n***Found correct password: " << password << '\n';

    skip_found_password:
    return 0;
}
