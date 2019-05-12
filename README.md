# computer_security

This repository will contain the experiments (and maybe some of the research) that
I made during the MC942 course at Unicamp.  
Experiment 1 was about simulating a backdoor on a VM and running a program on it.  
Experiment 2 is a program in c++ (look at wordharvest.cpp only, the other files
are intermediate steps only) that recursively visits directories and opens the
files with specific extensions to assemble a word dictionary.  
Compile it with g++ wordharvest.cpp -o wh, and to run it, use:  
path_to_wh -e file_extension1:f_ext2:f_ext3:...: -d desired_directory -o output_file  
