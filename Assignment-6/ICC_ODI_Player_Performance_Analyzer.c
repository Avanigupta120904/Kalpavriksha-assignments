#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Players_data.h" 

#define MAX_NAME_LENGTH 51    
#define MAX_PLAYER_ID 1500
#define NUMBER_OF_TEAMS 10
#define INPUT_BUFFER_SIZE 256

typedef struct PlayerNode{
    int playerID;
    char* name;
    char* teamName;
    char* role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerNode* next;
} PlayerNode;

typedef struct{
    int teamID; 
    char* name;
    int totalPlayers;
    int totalBatsmenAllRounders; 
    float averageBattingStrikeRate; 
    float avgStrikeRateBatterAllRounder; 
    PlayerNode *batsmen;
    PlayerNode *bowlers;
    PlayerNode *allRounders;
} TeamNode;

TeamNode teamList[NUMBER_OF_TEAMS];
TeamNode* teamPtrArr[NUMBER_OF_TEAMS]; 

int stringLength(const char* str){
    int length = 0;
    while(str && *str){
        length++;
        str++;
    }
    return length;
}

char* stringCopy(const char* source){
    if(!source) return NULL;
    size_t len = (size_t)stringLength(source);
    char* destination = (char*)malloc(len + 1);
    if(!destination) return NULL;
    char* ptr = destination;
    while(*source){
        *ptr++ = *source++;
    }
    *ptr = '\0';
    return destination;
}

int stringCompare(const char* str1, const char* str2){
    if(!str1 && !str2) return 0;
    if(!str1) return -1;
    if(!str2) return 1;
    while(*str1 && *str2){
        if((unsigned char)*str1 < (unsigned char)*str2) return -1;
        if((unsigned char)*str1 > (unsigned char)*str2) return 1;
        str1++; str2++;
    }
    if(*str1) return 1;
    if(*str2) return -1;
    return 0;
}

static char inputBuffer[INPUT_BUFFER_SIZE];

void readLine(char *dst, size_t size){
    while(1){
        if(!fgets(dst, (int)size, stdin)){
            clearerr(stdin);
            dst[0] = '\0';
            return;
        }
        size_t L = strlen(dst);
        if(L > 0 && dst[L-1] == '\n') dst[L-1] = '\0';
        return;
    }
}

int parseInteger(const char* str, int *out){
    if(!str) return 0;
    char extra;
    if(sscanf(str, " %d %c", out, &extra) == 1) return 1;
    return 0;
}

int parseFloat(const char* str, float *out){
    if(!str) return 0;
    char extra;
    if(sscanf(str, " %f %c", out, &extra) == 1) return 1;
    return 0;
}

int getValidInteger(){
    int number;
    while(1){
        readLine(inputBuffer, sizeof(inputBuffer));
        if(parseInteger(inputBuffer, &number)){
            return number;
        } else {
            printf("Invalid input. Please enter an integer: ");
        }
    }
}

float getValidFloat(){
    float number;
    while(1){
        readLine(inputBuffer, sizeof(inputBuffer));
        if(parseFloat(inputBuffer, &number)){
            return number;
        } else {
            printf("Invalid input. Please enter a floating-point number: ");
        }
    }
}

int getValidRole(){
    int role;
    while(1){
        readLine(inputBuffer, sizeof(inputBuffer));
        if(parseInteger(inputBuffer, &role) && role >= 1 && role <= 3){
            return role;
        } else {
            printf("Invalid Role. Enter 1 (Batsman), 2 (Bowler) or 3 (All-rounder): ");
        }
    }
}

int getValidTeamId(){
    while(1){
        int id = getValidInteger();
        if(id < 1 || id > teamCount){
            printf("Enter ID in range 1-%d: ", teamCount);
        } else {
            return id;
        }
    }
}

int playerIdExists(int id){
    for(int i = 0; i < teamCount; i++){
        PlayerNode* temp = teamList[i].batsmen;
        while(temp != NULL){
            if(temp->playerID == id) return 1;
            temp = temp->next;
        }
        temp = teamList[i].allRounders;
        while(temp != NULL){
            if(temp->playerID == id) return 1;
            temp = temp->next;
        }
        temp = teamList[i].bowlers;
        while(temp != NULL){
            if(temp->playerID == id) return 1;
            temp = temp->next;
        }
    }
    return 0;
}

int getValidPlayerId(){
    while(1){
        int id = getValidInteger();
        if(id < 1 || id > MAX_PLAYER_ID){
            printf("Enter ID in range 1-%d: ", MAX_PLAYER_ID);
        }
        else if(playerIdExists(id)){
            printf("ID already exists! Enter again: ");
        }
        else{
            return id;
        }
    }
}

