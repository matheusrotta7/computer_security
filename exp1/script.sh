echo "#include<stdio.h>

int main() {
    int i;
    while(1) {
        printf(\"%d\", i++);
    }



    return 0;
}
" > inf.c
gcc inf.c -o inf
./inf
