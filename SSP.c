#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int main();

char* encryptPass(char* pass) {
    const char* key = "8Mz2cFQknTmRpBqS5g6yW7JwYHujEvUh";
    int i, len, keyLen;

    len = strlen(pass);
    keyLen = strlen(key);

    char* enP = (char*)malloc(len + 1); // +1 for the null terminator

    if (enP == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < len; i++) {
        // Exclude symbols by replacing them with an empty character
        if (isalpha(pass[i]) || isdigit(pass[i])) {
            char base = islower(pass[i]) ? 'a' : 'A';
            enP[i] = (pass[i] + key[i % keyLen] - base + 26) % 26 + base;
        } else {
            // Replace symbols with an empty character
            enP[i] = 'X';
        }
    }

    enP[len] = '\0'; // Null-terminate the encrypted password

    return enP;
}

int clearScreen(){
    getchar();

    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    return 0;
}

void validateMenuChoice(int* choice, int min, int max){

    while (*choice < min || *choice > max){
        printf("\n\t\tChoice should be in between %d and %d !", min, max);
        printf("\n\n \t\t\t     Choice: ");
        scanf("%d", choice);
        while (getchar() != '\n');

        if ((*choice >= min) && (*choice <= max)){
            break;
        } 

    }
}

char* trimSpaces(const char* input) {
    // Skip leading spaces
    while (isspace(*input)) {
        input++;
    }

    const char* end = input + strlen(input) - 1;
    
    while (end > input && isspace(*end)) {
        end--;
    }


    size_t length = (end - input) + 1;
    char* result = (char*)malloc(length + 1);

    if (result != NULL) {
        strncpy(result, input, length);
        result[length] = '\0';
    }

    return result;
}

char* generateRandomID(){
    char* randID = (char*)malloc(5 * sizeof(char)); 

    if (randID == NULL){
        return NULL;
    } else {
        srand(time(NULL));

        for (int i = 0; i < 4; i++){  
            randID[i] = '0' + rand() % 10;
        }

        randID[4] = '\0'; 
    }

    return randID;
}

int mainMenu(){
    int choice;


    printf("-----------------------------------------------------\n");
    printf("|\t\t\tMenu\t\t\t    |\n");
    printf("-----------------------------------------------------\n");
    printf("|\t\t     (1) Login\t\t\t    |\n");
    printf("|\t\t     (2) Exit\t\t\t    |\n");
    printf("-----------------------------------------------------\n\n");
    printf(" \t\t     Choice: ");

    scanf("%d", &choice);

    while (getchar() != '\n');

    validateMenuChoice(&choice, 1, 2);

    printf("\n-----------------------------------------------------\n");


    return choice;
}

struct userDetails{
    char name[50];
    char password[50];
    char type[50];
};

struct studentDetails{
    char name[50];
    char id[20];
    char coreModules[90];
    char activities[200];
    char points[5];
};

char* readCoreMod(){
    FILE *f = NULL;
    long fSize = 0;


    f = fopen("coremodules.txt", "rb");

    if (f == NULL){
        perror("File not found!");
        return NULL;
    }else{

        fseek(f, 0, SEEK_END);
        fSize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *coreM = (char*)malloc(fSize + 1);

        if (coreM == NULL){
            perror("No core modules can be found");
            fclose(f);
            return NULL;
        }else{
            size_t readB = fread(coreM, 1, fSize, f);
            
            if (readB != (size_t)fSize){
                printf("Error reading file");
                fclose(f);
                free(coreM);
                return NULL;
            }

            coreM[fSize] = '\0';        
        }
        fclose(f);

        return coreM;
    }
}

char* readCoreModNames() {
    FILE *f = fopen("coremodules.txt", "r");

    if (f == NULL) {
        perror("File not found!");
    } else {
        char *coreNames = NULL;
        size_t coreNamesSize = 0;

        char line[142];

        while (fgets(line, sizeof(line), f) != NULL) {
            char id[20];
            char name[100];
            char des[500];

            // Use sscanf to directly extract the name field
            if (sscanf(line, "%20[^\t]\t%100[^\t]\t%500[^\t]", id, name, des) == 3) {
                // Trim leading and trailing spaces from name
                char *trimmedName = trimSpaces(name);

                if (trimmedName != NULL) {
                    if (coreNames == NULL) {
                        coreNamesSize = strlen(trimmedName) + 1;
                        coreNames = (char*)malloc(coreNamesSize);
                        strcpy(coreNames, trimmedName);
                    } else {
                        coreNamesSize += strlen(trimmedName) + 2;
                        coreNames = (char*)realloc(coreNames, coreNamesSize);
                        strcat(coreNames, ",");
                        strcat(coreNames, trimmedName);
                    }
                    free(trimmedName);  // Free the memory allocated by trimSpaces
                }
            }
        }

        fclose(f);
        return coreNames;
    }
}

char* readUserFile(){
    FILE *f = fopen("userdata.txt", "rb");

    if (f == NULL) {
        perror("File not found!");
        return NULL;
    } else {
        fseek(f, 0, SEEK_END);
        long fSize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *users = (char*)malloc(fSize + 1);

        if (users == NULL) {
            perror("Memory allocation error");
            fclose(f);
            return NULL;
        } else {

            size_t bytesRead = fread(users, 1, fSize, f);

            if (bytesRead != (size_t)fSize) {
                printf("Error reading file");
                fclose(f);
                free(users);
                return NULL;
            }

            users[fSize] = '\0'; 
        }

        fclose(f);
        return users;
    }
}

char* checkAccount(char* myUsers, char* myName, char* myPass){
    int found = 0;

    myPass = encryptPass(myPass);

    char* line = strtok(myUsers, "\n");

    while (line != NULL){
        char u[50];
        char p[50];
        char t[50];

        if (sscanf(line, "%50[^\t]\t%50[^\t]\t%50[^\t]", u, p, t) == 3){
            if (strcmp(u, myName) == 0){
                if(strcmp(p, myPass) == 0){
                    found = 1;                   
                    return strdup(t);
                }                
            }
        }
        line = strtok(NULL, "\n");
    }
    
    if (found == 0){
        return NULL;
    }

    free(myPass);
}  