int teamPtrCompare(const void* a, const void* b){
    TeamNode* ta = *(TeamNode**)a;
    TeamNode* tb = *(TeamNode**)b;
    return stringCompare(ta->name, tb->name);
}

int binarySearchTeam(const char* teamName){
    int left = 0, right = teamCount - 1;
    while(left <= right){
        int mid = left + (right - left) / 2;
        int cmp = stringCompare(teamPtrArr[mid]->name, teamName);
        if(cmp == 0){
            TeamNode* found = teamPtrArr[mid];
            for(int i = 0; i < teamCount; i++){
                if(&teamList[i] == found) return i;
            }
            return -1;
        }
        else if(cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

float calculatePerformanceIndex(PlayerNode* newPlayer){
    float performanceIndex = 0.0f;
    if(stringCompare(newPlayer->role, "Batsman") == 0){
        performanceIndex = (newPlayer->battingAverage * newPlayer->strikeRate) / 100.0f;
    }
    else if(stringCompare(newPlayer->role, "Bowler") == 0){
        performanceIndex = (newPlayer->wickets * 2.0f) + (100.0f - newPlayer->economyRate);
    }
    else{
        performanceIndex = ((newPlayer->battingAverage * newPlayer->strikeRate) / 100.0f) + (newPlayer->wickets * 2.0f);
    }
    return performanceIndex;
}

void insertSorted(PlayerNode** head, PlayerNode* newPlayer){
    if(*head == NULL || newPlayer->performanceIndex > (*head)->performanceIndex){
        newPlayer->next = *head;
        *head = newPlayer;
        return;
    }
    PlayerNode* current = *head;
    while(current->next != NULL && current->next->performanceIndex > newPlayer->performanceIndex){
        current = current->next;
    }
    newPlayer->next = current->next;
    current->next = newPlayer;
}

void updateTeam(PlayerNode* newPlayer){
    int teamIndex = binarySearchTeam(newPlayer->teamName);
    if(teamIndex == -1){
        printf("No team found for name '%s'. Player not assigned.\n", newPlayer->teamName);
        return;
    }

    if(stringCompare(newPlayer->role, "Batsman") == 0){
        insertSorted(&teamList[teamIndex].batsmen, newPlayer);
        float prevAvg = teamList[teamIndex].averageBattingStrikeRate;
        int prevCount = teamList[teamIndex].totalBatsmenAllRounders;
        teamList[teamIndex].averageBattingStrikeRate = ((prevAvg * prevCount) + newPlayer->strikeRate) / (prevCount + 1);
        teamList[teamIndex].totalBatsmenAllRounders++;
        teamList[teamIndex].avgStrikeRateBatterAllRounder = teamList[teamIndex].averageBattingStrikeRate;
    }
    else if(stringCompare(newPlayer->role, "All-rounder") == 0){
        insertSorted(&teamList[teamIndex].allRounders, newPlayer);
        float prevAvg = teamList[teamIndex].averageBattingStrikeRate;
        int prevCount = teamList[teamIndex].totalBatsmenAllRounders;
        teamList[teamIndex].averageBattingStrikeRate = ((prevAvg * prevCount) + newPlayer->strikeRate) / (prevCount + 1);
        teamList[teamIndex].totalBatsmenAllRounders++;
        teamList[teamIndex].avgStrikeRateBatterAllRounder = teamList[teamIndex].averageBattingStrikeRate;
    }
    else{
        insertSorted(&teamList[teamIndex].bowlers, newPlayer);
    }

    teamList[teamIndex].totalPlayers++;
}

void initializeTeams(){
    for(int i = 0; i < teamCount && i < NUMBER_OF_TEAMS; i++){
        teamList[i].teamID = i + 1;
        teamList[i].name = stringCopy(teams[i]); 
        teamList[i].totalPlayers = 0;
        teamList[i].totalBatsmenAllRounders = 0;
        teamList[i].averageBattingStrikeRate = 0.0f;
        teamList[i].avgStrikeRateBatterAllRounder = 0.0f;
        teamList[i].batsmen = NULL;
        teamList[i].bowlers = NULL;
        teamList[i].allRounders = NULL;
        teamPtrArr[i] = &teamList[i]; 
    }
    qsort(teamPtrArr, teamCount, sizeof(TeamNode*), teamPtrCompare);
}

void initializePlayers(){
    for(int i = 0; i < playerCount; i++){
        if (playerIdExists(players[i].id)) {
            printf("Duplicate Player ID %d found in dataset! Skipping.\n", players[i].id);
            continue;
        }

        PlayerNode *newPlayer = (PlayerNode*)malloc(sizeof(PlayerNode));
        if(!newPlayer){
            fprintf(stderr, "Memory allocation failed for player %d\n", i);
            continue;
        }
        newPlayer->playerID = players[i].id;
        newPlayer->name = stringCopy(players[i].name);
        newPlayer->teamName = stringCopy(players[i].team);
        newPlayer->role = stringCopy(players[i].role);
        newPlayer->totalRuns = players[i].totalRuns;
        newPlayer->battingAverage = players[i].battingAverage;
        newPlayer->strikeRate = players[i].strikeRate;
        newPlayer->wickets = players[i].wickets;
        newPlayer->economyRate = players[i].economyRate;
        newPlayer->performanceIndex = calculatePerformanceIndex(newPlayer);
        newPlayer->next = NULL;
        updateTeam(newPlayer); 
    }
}

void addNewPlayer(){
    PlayerNode* newPlayer = (PlayerNode*)malloc(sizeof(PlayerNode));
    if(!newPlayer){
        printf("Failed to allocate memory!\n");
        return;
    }
    memset(newPlayer, 0, sizeof(PlayerNode));

    printf("Enter Team ID to add player: ");
    int teamID = getValidTeamId();
    newPlayer->teamName = stringCopy(teamList[teamID - 1].name);

    printf("Enter Player Details:\n");

    printf("Player ID: ");
    newPlayer->playerID = getValidPlayerId();

    while(1){
        printf("Name (1-%d chars): ", MAX_NAME_LENGTH - 1);
        readLine(inputBuffer, sizeof(inputBuffer));
        size_t len = strlen(inputBuffer);
        if(len == 0){
            printf("Name cannot be empty. ");
            continue;
        }
        if(len >= MAX_NAME_LENGTH){
            printf("Name too long. Please use at most %d characters. ", MAX_NAME_LENGTH - 1);
            continue;
        }
        newPlayer->name = stringCopy(inputBuffer);
        break;
    }

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int role = getValidRole();
    if(role == 1) newPlayer->role = stringCopy("Batsman");
    else if(role == 2) newPlayer->role = stringCopy("Bowler");
    else newPlayer->role = stringCopy("All-rounder");

    printf("Total Runs: ");
    newPlayer->totalRuns = getValidInteger();

    printf("Batting Average: ");
    newPlayer->battingAverage = getValidFloat();

    printf("Strike Rate: ");
    newPlayer->strikeRate = getValidFloat();

    printf("Wickets: ");
    newPlayer->wickets = getValidInteger();

    printf("Economy Rate: ");
    newPlayer->economyRate = getValidFloat();

    newPlayer->performanceIndex = calculatePerformanceIndex(newPlayer);
    newPlayer->next = NULL;

    updateTeam(newPlayer);

    printf("Player added successfully to Team %s!\n", newPlayer->teamName);
}

void playerDetails(PlayerNode* p){
    if(!p) return;
    printf("%-5d %-20s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n",
           p->playerID,
           p->name ? p->name : "-",
           p->role ? p->role : "-",
           p->totalRuns,
           p->battingAverage,
           p->strikeRate,
           p->wickets,
           p->economyRate,
           p->performanceIndex);
}

void displayPlayersOfTeam(){
    printf("Enter Team ID: ");
    int teamID = getValidTeamId();
    TeamNode* t = &teamList[teamID - 1];

    printf("Players of Team %s:\n", t->name);
    printf("\n====================================================================================================\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("\n====================================================================================================\n");

    PlayerNode* temp = t->batsmen;
    while(temp != NULL){
        playerDetails(temp);
        temp = temp->next;
    }
    temp = t->allRounders;
    while(temp != NULL){
        playerDetails(temp);
        temp = temp->next;
    }
    temp = t->bowlers;
    while(temp != NULL){
        playerDetails(temp);
        temp = temp->next;
    }
    printf("\n====================================================================================================\n");
    printf("\nTotal Players: %d\n", t->totalPlayers);
    printf("Average Batting Strike Rate (batters + all-rounders only): %.2f\n", t->averageBattingStrikeRate);
}

void displayTeamsBySR(){
    int teamIndices[NUMBER_OF_TEAMS];
    for(int i = 0; i < teamCount; i++) teamIndices[i] = i;
    for(int i = 0; i < teamCount; i++){
        for(int j = 0; j < teamCount - i - 1; j++){
            if(teamList[teamIndices[j]].averageBattingStrikeRate < teamList[teamIndices[j + 1]].averageBattingStrikeRate){
                int tmp = teamIndices[j];
                teamIndices[j] = teamIndices[j + 1];
                teamIndices[j + 1] = tmp;
            }
        }
    }
    printf("Teams Sorted by Average Batting Strike Rate");
    printf("\n=========================================================\n");
    printf("%-3s %-15s %-14s %-12s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("\n=========================================================\n");
    for(int i = 0; i < teamCount; i++){
        TeamNode* t = &teamList[teamIndices[i]];
        printf("%-3d %-15s %-14.2f %-12d\n", t->teamID, t->name, t->averageBattingStrikeRate, t->totalPlayers);
    }
    printf("\n=========================================================\n");
}

void displayTopKPlayers(){
    printf("Enter Team ID: ");
    int teamID = getValidTeamId();
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int role = getValidRole();
    printf("Enter number of players: ");
    int k = getValidInteger();

    TeamNode* t = &teamList[teamID - 1];
    PlayerNode* temp;
    const char* roleStr = (role == 1 ? "Batsmen" : (role == 2 ? "Bowlers" : "All-rounders"));
    if(role == 1) temp = t->batsmen;
    else if(role == 2) temp = t->bowlers;
    else temp = t->allRounders;

    printf("Top %d %s of Team %s:\n", k, roleStr, t->name);
    printf("\n====================================================================================================\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("\n====================================================================================================\n");

    while(k-- > 0 && temp != NULL){
        playerDetails(temp);
        temp = temp->next;
    }
    printf("\n====================================================================================================\n");
}

void displayAllPlayersByRole(){
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int role = getValidRole();

    PlayerNode* teamPlayers[NUMBER_OF_TEAMS];
    for(int i = 0; i < teamCount; i++){
        if(role == 1) teamPlayers[i] = teamList[i].batsmen;
        else if(role == 2) teamPlayers[i] = teamList[i].bowlers;
        else teamPlayers[i] = teamList[i].allRounders;
    }

    printf("\n====================================================================================================================\n");
    printf("%-5s %-20s %-15s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("\n====================================================================================================================\n");

    while(1){
        int bestIndex = -1;
        float bestPI = -1.0f;
        for(int i = 0; i < teamCount; i++){
            if(teamPlayers[i] != NULL){
                if(teamPlayers[i]->performanceIndex > bestPI){
                    bestPI = teamPlayers[i]->performanceIndex;
                    bestIndex = i;
                }
            }
        }
        if(bestIndex == -1) break; 
        PlayerNode* temp = teamPlayers[bestIndex];
        printf("%-5d %-20s %-15s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n",
               temp->playerID, temp->name, temp->teamName, temp->role,
               temp->totalRuns, temp->battingAverage, temp->strikeRate,
               temp->wickets, temp->economyRate, temp->performanceIndex);
        teamPlayers[bestIndex] = teamPlayers[bestIndex]->next;
    }

    printf("\n====================================================================================================================\n");
}

void freeMemory(){
    for(int i = 0; i < teamCount; i++){
        PlayerNode* lists[3] = {teamList[i].batsmen, teamList[i].bowlers, teamList[i].allRounders};
        for(int role = 0; role < 3; role++){
            PlayerNode* current = lists[role];
            while(current != NULL){
                PlayerNode* temp = current;
                current = current->next;
                if(temp->name) free(temp->name);
                if(temp->teamName) free(temp->teamName);
                if(temp->role) free(temp->role);
                free(temp);
            }
        }
        if(teamList[i].name) free(teamList[i].name);
    }
}

void displayMenu(){
    printf("\n==============================================================================\n");
    printf("ICC ODI Player Performance Analyzer\n");
    printf("\n==============================================================================\n");
    printf("1. Add Player to Team\n");
    printf("2. Display Players of a Specific Team\n");
    printf("3. Display Teams by Average Batting Strike Rate\n");
    printf("4. Display Top K Players of a Specific Team by Role\n");
    printf("5. Display all Players of specific role Across All Teams by performance index\n");
    printf("6. Exit\n");
    printf("\n==============================================================================\n");
    printf("Enter your choice: ");
}

void initiateAnalyzer(){
    int choice = 0;
    while(choice != 6){
        displayMenu();
        choice = getValidInteger();
        switch(choice){
            case 1:
                addNewPlayer();
                break;
            case 2:
                displayPlayersOfTeam();
                break;
            case 3:
                displayTeamsBySR();
                break;
            case 4:
                displayTopKPlayers();
                break;
            case 5:
                displayAllPlayersByRole();
                break;
            case 6:
                freeMemory();
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

int main(){
    if(teamCount < 1 || teamCount > NUMBER_OF_TEAMS){
        fprintf(stderr, "Invalid teamCount in Players_data.h (expected 1..%d). Found: %d\n", NUMBER_OF_TEAMS, teamCount);
        return 1;
    }
    initializeTeams();
    initializePlayers();
    initiateAnalyzer();
    return 0;
}
