#include <stdio.h>
#include <ctype.h>
#include <string.h>

int take(char inp[] , int n);

int search(char *global_key) {
    FILE *fp = fopen("database.txt", "r");
    if(fp == NULL) {
        printf("No Such File Found\n");
        return 0;
    }

    // Process the key
    int key[50] = {0};
    int key_length = strlen(global_key);
    if(key_length == 0) {
        printf("Error: Unable to Fetch Key");
        fclose(fp);
        return 0;
    }

    int num = 0, no = 0;
    for(int i = 0; i < key_length; i++) {
        if(isdigit(global_key[i])) {
            num = num * 10 + (global_key[i] - '0');
        }
        else if(global_key[i] == '-' || i == key_length-1) {
            key[no] = num;
            num = 0;
            no++;
        }
        else {
            printf("Key Was Tampered");
            fclose(fp);
            return 0;
        }
    }

    // Read and decrypt the database
    char arr[1000][3][500] = {0};
    int i = 0, count = 0;
    char c;
    int field = 0, pos = 0;

    while(1) {
        c = fgetc(fp);
        if(c == EOF) break; //breaks the inf loop

        if(c == '\x1F') { //Separator used
            arr[i][field][pos] = '\0';
            field++;
            pos = 0;
            continue;
        }
        else if(c == '\n') {
            // Record separator
            arr[i][field][pos] = '\0';
            i++;
            field = 0;
            pos = 0;
            count = 0;  // RESET COUNT FOR NEW RECORD
            continue;
        }

        // Decrypt the character
        arr[i][field][pos] = c ^ key[count % no];
        pos++;
        count++;
    }
    fclose(fp);

    // Get search term
    char word[500] = {0};
    printf("Please enter either domain or username: ");
    while(getchar() != '\n'); // Clear buffer
    take(word, sizeof(word));

    // Search the database
    for(int x = 0; x < i; x++) {
        // Compare with domain
        int match = 1;
        for(int k = 0; word[k] != '\0' || arr[x][0][k] != '\0'; k++) {
            if(word[k] != arr[x][0][k]) {
                match = 0;
                break;
            }
        }
        if(match) {
            printf("%s | %s | %s\n", arr[x][0], arr[x][1], arr[x][2]);
            return x;
        }

        // Compare with username
        match = 1;
        for(int k = 0; word[k] != '\0' || arr[x][1][k] != '\0'; k++) {
            if(word[k] != arr[x][1][k]) {
                match = 0;
                break;
            }
        }
        if(match) {
            printf("%s | %s | %s\n", arr[x][0], arr[x][1], arr[x][2]);
            return x;
        }
    }

    printf("No matching entry found.\n");
    return 0;
}