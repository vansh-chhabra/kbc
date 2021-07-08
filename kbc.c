#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//DEFINING A VERTICAL SPACE (REUSED MANY TIMES)
#define VERSPACE "\n\n\n\n"

//DEFINING COLORS
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define MAGENTA "\x1b[35m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[36m"
#define UNDER "\e[4m"

#define RESET_CLR "\x1b[0m"
#define RESET_UNDER "\e[24m"

//Function to implement press Enter to continue
void pressAnyKeyToContinue(){
    char ch = getchar();
    scanf("%c", &ch);
}

//For Bit Manipulation
void setBit(int * val, int bit_position) { * val = * val | (1 << bit_position); }
void clearBit(int * val, int bit_position) { * val = * val & ~(1 << bit_position); }
void toggleBit(int * val, int bit_position) { * val = * val ^ (1 << bit_position); }
int isSet(int val, int bit_position) { return (val & (1 << bit_position)); }

//A structure to hold the question, its options, its answer and the money associated when extracted from file
struct Question {
     char question[128], opt1[128], opt2[128], opt3[128], opt4[128], expert[128];
     char ans;
     int money;
 } question;

long long int mymoney=0;
int qnum=0;
int lifeline = 0;


/************************************************************************************************************************
 
Lifeline String

Lifeline	    | bit position
----------------------------
50/50	        |     0             (50/50 lifeline used if 1)
Quit Game       |     1             (Quit the game if 1)
Expert Advice   |     2             (Expert Advice lifeline used if 1)
Show Advice     |     3             (Show Expert Advice for this question if 1)
 
************************************************************************************************************************/


//FUNCTIONS USED
void welcome();
void beginKBC();
void instructions();
void moneyBoard();
void developers();
void wrongAnswer();
struct Question extractQuesFromFile();
void displayQuestion(struct Question ques);
void quitGame();
void crorepati();


//********************************************************************************************************************
// welcome() - The initial user dashboard for the game
//********************************************************************************************************************
void welcome() {
    
    //Clearing all lifelines if the user is playing again after quitting once
    clearBit(&lifeline, 0);
    clearBit(&lifeline, 1);
    clearBit(&lifeline, 2);
    clearBit(&lifeline, 3);
    
    //Setting question no. and money won to 0
    qnum = 0;
    mymoney=0;
    
    system("clear");
    int opt;
    printf(VERSPACE);
    printf(CYAN"\t\t Welcome To KAUN BANEGA CROREPATI! \t\t"RESET_CLR);
    printf(VERSPACE);
    printf(GREEN"\tPlease select one of the following options : \n\n"RESET_CLR);
    printf("\t\t1: Start the Game \n\t\t2: Display instructions \n\t\t3: Display the value of every question\n\t\t4. Display the developers of this KBC game\n\t\t5: Exit the Program\n\n");
    printf(MAGENTA"\t\tEnter your choice - "RESET_CLR);
    scanf("%d", &opt);
    switch(opt) {
      case 1:   beginKBC();
                break;
      case 2:   instructions();
                break;
      case 3:   moneyBoard();
                break;
      case 4:   developers();
                break;
      case 5:   exit(0);
                break;
      default:  printf(RED"\n\tWrong option selected, please enter 1/2/3/4/5. Press Enter to continue "RESET_CLR);
                pressAnyKeyToContinue();
                welcome();
                break;
    }
}


//********************************************************************************************************************
// beginKBC() - To start the game
//********************************************************************************************************************

