#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define SEP 0x1F
char global_key[100] = {0};
int search(char*);
void delete_database(char*);


typedef struct{
    char domain[500];
    char username[100]; 
    char password[50]; //Encrypted
}credential;

int take(char inp[] , int n){
    char ch; int i = 0;
    while((ch = getchar())!='\n'){
        if(i<n)
            inp[i++] = ch;
    }
    inp[i] = '\0';
    return i;
}
void add_database() {
    credential out;
    while (getchar() != '\n');
    printf("Please Enter the Domain of the site: ");
    take(out.domain, sizeof(out.domain));
    printf("Please Enter the Username: ");
    take(out.username, sizeof(out.username));
    printf("Please Enter the Password: ");
    take(out.password, sizeof(out.password));
    // To Make sure key is available
    if (strlen(global_key) == 0) {
        printf("ACCESS DENIED:- CODE IS TAMPERED");
        exit(1);
    }
    // First, parse the key into an array of numbers
    int key_numbers[100];int key_count = 0;
    int current_num = 0;
    int has_digit = 0;
    for(int i = 0; i < strlen(global_key); i++) {
        if(isdigit(global_key[i])) {
            current_num = current_num * 10 + (global_key[i] - '0');
            has_digit = 1;//In case key has no no digit
        } else if(global_key[i] == '-' || global_key[i] == '\0') {
            if(has_digit) {
                key_numbers[key_count++] = current_num;
                current_num = 0;
                has_digit = 0;
            }
        }
    }
    if(has_digit) {
        key_numbers[key_count++] = current_num;//if last digit is -
    }
    
    if(key_count == 0) {
        printf("Error: Key contains no valid numbers\n");
        return;
    }

    int key_index = 0;
    
    // Encrypt password
    for(int i = 0; i < strlen(out.password); i++) {
        out.password[i] = out.password[i] ^ key_numbers[key_index];
        key_index = (key_index + 1) % key_count; //In case the password is greater than the key length
    }
    
    // Save to file
    FILE *fp = fopen("database.txt", "a");
    if (fp == NULL) {
        printf("Error opening database file\n");
        exit(1);
    }
    fprintf(fp, "%s%c%s%c%s\n", out.domain, SEP, out.username, SEP, out.password);
    fclose(fp);
    
    printf("Credentials added to database successfully!\n");
}

void fetch_database() {
    FILE *fp = fopen("database.txt", "r");
    if(fp == NULL) {
        printf("Error: Cannot open database file\n");
        exit(1);
    }

    if(strlen(global_key) == 0) {
        printf("Error: No decryption key provided\n");
        fclose(fp);
        return;
    }

    printf("\n--- Decrypted Credentials ---\n");
    
    // First, parse the key into an array of numbers
    int key_numbers[100];
    int key_count = 0;
    int current_num = 0;
    int has_digit = 0;
    
    for(int i = 0; i < strlen(global_key); i++) {
        if(isdigit(global_key[i])) {
            current_num = current_num * 10 + (global_key[i] - '0');
            has_digit = 1;
        } else if(global_key[i] == '-' || global_key[i] == '\0') {
            if(has_digit) {
                key_numbers[key_count++] = current_num;
                current_num = 0;
                has_digit = 0;
            }
        }
    }
    if(has_digit) {
        key_numbers[key_count++] = current_num;
    }
    
    if(key_count == 0) {
        printf("Error: Key contains no valid numbers\n");
        fclose(fp);
        return;
    }
    int key_index = 0;
    // Now process the file line by line
    char line[1000];int count_sep = 0;
    while(fgets(line, sizeof(line), fp) != NULL) {//parses one line
        // Remove newline if present
        line[strcspn(line, "\n")] = '\0';
        // Decrypt each character in the line (except separators)
        key_index = 0; count_sep = 0; //reinitialization
        for(int i = 0; i < strlen(line); i++) {
            if(line[i] == SEP) {
                printf(" | ");
                count_sep++;
                continue;
            }
            
            // Decrypt the character
            if(count_sep>1){ 
                printf("%c", line[i] ^ key_numbers[key_index]);
                key_index = (key_index + 1) % key_count; //so that key cycles back to 0 if key_index+1 > key_count
            }
            else printf("%c",line[i]);
            
            // Move to next key number, wrap around if needed
        }
        printf("\n");
    }
    
    fclose(fp);
    printf("----------------------------\n");
}

void audit_log(char* status){
    FILE *fp = fopen("logs.txt","a");
    if(fp == NULL){
        printf("ERROR : Cant Open Logs File");
        exit(1);
    }
    fprintf(fp,"%s   %s   %s\n",__DATE__,__TIME__,status);
    fclose(fp);
}

