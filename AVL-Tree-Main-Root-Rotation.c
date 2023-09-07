//Order of Operation is written in pdf file

#include <stdlib.h>  
#include <stdio.h>  
#include <math.h>  
#include <string.h> 
 
  
#define PLAYERS 1000  
#define STAGES 1000 

// A 2D integer array stagePlayers storing the stages and their players
int stagePlayers[STAGES][PLAYERS]; 

 
typedef struct {  
    int playersNum;  
    int beaten[PLAYERS][PLAYERS];  
    int beatenNum[PLAYERS];  
} Player;  
  
typedef struct {  
    int numPlayers;  
    Player *players;  
} Stage;  
  

// Define initializer that initializes a Stage struct 
Stage* stageInitializer(int numPlayers) {  
    Stage *stage = calloc(1, sizeof(Stage));
    stage->numPlayers = numPlayers;  
    stage->players = calloc(numPlayers, sizeof(Player));
    for (int i = 0; i < numPlayers; i++) {  
        Player *player = &stage->players[i];
        player->playersNum = i + 1;
        player->beatenNum[0] = 0;  
        memset(player->beaten[0], 0, PLAYERS * sizeof(int));
    }  
    return stage;  
}
  
// a function to free the stages
void freeStage(Stage *stage) {  
    free(stage->players);  
    free(stage);  
}  


// Following the matches' process with this function
int Matches(Player *firstPlayer, Player *secondPlayer, int numOfStages, Player **allPlayers) {
    int winnerNum;
    printf("\n- Match -> Player %d and Player %d\n", firstPlayer->playersNum, secondPlayer->playersNum);
    do {
        printf("- Enter the winner: ");
        scanf("%d", &winnerNum);
        // Check if the entered number is among the players of the current match
        if (winnerNum != firstPlayer->playersNum && winnerNum != secondPlayer->playersNum) {
            printf("\n\n!! This player is not playing in this match.\n\n");
        }
    } while (winnerNum != firstPlayer->playersNum && winnerNum != secondPlayer->playersNum);

    // Add the beaten player to the beaten list for the current stage
    Player *winner;
    Player *loser;
    //define a loser and winner in each match
    if (winnerNum == firstPlayer->playersNum) {
        winner = firstPlayer;
        loser = secondPlayer;
    } else {
        winner = secondPlayer;
        loser = firstPlayer;
    }
    winner->beaten[numOfStages][winner->beatenNum[numOfStages]] = loser->playersNum;
    winner->beatenNum[numOfStages]++;

    // Add the beaten player to the beaten list for the previous stage
    for (int i = 0; i < numOfStages; i++) {
        Player *p = allPlayers[winner->playersNum - 1];
        int index = p->beatenNum[i];
        p->beaten[i][index] = loser->playersNum;
        p->beatenNum[i]++;
    }

    // Update players beaten by the loser player and finilize it
    int numBeaten = loser->beatenNum[numOfStages - 1];
    if (numBeaten > 0) {
        int index = winner->beatenNum[numOfStages];
        int Bytes = numBeaten * sizeof(int);
        memcpy(winner->beaten[numOfStages] + index, loser->beaten[numOfStages - 1], Bytes);
        winner->beatenNum[numOfStages] += numBeaten;
    }

    return winnerNum;
}



// Following the matches in each of the stages
void stageGames(Stage *stage, int numOfStages, Player **allPlayers) {  
    printf("\nStage %d\n", numOfStages);  
    for (int i = 0; i < stage->numPlayers; i++) { 
        stagePlayers[numOfStages-1][i] = stage->players[i].playersNum; 
    } 
    //printing the winner
    if (stage->numPlayers == 1) {  
        printf("Winner: Player %d\n\n", stage->players[0].playersNum);  
        return;  
    }  
        // Initialize the number of matches for the current stage
        int num_matches = stage->numPlayers / 2;

        // Initialize the next stage
        Stage *next_stage = stageInitializer(num_matches);
        
        // Loop through all the matches in the current stage
        for (int i = 0; i < num_matches; i++) {
        // Play the match between players and get the winner
        int winnerNum = Matches(&stage->players[i * 2], &stage->players[i * 2 + 1], numOfStages, allPlayers);  
        next_stage->players[i].playersNum = winnerNum;  
    }  
    // Check the next stage and free the current stage
    stageGames(next_stage, numOfStages + 1, allPlayers);  
    freeStage(next_stage);  
}  
  
  
// Print beaten players
void beatenPlayers(Player *player) {
    printf("\nBeaten players for Player %d:\n", player->playersNum);
    int numBeaten = player->beatenNum[0];
    if (numBeaten == 0) {
    printf("No beaten players.\n");
    } else {
    for (int i = 0; i < numBeaten; i++) {
    printf("Player %d \n", player->beaten[0][i]);
    }
    }
}