void beginKBC() {
    system("clear");
    do {                // This loop will break when the player answers the question incorrectly
        struct Question question = extractQuesFromFile();
        char choice;
        start:          // Label used to display the question again, once a lifeline is chosen
            system("clear");
            printf(VERSPACE);
            displayQuestion(question);


        again:                                  // Label used to take input again if the player enters an invalid input
            scanf(" %c", &choice);
            choice = toupper(choice);           //So that the program accepts both uppercase and lowercase alphabets
            switch(choice) {
                case 'A': case 'B': case 'C': case 'D':
                if(choice == question.ans) {
                    mymoney=question.money;     // If player answer correctly, he wins the amount of money associated with question
                    clearBit(&lifeline, 3);     // Turning off expert's advice, clears the bit
                    printf("\n\n\t\tCongratulations, you have answered the question correctly");
                    printf("\n\n\t\tPress Enter to continue...");
                    pressAnyKeyToContinue();
                    continue;
                }
                else {  
                        printf("\n\n\t\tNo, the correct answer is %c", question.ans); // To display correct answer
                        printf("\n\n\t\tPress Enter to continue...");
                        pressAnyKeyToContinue();
                        wrongAnswer();
                        return;
                }
                break; 

            // Expert's advice
                case '2':
                if(isSet(lifeline, 2)) { // If player has already used this lifeline
                    printf(RED"\n\n\t\tYou have already used our Expert's advice, you cannot choose it again"RESET_CLR);
                    printf("\n\n\t\tYour choice: ");
                    goto again;
                }
                
                setBit(&lifeline, 2);
                setBit(&lifeline, 3);
                goto start;
                break;

            // Quit the game
                case 'Q':
                    quitGame();
                    break;

            // 50/50
                case '1':
                if(isSet(lifeline, 0)) { // If player has already used this lifeline
                    printf(RED"\n\n\t\tYou have already used 50/50 lifeline, you cannot choose it again"RESET_CLR);
                    printf("\n\n\t\tYour choice: ");
                    goto again;
                } 
                setBit(&lifeline, 0);
                srand(time(NULL));

            // To select 2 distinct and random options (no including the answer) to be removed from the screen
                int one, two;
                while((one = (rand() % 4) + 1) == (question.ans - 64));
                while((two = (rand() % 4) + 1) == one || two == (question.ans - 64));

            // Making randomly choosen option empty
                if (one == 1 || two == 1) question.opt1[0] = '\0';
                if (one == 2 || two == 2) question.opt2[0] = '\0';
                if (one == 3 || two == 3) question.opt3[0] = '\0';
                if (one == 4 || two == 4) question.opt4[0] = '\0';
                goto start;
                break;

                default:
                printf(RED"\n\n\t\tYou have entered an invalid choice, please enter A/B/C/D to select options or 1/2 for lifelines"RESET_CLR);
                printf("\n\n\t\tYour choice: ");
                goto again;
                break;
        }
    } while(1);
}


//********************************************************************************************************************
// instructions() - To display instructions of the game
//********************************************************************************************************************

void instructions() {
    system("clear");
    printf(VERSPACE);
    printf(UNDER"\tRead the following instructions carefully to understand the gameplay"RESET_UNDER);
    printf(VERSPACE);
    printf(YELLOW"\t1. There are 15 questions in all.\n");
    printf("\t2. Player will advance as he/she answers questions correctly.\n");
    printf("\t3. Press A/B/C/D to select an option, 1/2 for lifelines\n");
    printf("\t4. There are 2 lifelines available to the player: 50-50 and Expert Advice\n");
    printf("\t5. 50-50: 2 wrong options will disappear from the screen\n");
    printf("\t6. Expert Advice: Will display the advice of our expert on the question\n");
    printf("\t7. The lifelines can also be used only once in a game, even same time as well\n");    
    printf("\t8. There are two checkpoints, reaching them will guarantee the player atleast that amount of money\n");    
    printf("\t9. The checkpoints are at 5th and 10th question\n");
    printf("\t10. Answering any question wrong will set money amount won to previous checkpoint\n");
    printf("\t11. Player will always have an option to quit the game incase not so sure about answer\n");
    printf("\t12. If he/she quits, player will take home whatever money won till that level"RESET_CLR);
    printf(VERSPACE);
    printf(MAGENTA"\tPress Enter to return to main menu "RESET_CLR);
    pressAnyKeyToContinue();
    welcome();
}


//********************************************************************************************************************
// extractQuesFromFile() - Extracts question from a file, reads the data and stores in a global question variable.
//********************************************************************************************************************

struct Question extractQuesFromFile() {
    struct Question question;
    srand(time(NULL));
    qnum++;     //increment question number in every iteration (so no question is repeated)
    
    //create the path of the file
    char path[32]; 
    snprintf(path, 32, "questions/%d.txt", qnum);