char* readStudentData() {
    FILE *f = NULL;
    long fSize = 0;

    f = fopen("studentdata.txt", "rb");

    if (f == NULL) {
        printf("File not found!");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    fSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *stud = (char*)malloc(fSize + 1);

    if (stud == NULL) {
        printf("Memory allocation failed");
        fclose(f);
        return NULL;
    }

    size_t readB = fread(stud, 1, fSize, f);

    if (readB != (size_t)fSize) {
        printf("Error reading file");
        fclose(f);
        free(stud);
        return NULL;
    }

    fclose(f);

    return stud;
}

char* readElecMod(){
    
    FILE *f = NULL;
    long fSize = 0;


    f = fopen("electivespecialismmodule.txt", "rb");

    if (f == NULL){
        perror("File not found!");
        return NULL;
    }else{

        fseek(f, 0, SEEK_END);
        fSize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *elecModules = (char*)malloc(fSize + 1);

        if (elecModules == NULL){
            perror("No users can be found");
            fclose(f);
            return NULL;
        }else{
            size_t readB = fread(elecModules, 1, fSize, f);
            
            if (readB != (size_t)fSize){
                perror("Error reading file");
                fclose(f);
                free(elecModules);
                return NULL;
            }

            elecModules[fSize] = '\0';        
        }
        fclose(f);

        return elecModules;
    }
}

int countNumLines(char* data){
    int num = 0;

    for (const char* chaPtr = data; *chaPtr != '\0'; ++chaPtr){
        if(*chaPtr == '\n'){
            num++;
        }
    }
    num++;

    return num;
}

char* extractStudentAct(char* stud, char* myID) {
    int found = 0;


    // Make a copy of the original string
    char* copyStud = strdup(stud);
    char* line = strtok(copyStud, "\n");
    int studentNum = countNumLines(stud) - 1;

    struct studentDetails student[studentNum];
    int count = 0;

    while (line != NULL) {

        // Tokenize the line by tabs
        if (sscanf(line, "%50[^\t]\t%20[^\t]\t%90[^\t]\t%200[^\t]\t%5[^\t]",student[count].name, student[count].id,student[count].coreModules, student[count].activities,student[count].points) == 5) {
            if (strcmp(student[count].id, myID) == 0) {
                found = 1;
                break;
            }
        }

        line = strtok(NULL, "\n");
        count++;
    }

    free(copyStud);

    if (found) {
        return strdup(student[count].activities);
    } else {
        printf("Student not found!\n");
        return NULL;
    }
}

void updateActivityR_ST(char* act, char* myStud, int* myFound, char* theID, char* points) {

    printf("\n");
    int myStudNo = countNumLines(myStud) - 1;
    struct studentDetails theStud[myStudNo];

    char* line2 = strtok(myStud, "\n");

    int count = 0;

    // Initialize newLine with enough space for one line
    size_t bufferSize = 500;

    char* newLine = (char*)malloc(bufferSize);

    newLine[0] = '\0'; // Initialize with an empty string


    while (line2 != NULL) {
        if (sscanf(line2, "%50[^\t]\t%20[^\t]\t%90[^\t]\t%200[^\t]\t%5[^\t]", theStud[count].name, theStud[count].id, theStud[count].coreModules, theStud[count].activities, theStud[count].points) == 5) {
            
            if (strcmp(theStud[count].id, theID) == 0) {
                *myFound = 1;
                strcpy(theStud[count].activities, act);

                int existingPoints = atoi(theStud[count].points);
                int newPoints = atoi(points);
                int totalPoints = existingPoints + newPoints;
                if (totalPoints > 200) {
                    printf("You have exceeded the limit! Cannot add!\n");
                    *myFound = 0;
                    free(newLine);
                    return;
                } else {
                    snprintf(theStud[count].points, sizeof(theStud[count].points), "%d", totalPoints);
                    // Resize the buffer to accommodate the new line
                    size_t newLineLength = strlen(newLine) + strlen(theStud[count].name) + strlen(theStud[count].id) + strlen(theStud[count].coreModules) + strlen(theStud[count].activities) + strlen(theStud[count].points) + 5; // 5 for tabs and newline
                    newLine = (char*)realloc(newLine, newLineLength);
                    strcat(newLine, theStud[count].name);
                    strcat(newLine, "\t");
                    strcat(newLine, theStud[count].id);
                    strcat(newLine, "\t");
                    strcat(newLine, theStud[count].coreModules);
                    strcat(newLine, "\t");
                    strcat(newLine, theStud[count].activities);
                    strcat(newLine, "\t");
                    strcat(newLine, theStud[count].points);
                    strcat(newLine, "\n");
                }
            } else {
                // Resize the buffer to accommodate the existing content and a new line
                size_t newLineLength = strlen(newLine) + strlen(line2) + 2; // 2 for newline and null terminator
                newLine = (char*)realloc(newLine, newLineLength);
                strcat(newLine, line2);
                strcat(newLine, "\n");
            }

        }

        line2 = strtok(NULL, "\n");
        count++;
    }
    strcpy(myStud, newLine);

    free(newLine);
}

void updateActivityUR_ST(char* act, char* myStud, int* myFound, char* theID, char* points){
   int myStudNo = countNumLines(myStud);
    struct studentDetails theStud[myStudNo];

    int found = 0;  
    char* line2 = strtok(myStud, "\n");
    int count = 0;

    // Initialize newLine with enough space for one line
    size_t bufferSize = 500;
    char* newLine = (char*)malloc(bufferSize);
    newLine[0] = '\0'; // Initialize with an empty string

    while (line2 != NULL) {

        if (sscanf(line2, "%50[^\t]\t%20[^\t]\t%90[^\t]\t%200[^\t]\t%5s", theStud[count].name, theStud[count].id, theStud[count].coreModules, theStud[count].activities, theStud[count].points) == 5) {

            if (strcmp(theStud[count].id, theID) == 0) {
                *myFound = 1;
                strcpy(theStud[count].activities, act);

                int existingPoints = atoi(theStud[count].points);
                int newPoints = atoi(points);
                int totalPoints = existingPoints - newPoints;

                if (totalPoints < 120) {
                    printf("\nYou are below the limit! Cannot remove!\n");
                    *myFound = 0;
                    free(newLine);
                    return;
                } else {
                    snprintf(theStud[count].points, sizeof(theStud[count].points), "%d", totalPoints);
                    // Resize the buffer to accommodate the new line
                    size_t newLineLength = strlen(newLine) + strlen(theStud[count].name) + strlen(theStud[count].id) + strlen(theStud[count].coreModules) + strlen(theStud[count].activities) + strlen(theStud[count].points) + 5; // 5 for tabs and newline
                    newLine = (char*)realloc(newLine, newLineLength);
                    strcat(newLine, theStud[count].name);
                    strcat(newLine, "\t");
                    strcat(newLine, theStud[count].id);
                    strcat(newLine, "\t");
                    strcat(newLine, theStud[count].coreModules);
                    strcat(newLine, "\t");
                    strcat(newLine, theStud[count].activities);
                    strcat(newLine, "\t");
                    strcat(newLine, theStud[count].points);
                    strcat(newLine, "\n");
                }
            } else {
                // Resize the buffer to accommodate the existing content and a new line
                size_t newLineLength = strlen(newLine) + strlen(line2) + 2; // 2 for newline and null terminator
                newLine = (char*)realloc(newLine, newLineLength);
                strcat(newLine, line2);
                strcat(newLine, "\n");
            }

        }

        line2 = strtok(NULL, "\n");
        count++;
    }

    strcpy(myStud, newLine);
    free(newLine);

}

char* logIn(){
    char pass[50];
    char name[50];
    char* users;
    printf("-------------------------------------------------------------------\n");
    printf("|\t\t\t     Log in\t\t\t\t  |\n");
    printf("-------------------------------------------------------------------\n");
    printf("\t\t\t   Username: ");
    scanf("%s", name);
    printf("\n");
    printf("\t\t\t   Password: ");
    scanf("%s", pass);
    printf("\n");
    printf("-------------------------------------------------------------------\n\n");
    
    users = readUserFile();
    if (users == NULL){
        return NULL;     
    }

    char* userType = checkAccount(users, name, pass);
    return userType;
    
    free(users);

}

int studentMenu(){
    int choice;


    printf("--------------------------------------------------------------\n");
    printf("|\t\t\tStudent Menu\t\t\t     |\n");
    printf("--------------------------------------------------------------\n");
    printf("|\t\t     (1) Register new activities\t     |\n");
    printf("|\t\t     (2) Unregister from activities\t     |\n");
    printf("|\t\t     (3) Track activities and credits\t     |\n");
    printf("|\t\t     (4) Log out\t\t             |\n");

    printf("--------------------------------------------------------------\n\n");
    printf(" \t\t\t Choice: ");

    scanf("%d", &choice);

    while (getchar() != '\n');

    validateMenuChoice(&choice, 1, 4);

    printf("\n-------------------------------------------------------------------\n");


    return choice;
}

int listActivities(){
    int choice;


    printf("--------------------------------------------------------------\n");
    printf("|\t\t\tActivity list\t\t\t     |\n");
    printf("--------------------------------------------------------------\n");
    printf("|\t\t     (1) Elective/Specialism module\t     |\n");
    printf("|\t\t     (2) Talk/Workshop\t\t\t     |\n");
    printf("|\t\t     (3) Club/Society\t\t\t     |\n");
    printf("|\t\t     (4) Competition\t\t\t     |\n");

    printf("--------------------------------------------------------------\n\n");
    printf(" \t\t\t Choice: ");

    scanf("%d", &choice);

    while (getchar() != '\n');

    validateMenuChoice(&choice, 1, 4);

    printf("\n-------------------------------------------------------------------\n");


    return choice;
}

void displayElecSpecMod(){
    FILE *f = NULL;

    f = fopen("electivespecialismmodule.txt", "r");

    if(f == NULL){
        perror("File not found");
    }else{
        char data[75];
        while(fgets(data, sizeof(data), f) != NULL){
            printf("%s", data);
        }
    }
    fclose(f);

}

void writeMod(char* myData){

    FILE *f;
    f = fopen("studentdata.txt", "w");

    if (f == NULL){
        perror("Cannot open file");
    }else{
        fputs(myData, f);
    }
    
    fclose(f);

}

void regElecSpecMod(char* myID){

    char* myStud = readStudentData();
    char* myCredits = NULL;
    char* myPoints = (char*)malloc(3);

    if(myStud != NULL){
        char* tempBuffer  = strdup(myStud);
        char* myAct = extractStudentAct(tempBuffer, myID); 
        char* myElecMod;

        if (myAct != NULL){

            displayElecSpecMod();

            char choice[20];

            printf("\n\nChoice of module (e.g E0001): ");
            scanf("%s", choice);

            FILE *f1;

            f1 = fopen("electivespecialismmodule.txt", "r");

            if (f1 == NULL){
                perror("File not found");
            }else{
                myElecMod = readElecMod();

                int found = 0;
                char* act;
                char* line1 = strtok(myElecMod, "\n");

                while (line1 != NULL){
                    char id[20];
                    char n[90];
                    char c[5];


                    if (sscanf(line1, "%20[^\t]\t%90[^\t]\t%5[^\t]", id, n, c) == 3){
                        if (strcmp(id, choice) == 0){
                            found = 1;
                            myCredits = c;
                            act = strdup(n);
                            break;
                        }                
                    
                    }
                    line1 = strtok(NULL, "\n");
                }
                
                if (found == 0){
                    printf("Elective module not found !");
                    act = NULL;
                }else{

                    act = trimSpaces(act);

                    myAct = strcat(myAct,",");
                    myAct = strcat(myAct, act);

                    
                    int myFound = 0;

                    int cred = atoi(myCredits);
                    int pnt = 0;

                    pnt = cred * 10;

                    snprintf(myPoints, sizeof(myPoints), "%d", pnt);
        
                    updateActivityR_ST(myAct, myStud, &myFound, myID, myPoints);

                    if (myFound != 0){
                        writeMod(myStud);           
                        printf("\nElective/Specialism module added !\n");
                    }
                    free(myStud);
                    free(myAct);
                }  
            }   
            fclose(f1);
            return;

        }else {
            printf("\nStudent not found!");
        }

    }
}

void displayTalkWorkMod(){
    FILE *f = NULL;

    f = fopen("talkworkshop.txt", "r");

    if(f == NULL){
        perror("File not found");
    }else{
        char data[70];
        while(fgets(data, sizeof(data), f) != NULL){
            printf("%s", data);
        }
    }
    fclose(f);

}

char* readTWMod(){
    
    FILE *f = NULL;
    long fSize = 0;


    f = fopen("talkworkshop.txt", "rb");

    if (f == NULL){
        perror("File not found!");
        return NULL;
    }else{

        fseek(f, 0, SEEK_END);
        fSize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *tWModules = (char*)malloc(fSize + 1);

        if (tWModules == NULL){
            perror("No users can be found");
            fclose(f);
            return NULL;
        }else{
            size_t readB = fread(tWModules, 1, fSize, f);
            
            if (readB != (size_t)fSize){
                perror("Error reading file");
                fclose(f);
                free(tWModules);
                return NULL;
            }

            tWModules[fSize] = '\0';        
        }
        fclose(f);

        return tWModules;
    }
}

void regTalkWork(char* myID){

    clearScreen();

    char* myStud = readStudentData();
    char* myPoints;
    char* myTWMod;

    if (myStud != NULL) {

        char* tempBuffer  = strdup(myStud);

        char* myAct = extractStudentAct(tempBuffer, myID);
        
        if (myAct != NULL){

            displayTalkWorkMod();

            char choice[20];

            printf("\n\nChoice of module (e.g T0001, W0001): ");
            scanf("%s", choice);

            clearScreen();

            char cat = choice[0];
            FILE *f;

            f = fopen("talkworkshop.txt", "r");

            if (f == NULL){
                printf("File not found");
            }else{
                myTWMod = readTWMod();


                int found = 0;
                char* act;
                char* line1 = strtok(myTWMod, "\n");

                while (line1 != NULL){
                    char id[20];
                    char c[5];
                    char n[50];

                    if (sscanf(line1, "%20[^\t]\t%5[^\t]\t%50[^\t]", id, c, n) == 3){
                        if (strcmp(id, choice) == 0){
                            found = 1;
                            myPoints = "1";
                            act = strdup(n);
                            break;
                        }                
                    
                    }
                    line1 = strtok(NULL, "\n");
                }
                
                if (found == 0){
                    printf("Talk or workshop not found !");
                    act = NULL;
                }else{
            
                    act = trimSpaces(act);

                    myAct = strcat(myAct,",");
                    myAct = strcat(myAct, act);

                    printf("%s\n", myAct);

                    int myFound = 0;
                    updateActivityR_ST(myAct, myStud, &myFound, myID, myPoints);

                    printf("%s\n", myStud);

                    if ((myFound != 0)){
                        printf("x");
                        writeMod(myStud);

                        if (cat == 'T'){
                            printf("\nTalk added !\n");
                        }else{
                            printf("\nWorkshop added !\n");
                        }

                    }else{
                        perror("Student not found");
                    }

                    free(myStud);
                    free(myAct);
                }  
            }   
            fclose(f);
            return;

        }else {
            printf("\nStudent not found!");
        }

    }

}

char* readCSMod(){
    
    FILE *f = NULL;
    long fSize = 0;


    f = fopen("clubsociety.txt", "rb");

    if (f == NULL){
        perror("File not found!");
        return NULL;
    }else{

        fseek(f, 0, SEEK_END);
        fSize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *cSModules = (char*)malloc(fSize + 1);

        if (cSModules == NULL){
            perror("No users can be found");
            fclose(f);
            return NULL;
        }else{
            size_t readB = fread(cSModules, 1, fSize, f);
            
            if (readB != (size_t)fSize){
                perror("Error reading file");
                fclose(f);
                free(cSModules);
                return NULL;
            }

            cSModules[fSize] = '\0';        
        }
        fclose(f);

        return cSModules;
    }
}

void displayClubSoc(){
    FILE *f = NULL;

    f = fopen("clubsociety.txt", "r");

    if(f == NULL){
        perror("File not found");
    }else{
        char data[75];
        while(fgets(data, sizeof(data), f) != NULL){
            printf("%s", data);
        }
    }
    fclose(f);
}

void regClubSoc(char* myID){

    char* myStud = readStudentData();
    char* myPoints;

    if (myStud != NULL) {
        char* tempBuffer  = strdup(myStud);
        char* myAct = extractStudentAct(tempBuffer, myID);
        char* myCSMod;

        if (myAct != NULL){

            displayClubSoc();

            char choice[20];

            printf("\n\nChoice of module (e.g C0001, S0001): ");
            scanf("%s", choice);

            char cat = choice[0];
            FILE *f;

            f = fopen("clubsociety.txt", "r");

            if (f == NULL){
                perror("File not found");
            }else{
                myCSMod = readCSMod();

                int found = 0;
                char* act;
                char* line1 = strtok(myCSMod, "\n");

                while (line1 != NULL){
                    char id[20];
                    char c[5];
                    char n[50];
                    char p[5];

                    if (sscanf(line1, "%20[^\t]\t%5[^\t]\t%50[^\t]\t%5[^\t]", id, c, n, p) == 4){
                        if (strcmp(id, choice) == 0){
                            found = 1;
                            myPoints = strdup(p);
                            act = strdup(n);
                            break;
                        }                
                    
                    }
                    line1 = strtok(NULL, "\n");
                }
                
                if (found == 0){
                    printf("Club or society not found !");
                    act = NULL;
                }else{
            
                    act = trimSpaces(act);

                    myAct = strcat(myAct,",");
                    myAct = strcat(myAct, act);

                    int myFound = 0;

                    updateActivityR_ST(myAct, myStud, &myFound, myID, myPoints);

                    if (myFound != 0){
                        writeMod(myStud);

                        if (cat == 'S'){
                            printf("\nSociety added !\n");
                        }else{
                            printf("\nClub added !\n");
                        }

                    }else{
                        printf("Student not found");
                    }

                    free(myStud);
                    free(myAct);
                }  
            }   
            fclose(f);
            

        }else {
            printf("\nStudent not found!");
        }

    }
}

char* readCompMod(){
    FILE *f = NULL;
    long fSize = 0;


    f = fopen("competition.txt", "rb");

    if (f == NULL){
        perror("File not found!");
        return NULL;
    }else{

        fseek(f, 0, SEEK_END);
        fSize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *compModules = (char*)malloc(fSize + 1);

        if (compModules == NULL){
            perror("No users can be found");
            fclose(f);
            return NULL;
        }else{
            size_t readB = fread(compModules, 1, fSize, f);
            
            if (readB != (size_t)fSize){
                perror("Error reading file");
                fclose(f);
                free(compModules);
                return NULL;
            }

            compModules[fSize] = '\0';        
        }
        fclose(f);

        return compModules;
    }
}

void displayCompetitions(){

    FILE *f = NULL;

    f = fopen("competition.txt", "r");

    if(f == NULL){
        perror("File not found");
    }else{
        char data[75];
        while(fgets(data, sizeof(data), f) != NULL){
            printf("%s", data);
        }
    }
    fclose(f);

}

void regCompetition(char* myID){

    char* myStud = readStudentData();
    char* myCredits;
    char* myPoints = (char*)malloc(5);

    if (myStud != NULL) {
        char* tempBuffer = myStud;
        char* myAct = extractStudentAct(tempBuffer, myID);
        char* myCompMod;

        if (myAct != NULL){

            displayCompetitions();

            char choice[20];

            printf("\n\nChoice of module (e.g C0001): ");
            scanf("%s", choice);

            char cat = choice[0];
            FILE *f;

            f = fopen("competition.txt", "r");

            if (f == NULL){
                perror("File not found");
            }else{
                myCompMod = readCompMod();
                
                int found = 0;
                char* act;
                char* line1 = strtok(myCompMod, "\n");

                while (line1 != NULL){
                    char id[20];
                    char n[50];

                    if (sscanf(line1, "%20[^\t]\t%50[^\t]", id, n) == 2){
                        if (strcmp(id, choice) == 0){
                            found = 1;
                            myCredits = "1";
                            act = n;
                            break;
                        }                
                    
                    }
                    line1 = strtok(NULL, "\n");
                }
                
                if (found == 0){
                    printf("Competition not found !");
                    act = NULL;
                }else{
            
                    act = trimSpaces(act);

                    myAct = strcat(myAct,",");
                    myAct = strcat(myAct, act);


                    int myFound = 0;
                    int cred = atoi(myCredits);
                    int pnt = 0;

                    pnt = cred * 10;

                    snprintf(myPoints, sizeof(myPoints), "%d", pnt);
                    
                    updateActivityR_ST(myAct, myStud, &myFound, myID, myPoints);
                    

                    if (myFound != 0){
                        writeMod(myStud);
                        printf("\nCompetition added !\n");
                    }
                    free(myStud);
                    free(myAct);
                }  
            }   
            fclose(f);
            

        }else {
            printf("\nStudent not found!\n");
        }

    }
}

void unRegElecSpecMod(char* theID){

    char* myStud = readStudentData();
    char* myCredits;
    char* myPoints = (char*)malloc(5);

    if(myStud != NULL){
        char* myAct = extractStudentAct(myStud, theID);

        if(myAct != NULL){
            displayElecSpecMod();

            char choice[20];

            printf("\n\nChoice of module to unregister(e.g E0001): ");
            scanf("%s", choice);

            FILE *f;

            f = fopen("electivespecialismmodule.txt", "r");

            if (f == NULL){
                perror("File not found");
            }else{
                char* myElecMod = readElecMod();

                int found = 0;
                char* actToRemove;
                char* line = strtok(myElecMod, "\n");

                while (line != NULL){
                    char id[20];
                    char n[50];
                    char c[5];

                    if (sscanf(line, "%20[^\t]\t%50[^\t]\t%5[^\t]", id, n, c) == 3){
                        if (strcmp(id, choice) == 0){
                            found = 1;
                            myCredits = strdup(c);
                            actToRemove = strdup(n);
                            break;
                        }                
                    
                    }
                    line = strtok(NULL, "\n");
                }
                
                if (found == 0){
                    printf("Elective module not found !");
                    actToRemove = NULL;
                }else{
                    char* splitAct = strtok(myAct, ",");
                    char newAct[BUFSIZ] = "";
                    int firstAct = 1;

                    while(splitAct != NULL){

                        if(strcmp(trimSpaces(splitAct), trimSpaces(actToRemove)) != 0){
                            if(!firstAct){
                                strcat(newAct, ",");
                            }else{
                                firstAct = 0;
                            }

                            strcat(newAct, splitAct);
                        }

                        splitAct = strtok(NULL, ",");

                    }
                    
                    myAct = strdup(newAct);

                    int myFound = 0;
                    int cred = atoi(myCredits);
                    int pnt = 0;
                    pnt = cred * 10;
                    snprintf(myPoints, sizeof(myPoints), "%d", pnt);

                    updateActivityUR_ST(myAct, myStud, &myFound, theID, myPoints);

                    if (myFound != 0){
                        writeMod(myStud);           
                        printf("\nElective/Specialism module unregistered !\n");
                    }else{
                        printf("Student not found");
                    }
                    free(myStud);
                    free(myAct);
                }
            }
        }else{
            printf("\nStudent not found !");
        }
    }else{
        perror("\nNo data found in file !");
    }
}

void unRegTalkWork(char* theID){

    char* myStud = readStudentData();
    char* myPoints;

    if(myStud != NULL){
        char* myAct = extractStudentAct(myStud, theID);

        if(myAct != NULL){
            displayTalkWorkMod();

            char choice[20];
            char cat = choice[0];

            printf("\n\nChoice of module to unregister(e.g T0001, W0001): ");
            scanf("%s", choice);

            FILE *f;

            f = fopen("talkworkshop.txt", "r");

            if (f == NULL){
                perror("File not found");
            }else{
                char* mytWMod = readTWMod();

                int found = 0;
                char* actToRemove;
                char* line = strtok(mytWMod, "\n");

                while (line != NULL){
                    char id[20];
                    char n[50];
                    char c[5];

                    if (sscanf(line, "%20[^\t]\t%5[^\t]\t%50[^\t]", id, c, n) == 3){
                        if (strcmp(id, choice) == 0){
                            found = 1;
                            myPoints = "1";
                            actToRemove = strdup(n);
                            printf("Act to remove: %s\n", actToRemove);
                            break;
                        }                
                    
                    }
                    line = strtok(NULL, "\n");
                }
                
                if (found == 0){
                    printf("Talk or workshop not found !");
                    actToRemove = NULL;
                }else{
                    char* splitAct = strtok(myAct, ",");
                    char newAct[BUFSIZ] = "";
                    int firstAct = 1;

                    printf("Act to remove: %s\n", actToRemove);
                    while (splitAct != NULL) {
                        
                        if (strcmp(trimSpaces(splitAct), trimSpaces(actToRemove)) != 0) {
                            printf("%s\n", splitAct);
                            if (!firstAct) {
                                strcat(newAct, ",");
                            } else {
                                printf("\nYes\n");
                                firstAct = 0;
                            }

                            strcat(newAct, splitAct);
                        }

                        // Move to the next substring
                        splitAct = strtok(NULL, ",");

                    }

                    free(myAct); // Free the memory allocated for the original myAct
                    myAct = strdup(newAct);
                    int myFound = 0;
                    
                    updateActivityUR_ST(myAct, myStud, &myFound, theID, myPoints);


                    if (myFound != 0){
                        writeMod(myStud);

                        if (cat == 'T'){
                            printf("\nTalk unregistered !\n");
                        }else{
                            printf("\nWorkshop unregistered !\n");
                        }
                    }
                    free(myStud);
                    free(myAct);
                }
                return;
            }
        }else{
            printf("\nStudent not found !");
        }
    }else{
        printf("\nNo data found in file !");
    }
}

void unRegClubSoc(char* theID){

    char* myStud = readStudentData();
    char* myPoints;

    if(myStud != NULL){
        char* myAct = extractStudentAct(myStud, theID);

        if(myAct != NULL){
            displayClubSoc();

            char choice[20];
            char* cat;

            printf("\n\nChoice of module to unregister(e.g C0001, S0001): ");
            scanf("%s", choice);

            FILE *f;

            f = fopen("clubsociety.txt", "r");

            if (f == NULL){
                printf("File not found");
            }else{
                char* myCS = readCSMod();

                int found = 0;
                char* actToRemove;
                char* line = strtok(myCS, "\n");

                while (line != NULL){
                    char id[20];
                    char c[5];
                    char n[50];
                    char p[5];

                    if (sscanf(line, "%20[^\t]\t%5[^\t]\t%50[^\t]\t%5[^\t]", id, c, n, p) == 4){
                        if (strcmp(id, choice) == 0){
                            found = 1;
                            cat = strdup(c);
                            myPoints = strdup(p);
                            actToRemove = strdup(n);
                            break;
                        }                
                    
                    }
                    line = strtok(NULL, "\n");
                }
                
                if (found == 0){
                    printf("Club/Society not found !");
                    actToRemove = NULL;
                }else{
                    char* splitAct = strtok(myAct, ",");
                    char newAct[BUFSIZ] = "";
                    int firstAct = 1;

                    while(splitAct != NULL){

                        if(strcmp(trimSpaces(splitAct), trimSpaces(actToRemove)) != 0){
                            if(!firstAct){
                                strcat(newAct, ",");
                            }else{
                                firstAct = 0;
                            }

                            strcat(newAct, splitAct);
                        }

                        splitAct = strtok(NULL, ",");

                    }

                    free(myAct);
                    myAct = strdup(newAct);

                    int myFound = 0;


                    updateActivityUR_ST(myAct, myStud, &myFound, theID, myPoints);


                    if (myFound != 0){
                        writeMod(myStud);

                        if (strcmp(cat, "s") == 0){
                            printf("\nSociety unregistered !\n");
                        }else{
                            printf("\nClub unregistered !\n");
                        }
                    }
                    free(myStud);
                    free(myAct);
                }
                return;
            }
        }else{
            printf("\nStudent not found !");
        }
    }else{
        perror("\nNo data found in file !");
    }
}

void unRegCompetition(char* theID){

    char* myStud = readStudentData();
    char* myCredits;
    char* myPoints = (char*)malloc(5);

    if(myStud != NULL){
        char* myAct = extractStudentAct(myStud, theID);

        if(myAct != NULL){
            displayCompetitions();

            char choice[20];

            printf("\n\nChoice of module to unregister(e.g C0001): ");
            scanf("%s", choice);

            FILE *f;

            f = fopen("competition.txt", "r");

            if (f == NULL){
                printf("File not found");
            }else{
                char* myComp = readCompMod();

                int found = 0;
                char* actToRemove;
                char* line = strtok(myComp, "\n");

                while (line != NULL){
                    char id[20];
                    char n[50];

                    if (sscanf(line, "%20[^\t]\t%50[^\t]", id, n) == 2){
                        if (strcmp(id, choice) == 0){
                            found = 1;
                            myCredits = "1";
                            actToRemove = strdup(n);
                            break;
                        }                
                    
                    }
                    line = strtok(NULL, "\n");
                }
                
                if (found == 0){
                    printf("Competition not found !");
                    actToRemove = NULL;
                }else{
                    char* splitAct = strtok(myAct, ",");
                    char newAct[BUFSIZ] = "";
                    int firstAct = 1;

                    while(splitAct != NULL){

                        if(strcmp(trimSpaces(splitAct), trimSpaces(actToRemove)) != 0){
                            if(!firstAct){
                                strcat(newAct, ",");
                            }else{
                                firstAct = 0;
                            }

                            strcat(newAct, splitAct);
                        }

                        splitAct = strtok(NULL, ",");

                    }

                    free(myAct);
                    myAct = strdup(newAct);

                    int myFound = 0;
                    int cred = atoi(myCredits);
                    int pnt = 0;
                    pnt = cred * 10;
                    snprintf(myPoints, sizeof(myPoints), "%d", pnt);
                    updateActivityUR_ST(myAct, myStud, &myFound, theID, myPoints);

                    if (myFound){
                        writeMod(myStud);           
                        printf("\nCompetition unregistered !\n");
                    }

                    free(myStud);
                    free(myAct);
                }
                return;
            }
        }else{
            printf("\nStudent not found !");
        }
    }else{
        perror("\nNo data found in file !");
    }
}

void trackActivities(char* myID){

    int found = 0;

    char* studData = readStudentData();

    char* line = strtok(studData, "\n");
    int studentNum = 0;

    studentNum = countNumLines(studData) + 1;

    struct studentDetails students[studentNum];

    int count = 1;
    char* actList = (char*)malloc(200);
    char* myPoints = (char*)malloc(5);

    while (count <= studentNum){

        if (sscanf(line, "%50[^\t]\t%20[^\t]\t%90[^\t]\t%200[^\t]\t%5[^\t]", students[count].name, students[count].id, students[count].coreModules, students[count].activities, students[count].points) == 5){
            
            if (strcmp(trimSpaces(students[count].id), myID) == 0){
                found = 1;
                actList =  strdup(students[count].activities);
                myPoints = strdup(students[count].points);
                break;
            }                

        }
        line = strtok(NULL, "\n");

        studentNum++;
    }
    
    if (found == 0){
        printf("No data found for this student !");
    }else{
        printf("\n\nActivities: %s\n", actList);
        printf("Points score: %s\n\n", myPoints);
    }
}

void studentMain(){
    int option = studentMenu();

    char myID[50];

    while((option > 0) && (option <= 4)){
        if(option == 1){
            printf("\n\nEnter student ID: ");
            scanf("%s", myID);


            int choice = listActivities();

            if (choice == 1){
                regElecSpecMod(myID);
            }else if(choice == 2){
                regTalkWork(myID);
            }else if(choice == 3){
                regClubSoc(myID);
            }else{
                regCompetition(myID);
            }
        }else if(option == 2){
            printf("\n\nEnter student ID: ");
            scanf("%s", myID);


            int choice = listActivities();

            if (choice == 1){
                unRegElecSpecMod(myID);
            }else if(choice == 2){
                unRegTalkWork(myID);
            }else if(choice == 3){
                unRegClubSoc(myID);
            }else{
                unRegCompetition(myID);
            }
    
        }else if(option == 3){
            printf("\n\nEnter student ID: ");
            scanf("%s", myID);
            trackActivities(myID);
        }else{
            clearScreen();
            main();
        }

        option = studentMenu();

        if(option = 4){
            clearScreen();
            main();
        }
    }
}

void newProgrammeCoordinator() {
     FILE *f = NULL;

    f = fopen("userdata.txt", "a");

    if (f == NULL) {
        perror("Error opening file");
        return;
    }

    struct userDetails newPC;

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("\nEnter programme coordinator's username: ");
    fgets(newPC.name, sizeof(newPC.name), stdin);

    size_t len = strlen(newPC.name);
    if (len > 0 && newPC.name[len - 1] == '\n') {
        newPC.name[len - 1] = '\0';
    }

    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter programme coordinator's password: ");
    scanf("%49s", newPC.password);

    char* myPass = encryptPass(newPC.password);

    strcpy(newPC.type, "programme coordinator");

    fprintf(f, "%s\t%s\t%s\n", newPC.name, myPass, newPC.type);

    fclose(f);

    printf("\n\nNew Programme Coordinator added !\n");
}

void newStudentAcc() {
    FILE *f = fopen("userdata.txt", "a");

    struct userDetails newStud;
    printf("\nEnter student's username:");
    scanf("%s", newStud.name);
    printf("\nEnter student's password: ");
    scanf("%s", newStud.password);
    strcpy(newStud.type, "student");

    char* myPass = encryptPass(newStud.password);
    fprintf(f, "%s\t%s\t%s\n", newStud.name, myPass, newStud.type);

    fclose(f);

    FILE *f2 = fopen("studentdata.txt", "a");

    struct studentDetails newStud1;

    char* myRandID = generateRandomID();

    if (myRandID == NULL) {
        perror("Failed memory allocation !");
    } else {
        char* coreM = readCoreModNames();

        size_t len = strlen(coreM);
        if (len > 0 && coreM[len - 1] == ',') {
            coreM[len - 1] = ' ';
        }

        char theRandID[20];
        strcpy(theRandID, "S");
        strcat(theRandID, myRandID);

        strcpy(newStud1.id, theRandID);
        strcpy(newStud1.name, newStud.name);
        strcpy(newStud1.coreModules, coreM);
        strcpy(newStud1.activities, ", ");
        strcpy(newStud1.points, "120");

        fprintf(f2, "%s\t%s\t%s\t%s\t%s\n", newStud1.name, newStud1.id, newStud1.coreModules, newStud1.activities, newStud1.points);

        free(myRandID);

        fclose(f2);
    }

    printf("\n\nNew student added !\n");

}

void newProgrammeLeader() {
    clearScreen();
    FILE *f = NULL;

    f = fopen("userdata.txt", "a");

    if (f == NULL) {
        perror("Error opening file");
        return;
    }

    struct userDetails newPL;

    printf("\nEnter programme leader's username: ");
    fgets(newPL.name, sizeof(newPL.name), stdin);

    size_t len = strlen(newPL.name);
    if (len > 0 && newPL.name[len - 1] == '\n') {
        newPL.name[len - 1] = '\0';
    }

    printf("\nEnter programme leader's password: ");
    scanf("%49s", newPL.password);

    getchar();

    strcpy(newPL.type, "programme leader");

    char* pass = encryptPass(newPL.password);

    fprintf(f, "%s\t%s\t%s\n", newPL.name, pass, newPL.type);

    fclose(f);

    printf("\n\nNew programme leader added !");

    //clearScreen();
}

void updateUsername(char* data, const char* myUsername) {
    int userNo = 0;
    char* line = data;

    while ((line = strchr(line, '\n'))) {
        userNo++;
        line++;
    }

    struct userDetails userData[userNo];
    line = strtok(data, "\n");

    int count = 0;

    while (line != NULL && count < userNo) {
        if (sscanf(line, "%49[^\t]\t%49[^\t]\t%49s", userData[count].name, userData[count].password, userData[count].type) == 3) {
            if (strcmp(myUsername, userData[count].name) == 0) {
                char newUsername[50];
                printf("Enter new username: ");
                scanf("%s[^\n]", newUsername);

                snprintf(userData[count].name, sizeof(userData[count].name), "%s", newUsername);
            }
        }

        line = strtok(NULL, "\n");
        count++;
    }

    FILE* f;
    f = fopen("userdata.txt", "w");

    if (f == NULL) {
        perror("File cannot be found !");
    } else {
        for (int i = 0; i < userNo; i++) {
            fprintf(f, "%s\t%s\t%s\n", userData[i].name, userData[i].password, userData[i].type);
        }
        printf("\n\nFile updated !\n");
    }
    fclose(f);
}

void updatePassword(char* data, const char* myUsername) {
    int userNo = 0;
    char* line = data;

    while ((line = strchr(line, '\n'))) {
        userNo++;
        line++;
    }

    struct userDetails* userData = (struct userDetails*)malloc(userNo * sizeof(struct userDetails));
    if (userData == NULL) {
        perror("Memory allocation error");
        return;
    }

    line = data;
    for (int i = 0; i < userNo; i++) {
        if (sscanf(line, "%49s %49s %49[^\n]", userData[i].name, userData[i].password, userData[i].type) == 3) {
            if (strcmp(myUsername, userData[i].name) == 0) {
                char newPass[50];
                printf("Enter new password: ");
                scanf("%49s", newPass);

                char* myPass = encryptPass(newPass);
                snprintf(userData[i].password, sizeof(userData[i].password), "%s", myPass);
            }
        }

        line = strchr(line, '\n');
        if (line != NULL) {
            line++; 
        }
    }

    FILE* f = fopen("userdata.txt", "w");
    if (f == NULL) {
        perror("File cannot be found !");
        free(userData);
        return;
    }

    for (int i = 0; i < userNo; i++) {
        fprintf(f, "%s\t%s\t%s\n", userData[i].name, userData[i].password, userData[i].type);
    }

    printf("\n\nFile updated!\n");

    fclose(f);
    free(userData);
}

void adminMain();

void updateAccount(){
    char username[50];
    int choice;

    char* myData = readUserFile();

    printf("\n\n(1) Username");
    printf("\n(2) Password\n");

    printf("\nChoice: ");
    scanf("%d", &choice);

    validateMenuChoice(&choice, 1, 2);

    printf("\nEnter username: ");
    scanf("%s", username);

    if(choice == 1){
        updateUsername(myData, username);
        //clearScreen();
        adminMain();
    }else{
        updatePassword(myData, username);
        //clearScreen();
        adminMain();
    }

}

int administratorMenu(){
    int choice;


    printf("-------------------------------------------------------------------\n");
    printf("|\t\t\tAdministrator Menu\t\t\t  |\n");
    printf("-------------------------------------------------------------------\n");
    printf("|\t\t     (1) Register new account\t\t          |\n");
    printf("|\t\t     (2) Update account\t\t\t          |\n");
    printf("|\t\t     (3) Log out\t\t\t          |\n");
    printf("-------------------------------------------------------------------\n\n");

    printf(" \t\t\t     Choice: ");


    scanf("%d", &choice);

    while (getchar() != '\n');

    validateMenuChoice(&choice, 1, 3);

    printf("\n-------------------------------------------------------------------\n");

    return choice;
}

void regAccount(){

    int choice;

    printf("\n(1) Student Account");
    printf("\n(2) Programme Coordinator Account");
    printf("\n(3) Programme Leader Account");

    printf("\n\nOption: ");
    scanf("%d", &choice);

    validateMenuChoice(&choice, 1, 3);

    if(choice == 1){
        newStudentAcc();
        //clearScreen();
        adminMain();
    }else if(choice == 2){
        newProgrammeCoordinator();
        //clearScreen();
        adminMain();
    }else{
        newProgrammeLeader();
        //clearScreen();
        adminMain();
    }

}

void adminMain(){
    int option = administratorMenu();

    if (option == 1){
        clearScreen();
        printf("Register new account\n\n");
        regAccount();
    }else if(option == 2){
        clearScreen();
        printf("Update existing account\n\n");
        updateAccount();
    }else if (option == 3){
        clearScreen();
        main();
    }
}

void addESMod() {
    FILE *f = fopen("electivespecialismmodule.txt", "a");

    if (f == NULL) {
        perror("File not found !");
        return;
    }

    char id[20];
    char name[100];
    char credits[5];

    printf("Enter ID: ");
    scanf("%s", id);

    printf("Enter name: ");
    getchar(); // Consume the newline character left by the previous scanf

    fgets(name, sizeof(name), stdin);

    name[strcspn(name, "\n")] = '\0'; // Remove the newline character

    printf("Enter credit score: ");
    scanf("%s", credits);

    fprintf(f, "%s\t%s\t%s\n", id, name, credits);

    fclose(f);

    printf("\n\nNew elective module added !\n");
}

void addTW(){
   FILE *f = fopen("talkworkshop.txt", "a");

    if (f == NULL) {
        perror("File not found !");
        return;
    }

    char id[20];
    char name[100];
    char c[5];

    printf("Enter ID: ");
    scanf("%s", id);

    printf("Enter name: ");
    getchar(); // Consume the newline character left by the previous scanf

    fgets(name, sizeof(name), stdin);
    
    name[strcspn(name, "\n")] = '\0'; // Remove the newline character

    printf("Enter category: ");
    scanf("%s", c);

    fprintf(f, "%s\t%s\t%s\n", id, c, name);

    fclose(f);

    printf("\n\nNew talk/workshop added !\n");
}

void addComp(){
    FILE *f = fopen("competition.txt", "a");

    if (f == NULL) {
        perror("File not found !");
        return;
    }

    char id[20];
    char name[100];
    char c[5];

    printf("Enter ID: ");
    scanf("%s", id);

    printf("Enter name: ");
    getchar(); // Consume the newline character left by the previous scanf

    fgets(name, sizeof(name), stdin);
    
    name[strcspn(name, "\n")] = '\0'; // Remove the newline character

    fprintf(f, "%s\t%s\n", id, name);

    fclose(f);

    printf("\n\nNew competition added !\n");
}

void addCS(){
    FILE *f = fopen("clubsociety.txt", "a");

    if (f == NULL) {
        perror("File not found !");
        return;
    }

    char id[20];
    char name[100];
    char c[5];
    char p[5];

    printf("Enter ID: ");
    scanf("%s", id);

    printf("Enter name: ");
    getchar(); // Consume the newline character left by the previous scanf

    fgets(name, sizeof(name), stdin);
    
    name[strcspn(name, "\n")] = '\0'; // Remove the newline character

    printf("Enter category: ");
    scanf("%s", c);

    printf("Enter points: ");
    scanf("%s", p);

    fprintf(f, "%s\t%s\t%s\t%s\n", id, c, name, p);

    fclose(f);

    printf("\n\nNew club/society added !\n");
}

void programmeCoordinatorMain();

void addActivity(){
    int choice;

    printf("(1) Elective/Specialism Module\n");
    printf("(2) Talk/Workshop\n");
    printf("(3) Club/Society\n");
    printf("(4) Competition\n\n");

    printf("Choice: ");
    scanf("%d", &choice);

    validateMenuChoice(&choice, 1, 4);
    clearScreen();
    if(choice == 1){
        addESMod();
    }else if(choice == 2){
        addTW();
    }else if(choice == 3){
        addCS();
    }else{
        addComp();
    }
    programmeCoordinatorMain();

}

void updateESMod(char* myID) {
    int option;

    printf("(1) Name\n");
    printf("(2) Credit score\n");

    printf("\nOption: ");
    scanf("%d", &option);

    validateMenuChoice(&option, 1, 2);

    char* data = readElecMod();

    FILE *f;
    f = fopen("electivespecialismmodule.txt", "w");

    char* line = strtok(data, "\n");
    char newLine[BUFSIZ];

    while (line != NULL) {
        char i[20];
        char n[100];
        char c[5];

        if (sscanf(line, "%20[^\t]\t%100[^\t]\t%5[^\t]", i, n, c) == 3) {

            if (strcmp(myID, i) == 0) {
                if (option == 1) {
                    char newName[100];
                    getchar();
                    printf("Enter new name: ");
                    fgets(newName, sizeof(newName), stdin);
                    newName[strcspn(newName, "\n")] = '\0';
                    snprintf(newLine, sizeof(newLine), "%s\t%s\t%s", i, newName, c);
                } else {
                    char newC[5];
                    getchar();
                    printf("Enter new credit score: ");
                    fgets(newC, sizeof(newC), stdin);
                    snprintf(newLine, sizeof(newLine), "%s\t%s\t%s", i, n, newC);
                }
            } else {
                snprintf(newLine, sizeof(newLine), "%s\t%s\t%s", i, n, c);
            }

            fprintf(f, "%s", newLine);
        }

        line = strtok(NULL, "\n");
    }

    fclose(f);

    printf("\n\nElective/Specialism module updated !");
}

void updateTW(char* myID){

    char* data = readTWMod();

    FILE *f;
    f = fopen("talkworkshop.txt", "w");

    char* line = strtok(data, "\n");
    char newLine[BUFSIZ];

    while (line != NULL) {
        char i[20];
        char c[5];
        char n[100];

        if (sscanf(line, "%20[^\t]\t%5[^\t]\t%100[^\t]", i, c, n) == 3) {

            if (strcmp(myID, i) == 0) {
                char newName[100];
                getchar();
                printf("Enter new name: ");
                fgets(newName, sizeof(newName), stdin);
                snprintf(newLine, sizeof(newLine), "%s\t%s\t%s", i, c, newName);

            } else {
                snprintf(newLine, sizeof(newLine), "%s\t%s\t%s", i, c, n);
            }

            fprintf(f, "%s", newLine);
        }

        line = strtok(NULL, "\n");
    }

    fclose(f);

    printf("\n\nTalk/Workshop module updated !");
}

void updateCS(char* myID){
    int option;

    printf("(1) Name\n");
    printf("(2) Points\n");

    printf("\nOption: ");
    scanf("%d", &option);

    validateMenuChoice(&option, 1, 2);

    char* data = readCSMod();

    FILE *f;
    f = fopen("clubsociety.txt", "w");

    char* line = strtok(data, "\n");
    char newLine[BUFSIZ];

    while (line != NULL) {
        char i[20];
        char n[100];
        char c[5];
        char p[5];

        if (sscanf(line, "%20[^\t]\t%5[^\t]\t%100[^\t]%5[^\t]", i, c, n, p) == 4) {

            if (strcmp(myID, i) == 0) {
                if (option == 1) {
                    char newName[100];
                    getchar();
                    printf("Enter new name: ");
                    fgets(newName, sizeof(newName), stdin);
                    newName[strcspn(newName, "\n")] = '\0';
                    snprintf(newLine, sizeof(newLine), "%s\t%s\t%s\t%s", i, c, newName, p);
                } else {
                    char newP[5];
                    getchar();
                    printf("Enter new points: ");
                    fgets(newP, sizeof(newP), stdin);
                    snprintf(newLine, sizeof(newLine), "%s\t%s\t%s\t%s", i, c, n, newP);
                }
            } else {
                snprintf(newLine, sizeof(newLine), "%s\t%s\t%s\t%s", i, c, n, p);
            }

            fprintf(f, "%s", newLine);
        }

        line = strtok(NULL, "\n");
    }

    fclose(f);

    printf("\n\nClub/Society module updated !");
}

void updateComp(char* myID){

    FILE *f;
    f = fopen("competition.txt", "w+");

    char* data = readTWMod();


    char* line = strtok(data, "\n");
    char* newLine;

    while(line != NULL){
        char i[20];
        char n[50];

        if(sscanf(line, "%20[^\t]\t%5[^\t]\t%50[^\t]", i, n) == 2){
            if(strcmp(myID, i) == 0){
                
                char newName[50];

                printf("Enter new name: ");
                scanf("%s", newName);

                snprintf(newLine,sizeof(newLine), "%s\t%s\t\n", i, newName);
                

            }else{
                strcpy(newLine, line);
            }
        }
        
        line = strtok(NULL, "\n");
    }

    if(f == NULL){
        perror("File cannot be found !");               
    }else{
        fprintf(f, "%s\n", newLine);
    }
    fclose(f);

    printf("\n\nCompetition updated !");
}

void updateActivity_PC(){
    int choice;

    printf("(1) Elective/Specialism Module\n");
    printf("(2) Talk/Workshop\n");
    printf("(3) Club/Society\n");
    printf("(4) Competition\n\n");

    printf("Choice: ");
    scanf("%d", &choice);

    validateMenuChoice(&choice, 1, 4);
    char theID[20];

    if(choice == 1){
        printf("\n\nEnter module ID: ");
        scanf("%s", theID);

        updateESMod(theID);
    }else if(choice == 2){
        printf("\n\nEnter talk/workshop ID: ");
        scanf("%s", theID);

        updateTW(theID);
    }else if(choice == 3){
        printf("\n\nEnter club/society ID: ");
        scanf("%s", theID);

        updateCS(theID);
    }else{
        printf("\n\nEnter competition ID: ");
        scanf("%s", theID);

        updateComp(theID);
    }

}

void updateCoreMod(){
    int option;
    char myID[20];

    printf("Enter core module ID to update: ");
    scanf("%s", myID);


    printf("\n\n(1) Name\n");
    printf("(2) Description\n");

    printf("\nOption: ");
    scanf("%d", &option);

    validateMenuChoice(&option, 1, 2);

    char* data = readCoreMod();

    FILE *f;
    f = fopen("coremodules.txt", "w");

    char* line = strtok(data, "\n");
    char newLine[BUFSIZ];

    while (line != NULL) {
        char i[20];
        char n[100];
        char d[500];

        if (sscanf(line, "%20[^\t]\t%100[^\t]\t%500[^\t]", i, n, d) == 3) {

            if (strcmp(myID, i) == 0) {
                if (option == 1) {
                    char newName[100];
                    getchar();
                    printf("Enter new name: ");
                    fgets(newName, sizeof(newName), stdin);
                    newName[strcspn(newName, "\n")] = '\0';
                    snprintf(newLine, sizeof(newLine), "%s\t%s\t%s", i, newName, d);
                } else {
                    char newD[500];
                    getchar();
                    printf("Enter new description: ");
                    fgets(newD, sizeof(newD), stdin);
                    snprintf(newLine, sizeof(newLine), "%s\t%s\t%s", i, n, newD);
                }
            } else {
                snprintf(newLine, sizeof(newLine), "%s\t%s\t%s", i, n, d);
            }

            fprintf(f, "%s", newLine);
        }

        line = strtok(NULL, "\n");
    }

    fclose(f);

    printf("\n\nCore module updated !\n");

    clearScreen();
}
//C0004	Software Engineering	Gain insights into the software development lifecycle and best practices in this core module.
void addCoreMod(){
    char* myCore = readCoreMod();
    int numMod = countNumLines(myCore) - 1;

    if (numMod >= 4){
        printf("\n\nAlready have 4 core modules. Cannot add !\n");
    }else{
        char id[20];
        char n[50];
        char des[500];

        FILE *f = fopen("coremodules.txt", "a");

        if (f == NULL) {
            perror("File not found !");
            return;
        }

        printf("Enter ID: ");
        scanf("%s", id);

        printf("Enter name: ");
        getchar(); // Consume the newline character left by the previous scanf

        fgets(n, sizeof(n), stdin);
        
        n[strcspn(n, "\n")] = '\0'; // Remove the newline character

        printf("Enter description: ");
        fgets(des, sizeof(des), stdin);

        fprintf(f, "\n%s\t%s\t%s\n", id, n, des);
        
        printf("\n\nCore Module added !\n");
        fclose(f);
    }
}

void displayCoreMod(){
    FILE *f = NULL;

    f = fopen("coremodules.txt", "r");

    if(f == NULL){
        perror("File not found");
    }else{
        char data[150];
        while(fgets(data, sizeof(data), f) != NULL){
            printf("%s", data);
        }
    }
    fclose(f);
}

int programmeCoordinatorMenu(){
    int choice;


    printf("-------------------------------------------------------------------\n");
    printf("|\t\t     Programme Coordinator Menu\t\t          |\n");
    printf("-------------------------------------------------------------------\n");
    printf("|\t\t     (1) Add new activity\t\t          |\n");
    printf("|\t\t     (2) Update activity\t\t          |\n");
    printf("|\t\t     (3) Identification of core modules\t\t  |\n");
    printf("|\t\t     (4) Log out\t\t\t\t  |\n");
    printf("-------------------------------------------------------------------\n\n");
    printf(" \t\t\t     Choice: ");

    scanf("%d", &choice);

    while (getchar() != '\n');

    validateMenuChoice(&choice, 1, 4);

    printf("\n-------------------------------------------------------------------\n");


    return choice;
}

void coreModulesDef(){
    printf("Identification of core modules\n\n");
    printf("\n(1) Update core modules");
    printf("\n(2) Add core modules");
    printf("\n(3) Display core modules");

    int choice;

    printf("\n\nChoice: ");
    scanf("%d", &choice);

    validateMenuChoice(&choice, 1, 3);

    clearScreen();

    if(choice == 1){
        updateCoreMod();
    }else if(choice == 2){
        addCoreMod();
    }else{
        displayCoreMod();
    }


    programmeCoordinatorMenu();
}

void programmeCoordinatorMain(){
    int option = programmeCoordinatorMenu();

    clearScreen();

    if(option == 1){
        printf("Adding activity\n\n");
        addActivity();
    }else if(option == 2){
        updateActivity_PC();
    }else if(option == 3){
        coreModulesDef();
    }else{
        clearScreen();
        main();
    }


}

void programmeLeaderMain();

int programmeLeaderMenu(){
    int choice;


    printf("-------------------------------------------------------------------\n");
    printf("|\t\t     Programme Leader Menu\t\t          |\n");
    printf("-------------------------------------------------------------------\n");
    printf("|\t\t     (1) Track activity\t\t\t          |\n");
    printf("|\t\t     (2) Generate report\t\t          |\n");
    printf("|\t\t     (3) Log out\t\t\t\t  |\n");
    printf("-------------------------------------------------------------------\n\n");
    printf(" \t\t\t     Choice: ");

    scanf("%d", &choice);

    while (getchar() != '\n');

    validateMenuChoice(&choice, 1, 3);

    printf("\n-------------------------------------------------------------------\n");


    return choice;
}

int comparePoints(const void *x, const void *y) {
    return strcmp(((struct studentDetails *)y)->points, ((struct studentDetails *)x)->points);
}

void ranking(){
    char* studStats = readStudentData();

    int studNo = countNumLines(studStats);

    printf("Ranking: \n\n");
    if (studStats == NULL) {
        return;
    }else{

        struct studentDetails *stud = malloc(studNo * sizeof(struct studentDetails));

        if (stud == NULL) {
            free(studStats);
            return;
        }

        char* line = strtok(studStats, "\n");
        for (int i = 0; i <= studNo && line != NULL; i++) {
            if (sscanf(line, "%50[^\t]\t%20[^\t]\t%90[^\t]\t%200[^\t]\t%5[^\t]", stud[i].name, stud[i].id, stud[i].coreModules, stud[i].activities, stud[i].points) == 5) {
                line = strtok(NULL, "\n");
            }
        }


        qsort(stud, studNo, sizeof(struct studentDetails), comparePoints);

        for (int i = 0; i < studNo - 1; i++) {
            printf("%d.\t%-20s\t%s\n", i + 1 , stud[i].name, stud[i].points);
        }

        free(stud);
        free(studStats);
    }    
}

void studBelThresh(){
    char *studStats = readStudentData();
    int count = 0;

    if (studStats == NULL) {
        perror("\nNo data found");
    }else{

        char* line = studStats;
        printf("Students below threshold: \n\n");
        while ((line = strtok(line, "\n")) != NULL) {

            struct studentDetails myStud;

            if (sscanf(line, "%50[^\t]\t%20[^\t]\t%90[^\t]\t%200[^\t]\t%5[^\t]", myStud.name, myStud.id, myStud.coreModules, myStud.activities, myStud.points) == 5) {

                int points = atoi(myStud.points);
                if (points < 160) {
                    count++;
                    printf("%-20s\t%s\n", myStud.name, myStud.points);
                }

                line = NULL;

            } else {
                free(studStats);
                if(count == 0){
                    printf("\nNo students to display !\n");
                }
                return;
            }
        }
    }
    free(studStats);
}

void generateReport(){
    printf("\n(1) Ranking per points");
    printf("\n(2) List of students below threshold credits/points");

    int choice;

    printf("\n\nChoice: ");
    scanf("%d", &choice);

    validateMenuChoice(&choice, 1, 2);

    clearScreen();

    if(choice == 1){
        ranking();
    }else{
        studBelThresh();
    }

    if(getchar() == '\n'){
        clearScreen();
        programmeLeaderMain();
    }
}

// programme leader main
void programmeLeaderMain(){
    int option = programmeLeaderMenu();

    if(option == 1){
        char myID[20];
        printf("\n\nEnter students ID: ");
        scanf("%s", myID);

        trackActivities(myID);

    }else if(option == 2){
        generateReport();
    }else{
        clearScreen();
        main();
    }
}
    
// main function

int main(){

  
    int choice;
    char* type; 

    // calls login function
    type = logIn();

    if (strcmp(trimSpaces(type), "student") == 0){
        printf("\t\t\t     Logging in... ");
        getchar();
        clearScreen();
        studentMain();
    }else if(strcmp(trimSpaces(type), "programme leader") == 0){
        printf("\t\t\t     Logging in... ");
        getchar();
        clearScreen();
        programmeLeaderMain();
    }else if(strcmp(trimSpaces(type),"programme coordinator") == 0){
        printf("\t\t\t     Logging in... ");
        getchar();
        clearScreen();
        programmeCoordinatorMain();
    }else if(strcmp(trimSpaces(type), "admin") == 0){
        printf("\t\t\t     Logging in... ");
        getchar();
        clearScreen();
        adminMain();
    }else{
        printf("\nUser not found");
    }
    



}