// Print the players of the stages
void PlayersOfStages(int numPlayers, int stage) {
    printf("Players at stage %d:\n", stage); 
    // Calculate the number of stages
    int stageNum = (int) ceil(log2(numPlayers)); 
    int indexOfStages = stage - 1; 
    if (indexOfStages > stageNum) { 
        printf("This stage is not available\n");
    return;
}
// Loop through array and print the players
    for (int j = 0; j < numPlayers && stagePlayers[indexOfStages][j]!= 0 ; j++) { 
    printf("Player %d ", stagePlayers[indexOfStages][j]);
    }
printf("\n"); 
}

// Print the winner tree
void winnerTree(int stagePlayers[STAGES][PLAYERS]) {
    printf("\nWinner Tree:\n\n");
     // initial space of 1 between numbers
    int space = 1;
    // initializing the last element for findign the winner
    int lastElement = 0; 
    // Each time the space between elements of the row got *2
    const int TAB_WIDTH = 2;
    // Print the first row
    for (int j = 0; j < PLAYERS; j++) {
        if (stagePlayers[0][j] == 0) {
            break;
        }
        printf("%d ", stagePlayers[0][j]);
        for (int s = 0; s < space - 1; s++) {
            printf("%*s", TAB_WIDTH, "");
        }
    }
    printf("\n");
    // Double the space between elements of the next stage
    space *= 2;
    // Loop through each stage and print their values
    for (int i = 1; i < STAGES; i++) {
        if (stagePlayers[i][0] == 0) {
            break;
        }
        // The number of tabs before and after each row
        const int TABS_BEFORE = space / 2;
        const int TABS_AFTER = space / 2;
        // Print the tabs before the row
        for (int k = 0; k < TABS_BEFORE; k++) {
            printf("%*s", TAB_WIDTH, "  ");
        }
        // Print the players of the stage
        for (int j = 0; j < PLAYERS; j++) {
            if (stagePlayers[i][j] == 0) {
                break;
            }
            printf("%d ", stagePlayers[i][j]);
            for (int s = 0; s < space - 1; s++) {
                printf("%*s", TAB_WIDTH, "");
            }
            lastElement = stagePlayers[i][j];
        }
        // Print the tabs after the row
        for (int k = 0; k < TABS_AFTER; k++) {
            printf("%*s", TAB_WIDTH, "");
        }
        printf("\n");
        // Double the space between elements for the next stage
        space *= 2;
    }
    // Print the winner
    printf("\nWinner of this Game is player: %d\n", lastElement);
}
// The last match a player has been beaten in
int LostAgainst(int playerNum, Stage *stage) {
    for (int i = 0; i < stage->numPlayers; i++) {
        Player *player = &stage->players[i];
        for (int j = 0; j < player->beatenNum[0]; j++) {
            if (player->beaten[0][j] == playerNum) {
                printf("\nthe last match this player has been beaten in is with %d, and the player got lost in this match\n", player->playersNum);
            }
        }
    }
}



