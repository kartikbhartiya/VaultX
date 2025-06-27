#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int authorize(){
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
        if(key[i] == ',' || i == key_length-1){ //either ',' or if its the last character
            temp = (char)(encrypted[j]^num);
            decrypted[j] = temp; //stores the char into another string after decrypting
            num = 0;
            j++;
        }
    }
    return !(strcmp(decrypted,password));
}

int main(){
if(authorize()){
    printf("\nACCESS GRANTED");
}
else{
    printf("\nACCESS DENIED");
}
return 0;
}