    //opening and reading the file
    FILE *fp;
    fp = fopen (path,"r");
    if (fp!=NULL)
    {
        //storing the data into the struct object
        fgets(question.question, 128, fp);
        fgets(question.opt1, 128, fp);
        fgets(question.opt2, 128, fp);
        fgets(question.opt3, 128, fp);
        fgets(question.opt4, 128, fp);
        fgets(question.expert, 128, fp);
        fscanf(fp, "%c", &question.ans);
        fscanf(fp, "%d", &question.money);
        fclose(fp);
    }

    // Removing carriage return and new line from the extracted strings
    // ASCII CODE FOR CARRIAGE RETURN = 13
    // ASCII CODE FOR LINE FEED = 10
    for (int i = 0; i < 128; i++) {
        if(question.question[i] == 13 || question.question[i] == 10) question.question[i] = 0;
        if(question.opt4[i] == 13 || question.opt4[i] == 10) question.opt4[i] = 0;
        if(question.opt3[i] == 13 || question.opt3[i] == 10) question.opt3[i] = 0;
        if(question.opt2[i] == 13 || question.opt2[i] == 10) question.opt2[i] = 0;
        if(question.opt1[i] == 13 || question.opt1[i] == 10) question.opt1[i] = 0;
        if(question.expert[i] == 13 || question.expert[i] == 10) question.expert[i] = 0;
    }
    return question;
}

//********************************************************************************************************************
// displayQuestion() - To display the extracted question, its options and the lifelines.
// parameter - the struct object holding all the data
//********************************************************************************************************************
void displayQuestion(struct Question question) {
    if(qnum<=15) {
        printf(CYAN"\t\tCurrent amount won - Rs. %lld\n\n"RESET_CLR, mymoney*1000);
        printf("\t\tPresenting to you now, a question for Rs. %d - \n\n", question.money*1000);
        printf("\t\t%d. ", qnum);
        printf("%s",question.question);  //Print the question
        printf("\n\n\t\t\t");
        
        // To display the expert advice if opted for
        if(isSet(lifeline, 3)) {
            char expert[] = "";
            strcat(expert, question.expert);
            printf(YELLOW);
            printf("%s",expert);
            printf(RESET_CLR"\n\n\t\t\t");
        }
        
        // To display the 4 options corresponding to each question
        printf("%s",question.opt1);
        printf("\t\t\t");
        printf("%s",question.opt2);
        printf("\n\n\t\t\t");
        printf("%s",question.opt3);
        printf("\t\t\t");
        printf("%s",question.opt4);
        printf(VERSPACE);

        // To display the available lifelines to the player
        printf("\t\tAVAILABLE LIFELINES - \t");
        char lifelineStr[128] = "";
        char quitStr[128] = "";
        strcat(lifelineStr, ((isSet(lifeline, 0)) ? RED"1. 50/50         "RESET_CLR : GREEN"1. 50/50         "RESET_CLR));
        strcat(lifelineStr, ((isSet(lifeline, 2)) ? RED"2. Expert Opinion"RESET_CLR : GREEN"2. Expert Opinion"RESET_CLR));
        strcat(quitStr, ((isSet(lifeline, 1)) ? RED"q. Quit Game"RESET_CLR : GREEN"q. Quit Game"RESET_CLR));
        printf("%s",lifelineStr);
        printf("\n\n\t\tIF YOU WANT TO QUIT - \t");
        printf("%s",quitStr);
        printf("\n\n\t\tYour choice: ");
    } else crorepati(); // Function to be executed if the player has answered all the questions correctly
}


//********************************************************************************************************************
// wrongAnswer() - If the player answers any question wrongly
//********************************************************************************************************************
void wrongAnswer() {
        system("clear");
        printf(VERSPACE);
        printf("\t\tYou answered the question wrongly");
        
        // The checkpoint logic
        
        // If the player has not yet reached first checkpoint, the he'll win Rs. 0
        if(mymoney>0 && mymoney<10) mymoney=0;  
        // If the player has reached the first checkpoint, but not the 2nd one, he'll win money equal to 1st checkpoint
        else if(mymoney>10 && mymoney<320) mymoney=10;
        // If the player has reached the second checkpoint, but not 1 crore, he'll win money equal to 2nd checkpoint
        else if(mymoney>320 && mymoney<10000) mymoney=320;
        
        printf(VERSPACE);
        printf("\t\tYou won Rs. %lld \n\n", mymoney*1000);

        printf("\t\tThanks for playing KBC with us.....");
        printf(VERSPACE);
        printf(MAGENTA"\n\n\tPress Enter to return to main menu...."RESET_CLR);
        pressAnyKeyToContinue();
        printf(VERSPACE);
        
        welcome();
}