void password_strength(){
    char pass[500] = {0};
    printf("Please enter the password:- ");
    while(getchar()=='\n');//Clearing Buffer \n
    take(pass,500);
    int l = strlen(pass);int count = 0;
    int c[4] = {0};
    for(int i = 0 ; i<l ; i++){//here we will check if each type of character atleast occurs once or not
        if((pass[i]>='a' && pass[i]<='z') && (c[0]==0)){
            count++;
            c[0]++;
        }
        else if((pass[i]>='A' && pass[i]<='Z') && (c[1]==0)){
            count++;
            c[1]++;
        }
        else if((pass[i]>='0' && pass[i]<='9') && (c[2]==0)){
            count++;
            c[2]++;
        }
        else if((pass[i]>=33 && pass[i]<=126) && (c[3]==0)){
            count++;
            c[3]++;
        }
        else if(pass[i]<33 || pass[i]>126) {
            printf("Error:- Please enter the correct password\n");
            exit(1);
        }
    }
    switch(count){
        case 1:
            printf("\nWeak Password");
            break;
        case 2:
            printf("\nMild Password");
            break;
        case 3:
            printf("\nStrong Password");
            break;
        case 4:
            printf("\nVery Strong Password");
            break;
        default: break;
    }
}

void change(){
    printf("\nNOTE:- IF YOU CHANGE PASSWORRD ALL OF YOU THE PASSWORD DATABASE WILL BE DELETED");
    printf("\n Press Ctrl + C to Return Safely\n");
    getchar();
    char key[100] = {0};
    char user[50] = {0};
    printf("Please Enter the Username:- ");
    take(user,50);
    printf("Please Enter the Key(xxx-xxx-xxx... format):- ");
    take(key,100);
    for(int i = 0 ; i<strlen(key);i++){
        if(!(isdigit(key[i]) || key[i] == '-')){
            printf("Incorrect Key Format - Password change unsuccesful");
            exit(1);
        }
    }
    int key_length = strlen(key);
    char encrypted[50];int j=0;int count = 0;int num = 0 ; char temp;
    for(int i = 0 ; i<key_length ; i++){
        if(key[i]>='0' && key[i]<='9'){
            num = num*10 + (key[i] - '0'); //form a number out of characters of key[i] until a ',' is found
        }
        if(key[i] == '-' || i == key_length-1){ //either ',' or if its the last character
            temp = (char)(user[j]^num);
            user[j] = temp; //stores the char into another string after decrypting
            num = 0;
            j++;
        }
        count++;
    }
    if(count!=strlen(key)){
        printf("%d %d\n",count,strlen(key));
        printf("Wrong Format in Entering Key and Password");
        exit(1);
    }
    FILE *fp = fopen("encrypted_password.txt","w");
    fputs(user,fp);
    FILE *fp2 = fopen("database.txt","w");
    fputs("",fp2);
    fclose(fp);
    fclose(fp2);
    for(int i = 0 ; i<strlen(key) ; i++){
        global_key[i] = key[i];
    }
    printf("\nChanges Made Successful\n");
}

void password_generator(){
    srand(time(0)); //Seeding the RNG with current time
    int length = rand()%9 + 8; //length of password will be 8 to 15
    char pass[16] = {0};
    char temp;int temp1;
    for(int i = 0 ; i<length ; i++){
        temp1 = rand();//decide wether the character will be number/special character/letter/capital letter
        if(temp1%4 == 0){
            temp = rand()%10 + 48; //Number characters
        }
        else if(temp1%4 == 1){
            temp = rand()%26 + 65; //Upper case
        }
        else if(temp1%4 == 2){ //Special character
            temp1 = rand();
            if(temp1%4 == 0) temp = rand()%15 + 33;
            else if(temp1%4 == 1) temp = rand()%7 + 58;
            else if(temp1%4 == 2) temp = rand()%6 + 91;
            else temp = rand()%4 + 123;
        }
        else{
            temp = rand()%26 + 97; //lower case
        }
        pass[i] = temp;
    }
    printf("Your password is :- %s",pass);
}

void decrypter() {
    printf("Do you have key and text in file (y/n)? ");
    char resp;
    scanf(" %c", &resp);
    while(getchar() != '\n'); // Clear the input buffer

    char key[1000] = {0};      // Initialize to zeros
    char encrypted[500] = {0};
    char output[500] = {0};

    if(tolower(resp) == 'y') {
        FILE *fp = fopen("key.txt", "r");
        if(fp == NULL) {
            printf("Error: key.txt not found\n");
            exit(1);
        }
        fgets(key, sizeof(key), fp);
        fclose(fp);   
        FILE *fp2 = fopen("encrypted_text.txt", "r");
        if(fp2 == NULL) {
            printf("Error: encrypted_text.txt not found\n");
            exit(1);
        }
        fgets(encrypted, sizeof(encrypted), fp2);
        fclose(fp2);
        // Remove newlines from file input
        key[strcspn(key, "\n")] = '\0';
        encrypted[strcspn(encrypted, "\n")] = '\0';
    }
    else {
        printf("Please Enter the Encrypted Text: ");
        fgets(encrypted, sizeof(encrypted), stdin);
        encrypted[strcspn(encrypted, "\n")] = '\0';
        
        printf("Please Enter the Key: ");
        fgets(key, sizeof(key), stdin);
        key[strcspn(key, "\n")] = '\0';
    }
    int j = 0;
    int num = 0;
    int key_len = strlen(key);
    int text_len = strlen(encrypted);
    for(int i = 0; i <= key_len; i++) {
        if(key[i] >= '0' && key[i] <= '9') {
            num = num * 10 + (key[i] - '0');
        }
        else if(key[i] == '-' || key[i] == '\0') {
            if(j >= text_len) {
                printf("Error: Key is longer than encrypted text\n");
                exit(1);
            }
            output[j] = encrypted[j] ^ num;
            num = 0;
            j++;
        }
        else {
            printf("Invalid Key format\n");
            exit(1);
        }
    }
    output[j] = '\0'; // Null-terminate the string

    printf("\nThe Decrypted text is: %s\n", output);
}

