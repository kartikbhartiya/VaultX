#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int authorize(){ //Tier 2+ level Security
    char password[50];
    char key[100];
    printf("Please Enter the password:- ");
    fgets(password,50,stdin);
    password[strcspn(password, "\n")] = '\0'; // since password = kartik/n it strips down the /n
    printf("Please Give the key:- ");
    fgets(key,100,stdin);
    FILE *fp = fopen("encrypted_password.txt","r");//File open
    if(fp == NULL){
        printf("Error:- No File Found");
        exit(1);
    }
    char encrypted[50];
    fgets(encrypted,100,fp);
    fclose(fp);//File closed
    if(strlen(encrypted)!=strlen(password)) return 0; //If length of both are not equal password cant match
    //now we will separate each number separate from key and decrypt the extracted password
    int key_length = strlen(key);
    int j = 0;
    int num = 0;
    char temp;
    char decrypted[50];
    int count = 0;
    for(int i = 0 ; i<key_length ; i++){
        if(key[i]>='0' && key[i]<='9'){
            num = num*10 + (key[i] - '0'); //form a number out of characters of key[i] until a ',' is found
            count++;
        }
        if(count>strlen(key)) printf("Bug code 1\n");
        if(key[i] == '-' || i == key_length-1){ //either ',' or if its the last character
            temp = (char)(encrypted[j]^num);
            decrypted[j] = temp; //stores the char into another string after decrypting
            num = 0;
            j++;
        }
    }
    return !(strcmp(decrypted,password));
}

void encrypter() {
    srand(time(0)); // seed

    printf("Please enter the Text you want to encrypt\n");
    char input[500];
    getchar(); // clear stdin
    fgets(input, 500, stdin);
    input[strcspn(input, "\n")] = '\0';  // remove newline

    int length = strlen(input);

    char encrypted[500];
    char key[1000]; // large enough to store all keys as string
    key[0] = '\0';  // initialize empty string

    char temp_key[10];
    for (int i = 0; i < length; i++) {
        int r = rand() % 94 + 33; // get random printable ASCII (33–126)
        encrypted[i] = input[i] ^ r; // XOR encryption

        // Convert int r to string and append to key
        sprintf(temp_key, "%d-", r);
        strcat(key, temp_key);
    }
    encrypted[length] = '\0'; // null-terminate encrypted message

    printf("The Encrypted message is: ");
    for (int i = 0; i < length; i++) {
        printf("%c", (encrypted[i] >= 33 && encrypted[i] <= 126) ? encrypted[i] : '.');
    }
    printf("\n");

    printf("The Key is: %s\n", key);
    printf("NOTE:- KEEP THIS SAFE IN CASE IT GOT LOST CANNOT BE RECOVERED\n");
}


int main(){
if(1){ //use authorize() later rn due to testing purpose
    printf("\nACCESS GRANTED\n");
    printf("Please Select any option\n 1) Secret Message Encrypter\n 2) Secret Message Decrypter\n");
    int resp;
    scanf("%d",&resp);
    if(resp == 1) encrypter();
}
else{
    printf("\nACCESS DENIED");
}
return 0;
}