//********************************************************************************************************************
// quitGame() - If the player wants to quit the game, in which case he'll win the money he currently has irrespective 
//              of the checkpoints
//********************************************************************************************************************

void quitGame() {
        system("clear");

        printf(VERSPACE);
        printf("\tSo you have decided to quit game.");
        printf(VERSPACE);
        printf("\t\tCongratulations, you won Rs. %lld\n\n", mymoney*1000);
        printf(MAGENTA"\n\n\tPress Enter to return to main menu...."RESET_CLR);
        pressAnyKeyToContinue();
        printf(VERSPACE);

        welcome();
}

//********************************************************************************************************************
// moneyBoard() - To display the money which can be won by answering each question correctly and the checkpoints
//********************************************************************************************************************

void moneyBoard(){
    system("clear");
    printf("\n\n\t\t      QUESTION\t\t\t     MONEY");
    printf("\n\t\t      --------------------------------------------");
    printf(YELLOW"\n\t\t\t15. \t\t\tRs. 1,00,00,000"RESET_CLR);
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t14. \t\t\tRs. 50,00,000");
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t13. \t\t\tRs. 25,00,000");
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t12. \t\t\tRs. 12,50,000");
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t11. \t\t\tRs. 6,40,000");
    printf("\n\t\t      --------------------------------------------");
    printf(YELLOW"\n\t\t\t10. \t\t\tRs. 3,20,000"RESET_CLR);
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t 9. \t\t\tRs. 1,60,000");
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t 8. \t\t\tRs. 80,000");
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t 7. \t\t\tRs. 40,000");
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t 6. \t\t\tRs. 20,000");
    printf("\n\t\t      --------------------------------------------");
    printf(YELLOW"\n\t\t\t 5. \t\t\tRs. 10,000"RESET_CLR);
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t 4. \t\t\tRs. 5,000");
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t 3. \t\t\tRs. 3,000");
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t 2. \t\t\tRs. 2,000");
    printf("\n\t\t      --------------------------------------------");
    printf("\n\t\t\t 1. \t\t\tRs. 1,000");
    printf("\n\t\t      --------------------------------------------");
    printf(MAGENTA"\n\n\tPress Enter return to main menu "RESET_CLR);
    pressAnyKeyToContinue();
    welcome();
}

//********************************************************************************************************************
// developers() - To display the developers section of the project
//********************************************************************************************************************

void developers() {
    system("clear");
    printf(UNDER"\n\t\t\tDEVELOPERS SECTION\n\n"RESET_UNDER);
    printf("\tSECTION NO. 7\t\tINSTRUCTOR: Ms. Divya Bharadwaj\n\n");
    printf("\tThis KBC game has been developed by following\n");
    printf("\n\t\t1. Dhruv Rawat, 2019B3PS0537P");
    printf("\n\t\t2. Shubham Sharma, 2019B2PS0925P");
    printf("\n\t\t3. Vansh Chhabra, 2019B1PS1039P");
    printf(MAGENTA"\n\n\tPress Enter return to main menu "RESET_CLR);
    pressAnyKeyToContinue();
    welcome();
}

//********************************************************************************************************************
// crorepati() - To be executed if the player answers all the questions correctly and becomes a crorepati
//********************************************************************************************************************

void crorepati() {
    printf(GREEN"\n\n\t\t\tCONGRATULATIONS! YOU HAVE BECOME A CROREPATI!\n\n\n");
    printf("\tYou Have A total Of Rs. 1 Crore in your pool.\n\n");
    printf("\tThank you for playing our game"RESET_CLR);
    printf(MAGENTA"\n\n\tPress Enter return to main menu "RESET_CLR);
    pressAnyKeyToContinue();
    welcome();
}

//********************************************************************************************************************
// MAIN FUNCTION
//********************************************************************************************************************
int main() { 
    welcome();
    return 0;
}
