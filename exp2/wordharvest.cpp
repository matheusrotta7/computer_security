#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;

long int global_num_of_directories = 0;
long int global_num_of_files_seen = 0;
long int global_num_of_files_correct_extension = 0;
long int global_num_of_words_harvested = 0;

long int MAX_NUM_WORDS = 5000000;

void visit_directory(DIR* dr, string& base_dir, unordered_set<string>& desired_extensions, unordered_set<string>& harvested_words, const string& output_file) {
    struct dirent* de;

    while ((de = readdir(dr)) != NULL) {
        if ((int)de->d_type == 4) { // directory
            string dname = de->d_name;
            if (dname != "." && dname != "..") {
				if (base_dir[base_dir.length() - 1] == '/') {
					base_dir = base_dir.substr(0, base_dir.length()-1); //remove trailing slash if necessary
				}
                string next_dir_str = base_dir + "/" + dname;
                DIR* next_dir = opendir(next_dir_str.c_str());
                global_num_of_directories++;
                if (next_dir == NULL) {
                    cout << "Could not open current directory: " << next_dir_str << endl;
                    continue;
                }
                visit_directory(next_dir, next_dir_str, desired_extensions, harvested_words, output_file);
            }
        } else if ((int)de->d_type == 8) { // file
            global_num_of_files_seen++;
            string filename = de->d_name;
            stringstream ss(filename);
            string token, last_token;
            while (getline(ss, token, '.')) {
                last_token = token;
            }

            if (filename.find('.') != string::npos && desired_extensions.find(last_token) != desired_extensions.end()) {
                global_num_of_files_correct_extension++;

                string full_name = base_dir + "/" + filename;

                FILE* fp_in = fopen(full_name.c_str(), "r");
                if (fp_in == NULL) {
                    cout << "Tried to open file " << full_name << ", but it failed\nCONTINUING...\n";
                    continue;
                }

                FILE* fp_out = fopen(output_file.c_str(), "a");
                char cur_str[10000];
                while (fscanf(fp_in, "%s", cur_str) != EOF) {
                    char* pch = strtok(cur_str, "'! ,.-():;?_\"@");
                    while (pch != NULL) {
                        if (harvested_words.find(pch) == harvested_words.end()) {
                            fprintf(fp_out, "%s\n", pch);
                            if (global_num_of_words_harvested > MAX_NUM_WORDS) {
                                cout << "Reached max number of words, deduplication no longer assured\n";
                            } else {
                                harvested_words.insert(pch);
                            }
                            global_num_of_words_harvested++;
                        }
                        pch = strtok(NULL, "'! ,.-():;?_\"@");
                    }
                }
                fclose(fp_in);
                fclose(fp_out);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (!(argc == 5 || argc == 7)) {
        end:
        cout << "Usage: wordharvest -d search_dir -o output_file\n";
        cout << "Optional: wordharvest -e file_extension1:file_extension2:... -d search_dir -o output_file\n";
        cout << "Default file extensions: .txt and .text\n";
        return 0;
    }

    int num_flags = (argc - 1) / 2;
    bool provided_d = false;
    bool provided_o = false;

    unordered_set<string> desired_extensions;
    string directory;
    string output_file;

    for (int i = 0; i < num_flags; i++) {
        string flag = argv[2 * i + 1];
        string value = argv[2 * i + 2];
        if (flag.size() == 2) {
            if (flag[1] == 'e') {
                if (!value.empty()) {
                    stringstream ss(value);
                    string ext;
                    while (getline(ss, ext, ':')) {
                        desired_extensions.insert(ext);
                    }
                } else {
                    goto end;
                }
            } else if (flag[1] == 'd') {
                directory = value;
                provided_d = true;
            } else if (flag[1] == 'o') {
                output_file = value;
                provided_o = true;
            } else {
                goto end;
            }
        } else {
            goto end;
        }
    }

    if (!(provided_d && provided_o)) {
        goto end;
    }
    if (desired_extensions.empty()) {
        goto end;
    }

    system(("touch " + output_file).c_str());
    system(("rm " + output_file).c_str());

    unordered_set<string> harvested_words;

    DIR* dr = opendir(directory.c_str());
    global_num_of_directories++;

    if (dr == NULL) {
        cout << "Could not open directory " << directory << "\nABORTING...\n";
        return 0;
    }

    visit_directory(dr, directory, desired_extensions, harvested_words, output_file);
    closedir(dr);

    cout << "\n\n******************STATS*******************\n\n";
    cout << "Number of directories visited: " << global_num_of_directories << '\n';
    cout << "Number of files 'seen': " << global_num_of_files_seen << '\n';
    cout << "Number of files with the correct extension: " << global_num_of_files_correct_extension << '\n';
    cout << "Number of words harvested: " << global_num_of_words_harvested << '\n';
    cout << "\n***************END OF STATS****************\n\n";

    return 0;
}