int main() {  
    int numPlayers;  
    do { 
        printf("Enter number of players.\n !!Make sure to enter a number with power 2: ");  
        scanf("%d", &numPlayers); 
    } while ((numPlayers & (numPlayers - 1)) != 0); 
    // Initialize stage
    Stage *stage = stageInitializer(numPlayers);  
    // An array of pointers to all players  
    Player **allPlayers = (Player**)malloc(sizeof(Player*) * numPlayers);  
    for (int i = 0; i < numPlayers; i++) {  
        allPlayers[i] = &stage->players[i];  
    }  
    // Play the tournament
    stageGames(stage, 1, allPlayers);  
    int choice = 0;  
    do {  
        printf("\nChoose one of the options:\n");  
        printf("1. Display Players at Given Stage\n");  
        printf("2. Check the Beaten Players\n");  
        printf("3. Print Winner Tree\n");  
        printf("4. Exit\n");  
        scanf("%d", &choice);  
  
        switch (choice) {  
            case 1:  
                printf("Enter the number of the stage to see its players:\n ");  
                int numOfStages;  
                scanf("%d", &numOfStages);  
                PlayersOfStages(numPlayers, numOfStages); 
                break;  
            case 2:  
                printf("Enter player number to see its beaten players:\n ");  
                int playersNum;  
                scanf("%d", &playersNum);  
                Player *player = NULL;  
                for (int j = 0; j < numPlayers; j++) {  
                    if (stage->players[j].playersNum == playersNum) {  
                        player = &stage->players[j];  
                        break;  
                    }  
                }  
                // If the player is found, print its beaten players  
                if (player != NULL) {  
                    beatenPlayers(player); 
                    LostAgainst(playersNum,stage);
                    
                } else {  
                    printf("Player not found.\n");  
                }  
                break;  
                 
            case 3:  
                winnerTree(stagePlayers);
                break; 
                 
            case 4: 
                printf("Exiting...\n");  
                break;  
            default:  
                printf("Wrong Input. Enter your choice again.\n");  
        }  
    } while (choice != 4);  
  
    // Free the allocated memory 
    freeStage(stage);  
    free(allPlayers);  
    return 0;  
}