int authorize(){ //Tier 2+ level Security
    char key[100];
    char password[50];
    printf("Please Enter the password:- ");
    fgets(password,50,stdin);
    password[strcspn(password, "\n")] = '\0'; // since password = xyz/n(for) it strips down the /n
    printf("Please Give the key:- ");
    fgets(key,100,stdin);
    for(int i = 0 ; i<strlen(key) ; i++){
        global_key[i] = key[i];
    }
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

void save(char encrypted[] , char key[]){
    FILE *fp = fopen("encrypted_text.txt","w");
    if(fp == NULL){
        printf("Error in Opening file\nPlease remember the text rather");
        exit(1);
    }
    fprintf(fp,encrypted);
    fclose(fp);
    FILE *fp2 = fopen("key.txt","w");
    if(fp2 == NULL){
        printf("Error in Opening file\nPlease remember the key rather");
        exit(1);
    }
    fprintf(fp2,key);
    fclose(fp2);
    printf("Files saved Succesfully");
}

void encrypter() {
    srand(time(0)); // seed with current time

    printf("Please enter the Text you want to encrypt\n");
    char input[500];
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    
    // Get input
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    input[strcspn(input, "\n")] = '\0';  // remove newline

    int length = strlen(input);
    if (length == 0) {
        printf("No input provided\n");
        return;
    }

    char encrypted[500] = {0};  // initialize to zeros
    char key[1000] = {0};       // initialize to zeros
    int r;
    for (int i = 0; i < length; i++) {
        do {
             r = rand() % 94 + 33; // r = 33-126
            encrypted[i] = input[i] ^ r; // XOR
        } while (encrypted[i] < 32 || encrypted[i] > 126); // Ensure encrypted[i] is printable
        encrypted[i] = input[i] ^ r; // XOR encryption

        // Build the key string
        char num_str[10];
        snprintf(num_str, sizeof(num_str), "%d", r); //safely storing r in num_str so that we can concaneate later
        strcat(key, num_str);
        
        // Add delimiter unless it's the last character
        if (i < length - 1) {
            strcat(key, "-");
        }
    }

    printf("The Encrypted message is:- %s\n", encrypted);
    printf("The Key is: %s\n", key);
    printf("Do you want to save this key y/n\n");
    char resp;
    scanf("%c",&resp);
    if(tolower(resp) == 'y') save(encrypted,key);
    else printf("NOTE:- IF THE KEY GET LOST IT CANNOT BE RECOVERED");
}

int main(){
    printf("Enter Admin mode? (y/n):- ");
    char temp;
    scanf("%c",&temp);
    getchar();
    if(tolower(temp) == 'y'){
        if(authorize()){ //use authorize() later rn due to testing purpose
            printf("\n==============\nACCESS GRANTED\n==============\n");
            while(1){
            audit_log("ADMIN");
            printf("\nPlease Select any option\n 1) Secret Message Encrypter\n 2) Secret Message Decrypter\n 3) Password Generator\n 4) Password Strength Teller\n 5) Add to Database\n 6) Show all Stored Passwords\n 7) Search Database\n 8) Delete Data Entry \n 9) Change Password\n 10) End The Current Session \n Please Enter Your Response:- ");
            int resp;
            scanf("%d",&resp);
            if(resp == 1) encrypter();
            else if(resp == 2) decrypter();
            else if(resp == 3) password_generator();
            else if(resp == 4) password_strength();
            else if(resp == 5) add_database();
            else if(resp == 6) fetch_database();
            else if(resp == 7) search(global_key);
            else if(resp == 8) delete_database(global_key);
            else if(resp == 9) change();
            else break;
            }
        }
        else{
            printf("\n=============\nACCESS DENIED\n=============\n");
            audit_log("INTRUDER");
        }
    }
    else{
        audit_log("USER");
        printf("Enter the number\n 1) Decrypt Secret Messages\n 2) Password Strength Teller\n 3) Exit\n Please Enter Your Response:- ");
        int resp;
        scanf("%d",&resp);
        if(resp == 1) decrypter();
        else if(resp == 2) password_strength();
        else ;
    }
    return 0;
}