/*Pseudocode

BEGIN
DEFINE STAGES as a two-dimensional array with size of [STAGES][PLAYERS]
DEFINE STRUCT Player with playersNum, beaten array, and beatenNum
DEFINE STRUCT Stage with numPlayers and players



FUNCTION stageInitializer(numPlayers)
    SET stage as a pointer to the Stage struct
    ALLOCATE memory for stage 
    SET stage's numPlayers member to numPlayers
    ALLOCATE memory for stage's players member 
    FOR i = 0 to numPlayers
        SET player as pointer to the player in i-th position
        SET player's playersNum to i + 1
        SET player's beatenNum index to 0
        SET elements in beaten array at index 0 to PLAYERS to 0 
    RETURN stage
END FUNCTION


FUNCTION freeStage(stage)
    FREE memory for the array of Players
    FREE memory for stage


FUNCTION Matches(firstPlayer, secondPlayer, numOfStages, allPlayers)
    SET winnerNum to 0
    WHILE winnerNum is not "firstPlayer->playersNum" or "secondPlayer->playersNum" DO
        GET input as winnerNum
    IF "winnerNum" is not equal to "firstPlayer->playersNum" and "secondPlayer->playersNum" THEN
        PRINT "This player is not playing in this match."
    ENDIF
    ENDWHILE
    
    SET winner and loser
    INCREMENT "winner->beatenNum[numOfStages]"
    
    FOR i = 0 to numOfStages - 1 DO
        SET "p" to "allPlayers[winner->playersNum - 1]"
        SET "index" to "p->beatenNum[i]"
        SET "p->beaten[i][index]" to "loser->playersNum"
        INCREMENT "p->beatenNum[i]"
    ENDFOR
    
    SET "numBeaten" 
    IF "numBeaten" is greater than 0 THEN
        SET "index" to "winner->beatenNum[numOfStages]"
        SET "Bytes" to "numBeaten * sizeof(int)"
        COPY "loser->beaten[numOfStages - 1]" to "winner->beaten[numOfStages]
        INCREMENT "winner->beatenNum[numOfStages]" 
    ENDIF
RETURN "winnerNum"


FUNCTION stageGames(Stage, numOfStages, allPlayers)
    FOR i FROM 0 TO stage.numPlayers - 1
        stagePlayers[numOfStages-1][i] = stage.players[i].playersNum
    END FOR
    IF stage.numPlayers == 1 THEN
        PRINT "Winner: Player " + stage.players[0].playersNum
    END IF

    SET num_matches = stage.numPlayers / 2
    SET next_stage = stageInitializer(num_matches)

    FOR i FROM 0 TO num_matches - 1
        SET winnerNum 
        next_stage.players[i].playersNum = winnerNum
    END FOR
    CALL stageGames(next_stage, numOfStages + 1, allPlayers)
    CALL freeStage(next_stage)
    END FUNCTION


FUNCTION beatenPlayers(player: Player)
    SET numBeaten
    IF numBeaten == 0 THEN
        PRINT "No beaten players."
    ELSE
        FOR i FROM 0 TO numBeaten - 1
            PRINT "Player " + player.beaten[0][i]
        END FOR
    END IF
END FUNCTION


FUNCTION PlayersOfStages(numPlayers, stage):
    SET stageNum 
    indexOfStages = stage - 1
    IF (indexOfStages > stageNum) THEN
        PRINT "This stage is not available"
        RETURN
    FOR j = 0 TO numPlayers AND stagePlayers[indexOfStages][j] != 0
        PRINT Players 
    END FOR
END FUNCTION

FUNCTION winnerTree(stagePlayers):
    DEFINE space = 1, lastElement = 0, TAB_WIDTH = 2
    FOR j = 0 TO PLAYERS
        IF stagePlayers[0][j] == 0 THEN
            BREAK
        END IF
        PRINT stagePlayers[0][j]
        FOR s = 0 TO space - 1
            PRINT TAB_WIDTH spaces
        END FOR
    END FOR
    space *= 2
    FOR i = 1 TO STAGES
        IF stagePlayers[i][0] == 0 THEN
            BREAK
        END IF
        DEFINE TABS_BEFORE = space / 2, TABS_AFTER = space / 2
        FOR k = 0 TO TABS_BEFORE
            PRINT TAB_WIDTH spaces
        END FOR
        FOR j = 0 TO PLAYERS
            IF stagePlayers[i][j] == 0 THEN
                BREAK
            END IF
            PRINT stagePlayers[i][j]
            FOR s = 0 TO space - 1
                PRINT TAB_WIDTH spaces
            END FOR
            SET lastElement
        END FOR
        FOR k = 0 TO TABS_AFTER
            PRINT TAB_WIDTH spaces
        END FOR
        space *= 2
    END FOR
    PRINT winner
END FUNCTION


FUNCTION LostAgainst(playerNum, stage)
   FOR i = 0 to stage.numPlayers - 1 DO
        SET player
        FOR j = 0 to player.beatenNum[0] - 1 DO
            IF player.beaten[0][j] equals playerNum THEN
                print "the last match this player has been beaten in is with " + player.playersNum + ", and the player got lost in this match"
            END IF
        END IF
    END IF
END FUNCTION



FUNCTION main()
    DO:
        PRINT "Enter the number of players"
        INPUT numPlayers
    WHILE ((numPlayers & (numPlayers - 1)) != 0)
    DEFINE stage 
    DECLARE allPlayers
    FOR i FROM 0 TO numPlayers-1 DO:
        allPlayers[i] SET TO stage->players[i]
    ENDFOR
    CALL stageGames(stage, 1, allPlayers)
    DEFINE choice=0
    DO:
        PRINT "\nChoose one of the options:"
        PRINT "1. Display Players at Given Stage"
        PRINT "2. Check the Beaten Players"
        PRINT "3. Print Winner Tree"
        PRINT "4. Exit"
        INPUT choice
        SWITCH choice DO:
            CASE 1:
                PRINT "Enter the number of the stage to see its players:\n "
                DEFINE numOfStages
                INPUT numOfStages
                CALL PlayersOfStages(numPlayers, numOfStages)
            BREAK
            CASE 2:
                PRINT "Enter player number to see its beaten players:\n "
                DEFINE playersNum
                INPUT playersNum
                FOR j FROM 0 TO numPlayers-1 DO:
                    IF stage->players[j].playersNum == playersNum THEN:
                        player SET TO stage->players[j]
                        BREAK
                    ENDIF
                ENDFOR
                IF player != NULL THEN:
                    CALL beatenPlayers(player)
                ELSE:
                    PRINT "Player not found.\n"
                ENDIF
            BREAK
            CASE 3:
                CALL winnerTree(stagePlayers)
            BREAK
            CASE 4:
                PRINT "Exiting...\n"
            BREAK
            DEFAULT:
                PRINT "Wrong Input. Enter your choice again.\n"
        ENDSWITCH
    WHILE choice != 4
    CALL freeStage(stage)
    FREE allPlayers
    RETURN 0
END FUNCTION



*/