//Header Files
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>
#define MAX 100
#define password "pass1234"

//sREC is for student record
typedef struct Srec{
char id[31];
char pin[5];
char name[31];
} sREC;

//cRec is for candidate record
typedef struct Crec{
char name[31];
char position[31];
int vote;
}cREC;

typedef struct list{
sREC voter[MAX];
cREC candidate[MAX];
int Clast;
int Slast;
} LIST;

LIST l;

//Function Declarations
void delay();   //for loading print
void gotoxy(int x,int y); //for cleaner display
void rectangle(int x1,int y1,int x2,int y2); //for border
void bye(); //exit code
void makenull(); //empties our list
int SisFull();  //checks if our student list is full
int SisEmpty(); //checks if our student list is empty
int CisFull();  //checks if our candidate list is full
int CisEmpty(); //checks if our candidate list is empty
void insertCard(); //detects our card if it matches with our record
void initialize(); //menu
void regMenu(); //menu for registration
void Sregister(); //registration process for student or voters
void SsaveUSB(sREC sr); //saves the student record to flash drive
void Ssave();   //save the student record to a dbf file
void insertS(sREC sr); //inserting student record to the list
int Spos(char namepos[31]); //function for sorting
int Cregister(); //registration process for candidates
void insertC(cREC cr); //inserting the candidate record to the list
void Csave(); //save the candidate record to a dbf file
int Slocate(char nameloc[31]); //locates the student record
int Clocate(char nameloc[31]); //locates the candidate record
int Cpos(char posPos[31]); //function for sorting
void viewCandidates(); //function to view the elected candidates
int ClocatePrint(char posloc[31]); //function to locate name existing in the list
void Sretrieve(); //stores the content of our student record
void Cretrieve(); //stores the content of our candidate record
void login(); //validation to see if the student is registered to vote
int castVote(); //for vote casting
int Slocateid(char idloc[31]); //function for locating the student or voters id
void votesys(); //menu for voting system
int tallyprint(char posloc[31]); //print the tally
void tally(); //tally function to see the leading candidates


//Main program
int main(){
FILE *fpDriveChk; //these are the local variables
int index=0;
char pw[9], p;
int valid, retry=1;
makenull(); //calls this function to empty our list
Sretrieve(); //calls this function to store our student record
Cretrieve(); //calls this function to store our candidate record

fpDriveChk=fopen("E:\\voter.txt", "a"); //opens file in append mode so it returns null if it can't be opened
fclose(fpDriveChk); //closes our file

    do{ //the user cannot enter the program unless there's a flashdrive detected
    system("cls"); system("COLOR 0F"); //clears the screen and changes the color in console
    rectangle(15,6,79,20); //for border
    gotoxy(16,5); //places our cursor here for printing
    printf("Please insert your card"); delay(); //ask the user to input their card so they can enter the program
    fpDriveChk=fopen("E:\\voter.txt", "a"); //opens the file voter.txt in the USB
    fclose(fpDriveChk); //closes the file

    }

    while(fpDriveChk=fopen("E:\\voter.txt", "r")==NULL);{ //while the file voter.txt in the USB is null, the user can't enter
    fclose(fpDriveChk);} //closes the file

    do{
    system("cls"); //clears the screen
    gotoxy(16,5); system("COLOR 06"); //places our cursor and changes the color of our console
    printf("<CARD DETECTED SUCCESSFULLY!>"); //tells the user that the card was detected and can now proceed
    rectangle(15,6,79,20); //for border
    gotoxy(20,8); //places the cursor for printing
    printf("Enter 8-character password: "); //asks the user for the program password
    while((p =_getch())!= 13){ //so that the user can use backspace when entering the masked password and hit enter after inputting
        if(index<0)
        index=0; //set our index to 0
        if(p == 8){ //8 is the ASCII value of backspace char and 13 is for enter key
        putch('\b'); //for the backspace function
        putch(' '); //for the backspace function
        putch('\b'); //for the backspace function
        index--; //decrement our index so it would move back our cursor and delete the last '*' from screen
        continue;} //proceed
       pw[index++] = p;
       putch('*');}

    if(strcmp(pw,password)!=0){ //compares our string if they are matching
    gotoxy(18,14); system("COLOR 04"); //places our cursor for printing and changes the color of console
    printf("<ERROR: Incorrect password.>"); //tells the user that they entered the wrong password
    gotoxy(18,15); //places our cursor for printing
    printf("WARNING! You only have have %d trial remaining.", 3-retry); //tells the remaining attempts to be made
    gotoxy(18,18); //places our cursor for printing
    system("pause"); //pauses our console
    valid=0; //set valid to 0
    retry+=1;} //increment our retry
    else{valid=1;}} //tells that they can now enter the menu
    while(!valid&&retry<=3); //while not valid and the user did not reach the 3 attempts the statement above would be executed
        if(retry>3) //if the user reached 3 attempts
        {   gotoxy(16,21); system("COLOR 04"); //places our cursor and changes the color of console
            printf("<You already have exceeded the number of attempts. Please try again later.>\n\n"); } //tells that the user can't make more attempts
        else
        {   gotoxy(18,17); system("COLOR 02"); //places our cursor and changes the color of console
            printf("<ACCESS APPROVED!>\n\n"); //tells that they can now access the menu
            gotoxy(18,18); //places the cursor here for printing
            printf("Redirecting you to the menu"); delay(); //loading screen telling that they are being redirected to the menu
            initialize(); //calls the initialize function
        }
    gotoxy(16,22); system("pause"); bye(); //if reached the max number of attempts, we'll call the exit function
}



void delay() //for loading print
{
    for(int i=0;i<3;i++) //loop
    {   usleep(700000); //for loading or pausing the screen
        printf("."); } //prints ...
        usleep(700000); //for loading or pausing the screen
}


void gotoxy(int x,int y) //for cleaner display
{
    COORD coord; //this is for placing our cursor
    coord.X=x;   //for cleaner display
    coord.Y=y;   //so we won't have to use tab all the time
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}


void rectangle(int x1,int y1,int x2,int y2) //for the rectangular border
{
    //for the corners
    gotoxy(x1,y1); printf("%c",201);
    gotoxy(x1,y2); printf("%c",200);
    gotoxy(x2,y1); printf("%c",187);
    gotoxy(x2,y2); printf("%c",188);

    //for the top and bottom edge
    for(int i=x1+1;i<=x2-1;i++){
    gotoxy(i,y1); printf("%c",205);
    gotoxy(i,y2); printf("%c",205);}

    //for the left and right edge
    for(int i=y1+1;i<=y2-1;i++){
    gotoxy(x1,i); printf("%c",186);
    gotoxy(x2,i); printf("%c",186);
    }
}


void bye() //function for the exit
{
    system("cls"); system("COLOR 0B"); //places our cursor and changes the color of console
    rectangle(15,6,79,20); //for border
    gotoxy(17,8); //places our cursor
    printf("\t  TECHNOLOGICAL UNIVERSITY OF THE PHILIPPINES"); //print in the screen
    gotoxy(17,11); //places our cursor
    printf("\t THANK YOU FOR USING THE STUDENT ELECTION S.Y 2021!"); //prints in the console
    gotoxy(17,13); //places our cursor
    printf("\t\t\t  BSCS-2AB"); //print in the screen
    gotoxy(17,16); //places our cursor
    printf("\t\t  Programmed & Designed by:"); //prints in the console
    gotoxy(17,17); //places our cursor
    printf("\n\t\t\t\t  RASHEED RIGA | MIKA GAJETO"); //prints the name of the programmers
    gotoxy(50,26); //places our cursor here
    exit(0); //exit the program
}


void makenull() //makenull function that empty our list
{    l.Clast = -1;
     l.Slast = -1;
} //makes or set our index to -1


int SisFull() //function to determine if the list is already full
{   return (l.Slast == MAX-1);}


int SisEmpty() //function to determine if the list is empty
{   return (l.Slast == -1);}


int CisFull() //function to determine if the list is already full
{   return (l.Clast == MAX-1);}


int CisEmpty() //function to determine if the list is empty
{   return (l.Clast == -1);}


void insertCard() //detects our card if it matches with our record
{
    sREC sr; //our local variable
    FILE *fpDriveChk;
    int p,j,x;

    do{
    gotoxy(5,5); system("COLOR 06"); //places our cursor and changes the color of console
    printf("Analyzing your card"); delay(); ;} //tells the user we're scanning their card

    while(fpDriveChk=fopen("E:\\voter.txt", "r")==NULL); //opens the voter.txt file in the USB
    fclose(fpDriveChk); //closes our file


    fpDriveChk=fopen("E:\\voter.txt", "r"); //opens our file for reading
    fscanf(fpDriveChk,"%s %s %[^\t] %d\n",sr.id,sr.pin,sr.name,&x); //scans our file
    fclose(fpDriveChk); //closes our file
    if (x!=0){ //to detect if the voter had entered the voting cast system
        gotoxy(8,12); system("COLOR 0C"); //places our cursor and changes the color of console
        printf("<You have already voted.>\n"); //tells the user that they voted already
        gotoxy(8,13); printf("You cannot vote anymore."); //if they entered the vote casting, they cannot vote anymore
        gotoxy(8,18); system("pause"); bye(); exit(0);} //calls the exit function
    for(int i=0;i<=l.Slast;i++){ //loop
        if (strcmp(sr.id,l.voter[i].id)==0) //compares these two string if they are the same
        {	if (strcasecmp(sr.name,l.voter[i].name)==0) //compares the name if they are the same
            {	p=Slocate(sr.name); //we then locate the name
            	j=Slocateid(sr.id); //we then locate the if
            		if (p==j) //checks if they matched
                    {	gotoxy(8,12); system("COLOR 02"); //places our cursor and changes the color of console
                        printf("<SUCCESS:ID successfully matched.>\n"); //tells that we verified that it matched
                        gotoxy(8,13); //place out cursor
                        printf("You can now proceed to vote"); //it matched so now the student can proceed to the voting phase
                        fpDriveChk=fopen("E:\\voter.txt", "w"); //open our voter.txt file into writing mode
                        fprintf(fpDriveChk,"%s\t%s\t%s\t%d\n",sr.id,sr.pin,sr.name,x+1); //we then print the student records in the voter.txt file
                        fclose(fpDriveChk); //closes our file
                        delay(); system("cls"); //loads the screen and clears it
                     	login();	} //calls the login function so they can vote now
            		else {	gotoxy(12,12); system("COLOR 04"); printf("<ERROR:not found>");	} //tells that we cannot find their record in our system
            }
        }
    continue; //proceed
    }

    initialize(); //calls our menu function
}


void initialize() //menu
{
    int op; //local variable

    while (1){ //while running
    system ("cls"); system("COLOR 07"); //places our cursor and changes the color of console
    rectangle(15,6,79,20); //for border
    gotoxy(42,8); //places our cursor
    printf (" M E N U "); //prints in console
    gotoxy(37,11); //places our cursor
    printf("1.] REGISTRATION"); //prints
    gotoxy(37,13); //places our cursor
    printf("2.] VOTING SYSTEM"); //prints in console
    gotoxy(37,15); //places our cursor
    printf("3.] EXIT"); //prints in console
    gotoxy(18,18); //places our cursor
    printf("Enter choice: "); //asks the user to enter their choice
    scanf("%d",&op); //scans their choice

    switch(op)
        {   case 1: //if the user choose 1
            system("cls"); //clears the screen
         	system("COLOR 07"); //changes the color in console
        	regMenu(); //calls the registration menu function
            break; //terminate the loop

            case 2: //if the user choose 2
            system("cls"); //clears the screen
         	system("COLOR 07"); //changes the color in console
        	votesys(); //calls the voting system function
            break; //terminate the loop

            case 3: //if the user choose 3
            {char ans; //local variable

            gotoxy(18,21); system("COLOR 06"); //places the cursor and changes the color of the console
            printf("Are you sure you want to exit?\n"); //asks the user if they really want to exit
            gotoxy(18,22); //places the cursor
            printf("Enter choice [y/n]: "); //ask their answer
            scanf("%s", &ans); //scans their answer
            if(ans=='y' || ans =='Y'){ //if their answer is y for yes
            gotoxy(17,4); system("COLOR 04"); //places the cursor and changes the color of the console
            printf("Thank you for using our program!\n"); //prints
            bye();} //calls the exit function
            else { //if their answer is n for no
            gotoxy(18,24); system("COLOR 02"); //places the cursor and changes the color of the console
            printf("Please wait"); //prints
            delay(); //loading function
            initialize();} //redirect them back to the menu
            break;} //terminate the loop

            default: //if they entered an invalid number or choice
            gotoxy(18,21); system("COLOR 04"); //places the cursor and changes the color of the console
            printf("Select 1-3 only"); //ask them to enter from 1-3 only
            gotoxy(18,22); //places the curser
            system("pause"); //pauses the console
        }
    }
}



void regMenu() //menu for registration
{
    int option; //local variable

    while (1){
    system ("cls"); //clears the screen
    rectangle(15,6,79,20); //for border
    gotoxy(34,8); //places the cursor
    printf (" R E G I S T R A T I O N"); //prints
    gotoxy(18,11); //places the cursor
    printf("1.] Register a Voter"); //prints in the console
    gotoxy(18,12); //places the cursor
    printf("2.] Register a Candidate"); //prints in the console
    gotoxy(18,13); //places the cursor
    printf("3.] Exit Registration"); //prints
    gotoxy(18,18); //places the cursor
    printf("Enter choice: "); //ask the user to enter their choice
    scanf("%d",&option); //scans their choice

    switch(option) //switch case
        {   case 1: //if the user choose 1
            system("cls"); system("COLOR 07");  //places the cursor and changes the color of the console
        	rectangle(15,6,79,20); //for border
        	gotoxy(18,8); //places the cursor
        	printf("REGISTER A VOTER"); //prints in the console
        	Sregister(); //calls the student registration function
        	Ssave(); //calls the save function for student records
        	initialize(); //calls the menu again
            break; //terminate loop

            case 2: //if the user choose 2
            system("cls"); system("COLOR 07");  //places the cursor and changes the color of the console
        	Cregister(); //calls the candidate register function
        	gotoxy(17,21); //places our cursor
        	printf("CANDIDATE RECORD ADDED!"); //tells that the candidate record was added
        	gotoxy(17,22); //places our cursor
        	printf("Redirecting you to the menu"); delay(); //tells that we are redirecting the user back to the menu
        	Csave(); //calls the save function for candidate record
        	initialize(); //calls the menu again
            break; //terminate loop

            case 3: //if the user choose 3
            {char ans; //local variable

            gotoxy(18,21); system("COLOR 06"); //places cursor and changes the color of the console
            printf("Are you sure you want to exit?\n"); //asks the user if they really want to exit
            gotoxy(18,22); //places the cursor
            printf("Enter choice [y/n]: "); //ask their answer
            scanf("%s", &ans); //scans their answer
            if(ans=='y' || ans=='Y'){ //if their answer is y for yes
            gotoxy(17,4); system("COLOR 04"); //places the cursor and changes the color of the console
            printf("Thank you for using our program!\n"); //prints
            bye();} //calls the exit function
            else {
            gotoxy(18,24); system("COLOR 02"); //places the cursor and changes the color of the console
            printf("Please wait"); //prints
            delay(); //loading function
            initialize();} //redirect back to the menu
            break;} //terminate loop

            default: //if they entered an invalid number or choice
            gotoxy(18,21); system("COLOR 04"); //places the cursor and changes the color of the console
            printf("Select 1-4 only"); //ask them to enter from 1-4 only
            gotoxy(18,22); //places the curser
            system("pause");}  //pauses the console
}
}

void Sregister() //registration process for student or voters
{
    sREC vr;

    gotoxy(18,11);system("COLOR 07");  //places the cursor and changes the color of the console
    printf("Enter your Voter's ID: "); //asks the user to enter their voters id
    scanf("%s",&vr.id); //scans data
    gotoxy(18,12);// places the cursor
    printf("Enter your Pin code: "); //asks the user to enter their pin code
    scanf("%s",&vr.pin); //scans data
    gotoxy(18,13); //places the cursor
    printf("Enter your Name: "); //asks the user to enter their name
    scanf(" %[^\n]s",vr.name); //scans the name
    gotoxy(18,18); system("COLOR 02");  //places the cursor and changes the color of the console
    printf("Analyzing your data"); //print sin the console
    delay(); //loading function
    insertS(vr); //calls the insert student/voter record function
    SsaveUSB(vr); //calls the save USB function for the student record

}

void SsaveUSB(sREC sr) //saves the student record to flash drive
{
    FILE *Sfp1; //local variables
    int i=0;

    Sfp1 = fopen("E:\\voter.txt","w"); //opens file in write mode
    if (Sfp1 == NULL){ //if the file doesn't contain anything
    printf("<ERROR:File Not Found.>\n"); //display that the file is not found
    system ("pause");} //pauses the console

    else{
    fprintf(Sfp1,"%s\t\t %s \t\t %s\t\t\t %d\n",sr.id,sr.pin,sr.name,i); //prints student record in the voter.txt file
    fclose(Sfp1);//closes our file
    }
}

void Ssave() //save the student record to a dbf file
{
    FILE *Sfp1; //local variable
    int i;

    gotoxy(17,21); //places our cursor
    printf("VOTER RECORD ADDED!"); //print in console
    gotoxy(17,22); // //places the cursor
    printf("Redirecting you to the menu"); delay(); //print and loading function
    Sfp1 = fopen("voterList.dbf","w+"); //opens file in write mode
    if (Sfp1 == NULL){ //if the file doesn't contain anything
    printf("<ERROR:File Not Found.>\n"); //display that the file is not found
    system ("pause");} //pauses the console

    else {
    for(i=0; i<=l.Slast; i++) //loop
    {   fprintf(Sfp1,"%s\t\t %s\t\t %s\n",l.voter[i].id, l.voter[i].pin, l.voter[i].name);} //print student record in our voterList.dbf file
        }
    fclose(Sfp1);
}


void insertS(sREC sr) //inserting student record to the list
{
    int i, p; //local variables

	if(SisFull()) //if student record is full
    {  system("cls"); system("COLOR 04");  //places the cursor and changes the color of the console
       printf("List is full."); //display in console
       getch();} //pauses the console until a key is pressed
    else
    {   if(Slocate(sr.name)>=0) //checks the name if there are duplicate record
        {   gotoxy(18,21); system("COLOR 04");  //places the cursor and changes the color of the console
            printf("<ERROR: Duplicate Record Found>"); //displays in the console
            gotoxy(18,22); //places our cursor
            printf("%s is already in the list. Try another name!",sr.name); //states the duplicate name and ask the user to input another
        }

		else
		{   p=Spos(sr.name); system("COLOR 07"); //changes the color of console and checks the position of name for sorting
			for(i=l.Slast; i>=p; i--) //move elements
			{   l.voter[i+1]=l.voter[i];
            }
            strcpy(l.voter[p].id, sr.id); //copies the id
            strcpy(l.voter[p].pin, sr.pin); //copies the pin
			strcpy(l.voter[p].name, sr.name); //copies the name
		    printf("\n"); //print ne line
		    l.Slast++; //incrementation
		}

    }
}


int Spos(char namepos[31])  //function to find the position of element on the list
{
	int i; //local variable

	for(i=0;i<=l.Slast;i++) //loop
	{   if(strcmp(l.voter[i].name,namepos)>0) //compares the two strings if they are the same
		{   return(i);} //if same it will return index
	}
	return(i);
}


int Cregister() //registration process for candidates
{
    cREC cr; //local variable

    rectangle(15,6,79,20); //for border
    gotoxy(18,8); //places cursor
    printf("REGISTER A CANDIDATE"); //prints in console
    gotoxy(18,10); //places cursor
    printf("Input name of the candidate: "); //ask the user to input the candidate they want to eleect
    scanf(" %[^\n]s",cr.name); //scans the name
    gotoxy(18,12); //places cursor
    printf("Positions Available: "); //prints the position available
    gotoxy(18,13); //places cursor
    printf("PRESIDENT"); //prints
    gotoxy(18,14); //places cursor
    printf("VICE PRESIDENT"); //prints
    gotoxy(18,15); //places curson
    printf("SECRETARY"); //prints
    gotoxy(18,16); //places cursor
    printf("TREASURER"); //prints
    gotoxy(18,18); // places cursor
    printf("Input position: "); //asks the user the position of the candidate they want to elect
    scanf(" %[^\n]s",cr.position); //scans
    cr.vote=0;
    insertC(cr); //calls the insert candidate function
}


void insertC(cREC cr) //inserting the candidate record to the list
{
    int p,j; //lcoal variable

	if(CisFull()) //checks if C is full
    {   printf("List is full."); //prints
        getch(); } //pauses the console until a key is pressed

    else
    { if(Clocate(cr.name)>=0) //checks the name if there are duplicate record
        {   gotoxy(18,21); //places the cursor
            printf("<ERROR: Duplicate Record Found>"); //tells that there are duplicate record found
            gotoxy(18,22); //places cursor
        	printf("\n%s is already in the list. Try another name!\n",cr.name);} //tells the name that is already on the list and asks the user to input another

		else
		{   p = Cpos(cr.position); //checks the position of name for sorting
			for(int i=l.Clast; i>=p; i--) //move elements
			{   l.candidate[i+1] = l.candidate[i];
            }

			strcpy(l.candidate[p].name, cr.name); //copies the name
			strcpy(l.candidate[p].position, cr.position); //copies the postion
		    printf("\n"); //prints new line
		    l.Clast++; //incrementation
		}

    }
}


void Csave() //save the candidate record to a dbf file
{
    FILE *Cfp1; //local variables
    int i;

    Cfp1 = fopen("candidateList.dbf","w+"); //open file in write mode
    if (Cfp1 == NULL){ //if the file doesn't contain anything
    printf("<ERROR:File Not Found.>\n"); //display that the file is not found
    system ("pause");} //pauses the console
    else {
    for (i=0; i<=l.Clast; i++) //loop
    {   fprintf(Cfp1,"%s\t\t %s\t\t %d\n",l.candidate[i].name,l.candidate[i].position,l.candidate[i].vote);} //print the candidate record in our candidateList.dbf file
    fclose(Cfp1); } //closes out file
}

int Slocate(char nameloc[31]) //locates the student record
{
    int i; //local variable

    for(i=0; i<=l.Slast; i++) //loop from first index to last containing element
    {   if((strcasecmp(l.voter[i].name, nameloc)==0)) //compares two string if the same
        {   return(i);} //if same, it will return index
    }
    return(-1); //if not, it will return -1
}


int Clocate(char nameloc[31]) //locates the candidate record
{
    int i; //local variable

    for(i=0; i<=l.Clast; i++) //loop from first index to last containing element
    {   if((strcasecmp(l.candidate[i].name, nameloc)==0)) //compares two string if the same
        {    return(i);} //if same, it will return index
    }
    return(-1); //if not, it will return -1
}


int Cpos(char posPos[31])  //function to find the position of element on the list
{
	int i; //local variable

	for(i=0; i<=l.Clast; i++) //loop
	{   if(strcmp(l.candidate[i].position, posPos)>0) //compares two string
		{   return(i);} //if same it will return index
	}
	return(i); //return index;
}


void viewCandidates() //function to view the elected candidates
{
    cREC cr; // local variable for the array of structures
    char temp[MAX][31];  //a temporary variable for sorting
    char cast[31]; //string variable for the answer
    int p,count=0,j,i; //integer for loops and counters

    for(p=0; p<=l.Clast; p++) //a loop that sorts all the position
    {   if (strcasecmp(l.candidate[p].position, l.candidate[p+1].position)==0) //compares a string to a string that is next to his index
        {continue;} //procees

        else if (strcasecmp(l.candidate[p].position, l.candidate[p+1].position)!=0) //compares if the strings are  different
        {   strcpy(temp[p],l.candidate[p].position); //it then copies the string to temp
            printf("\n\n\tRUNNING FOR: "); //prints
            printf("\t%s\n",temp[p]);  // prints a string in temp variable
            count++;} //increment counter

    ClocatePrint(temp[p]); //a function that prints all the character with the same position
    }
    printf("\n\n\t"); system("pause"); //print new line and pauses the console
}


int ClocatePrint(char posloc[31]) //function to locate name existing in the list
{
    int i; //local variable

    for(i=0; i<=l.Clast; i++) //loops
    {   if((strcasecmp(l.candidate[i].position, posloc)==0)) //compares the position in the array with the passed string
        {   printf("\tNAME: %s\n",l.candidate[i].name);} //prints the name with the particular position
    }
}


void Sretrieve() //stores the content of our student record
{
    sREC sr; //local variable
    FILE *Sfp1;

    if((Sfp1=fopen("voterList.dbf","r+"))!=NULL) //opens file with condition
    {   while (!feof(Sfp1)){ //loops until the end of file
        fscanf(Sfp1,"%s %s %[^\t\n]",sr.id,sr.pin,sr.name); //scans the file
        insertS(sr);} //passes the variable then sends to insert function
    }
    fclose(Sfp1); //closes file
}


void Cretrieve() //stores the content of our candidate record
{
    cREC cr; //local variable
    FILE *Cfp1;

    if((Cfp1=fopen("candidateList.dbf","r+"))!=NULL)  //opens file with condition
    {   while (!feof(Cfp1)){ //reads file until end
        fscanf(Cfp1,"%[^\t] %[^\t]  %d\n",cr.name,cr.position,&cr.vote); //scans the content of file then is saved to memory
        insertC(cr);} //sends the scanned variables to insertc function
    }
    fclose(Cfp1); // closes the file
}


void login() //validation to see if the student is registered to vote
{
    sREC sr; //local variables
    int j;

    system("COLOR 07"); //changes color in console
    rectangle(4,6,69,50); //for border
    gotoxy(8,8); //places cursor
    printf("CASTING YOUR VOTE\n\n"); //prints in console
    gotoxy(8,11); //places our cursor
    printf ("Enter Voter's ID: "); //ask the user to enter their ID
    scanf("%s",sr.id); //scans their ID
    j=Slocateid(sr.id); //saves the value that will be returned by slocate

    if (strcasecmp(sr.id,l.voter[j].id)!=0) //compares strings
        {   gotoxy(8,13); system("COLOR 04"); //places cursor and changes the color of the console
            printf("ERROR:<ID not found>"); //tells the user that the ID was not found
            getch();    } //pauses the console until a key is pressed

    else
        {   gotoxy(8,12); //places the cursor
            printf("Enter pin: "); //ask the user to enter pin
            scanf("%s",sr.pin); //scans the entered pin
                if (strcmp(l.voter[j].pin,sr.pin)==0) //compares strings
                {   castVote(); } //calls the vote casting function
                else
                {   gotoxy(12,14); system("COLOR 04"); //places cursor and changes the color of the console
                    printf ("<ERROR: Wrong Pin>\n"); } //tells the user that they entered the wrong pin
        }
}


int castVote() //for vote casting
{
    cREC cr;  //local variable for the array of struct
    char temp[MAX][31]; //temporary variable for sorting
    char cast[31]; //string memory for input
    char temp2[MAX][31]; //temporary variable for sorting
    int p,count=0,j=0; //loops and counter integer declaration

    system("COLOR 07"); //changes color of console
    printf("\n\n\tCANDIDATES TO VOTE"); //prints in console
    for(p=0; p<=l.Clast;p++)
    {   if (strcasecmp(l.candidate[p].position,l.candidate[p+1].position)==0) //compares strings if same
        {   continue;} //proceed

        else if (strcasecmp(l.candidate[p].position,l.candidate[p+1].position)!=0) //compares strings
        {   strcpy(temp[p],l.candidate[p].position); //saves to temp when not same
            strcpy(temp2[j],l.candidate[p].position); //also saves to temp 2 but starting at 0th index
            printf("\n\n\t%s: ",temp[p]); printf("\n"); //prints in console and prints new line
            count++; //increment counter
            j++; //increment counter
        }

    ClocatePrint(temp[p]);  //function to locate name existing in the list
    }

    for(int i=0;i<count;i++)
    {   printf("\n\tInput the name of the candidate you want to vote as\n"); //asks the user to input the candidate they want to elect
        printf("\t%s: ",temp2[i]); //shows the position of the candidate
        scanf(" %[^\n]s",cast); printf("\n"); //scans the vote casted
        j=Clocate(cast); //saves the integer returned by clocate to j
        l.candidate[j].vote = l.candidate[j].vote+1;}  //increments the vote
        Csave();
}


int Slocateid(char idloc[31]) //function for locating the student or voters id
{
    int i; //local variable

    for(i=0; i<=l.Slast; i++) //loop
    {   if((strcasecmp(l.voter[i].id, idloc)==0)) //compares strings if same
        {   return(i);} //if same it will return index
    }
    return(-1); //if they are not the same it will return -1
}


void votesys() //menu for voting system
{  int option; //local variable

   while (1){ //while running
    system("cls"); system("COLOR 07"); //places cursor and changes the color of the console
    rectangle(15,6,79,20); //for border
    gotoxy(34,8); //places cursor
    printf (" V O T I N G   S Y S T E M \n\n"); //prints in the console
    gotoxy(18,11); //places cursor
    printf("1.] Cast a vote \n"); //prints in the console
    gotoxy(18,12); //places cursor
    printf("2.] Tally of votes\n"); //prints in the console
    gotoxy(18,13); //places cursor
    printf("3.] View Candidates"); //prints in the console
    gotoxy(18,14); //places cursor
    printf("4.] Remove Card/ EXIT"); //prints in the console
    gotoxy(18,18); //places cursor
    printf("Enter choice: "); //asks the user to enter choice
    scanf("%d",&option); //scans their choice

    switch(option) //switch case
        {   case 1: //if the user choose 1
            system("cls"); system("COLOR 07");  //places cursor and changes the color of the console
            rectangle(4,6,69,20); //for border
            gotoxy(8,8); //places cursor
            printf("CASTING YOUR VOTE"); //prints
            insertCard(); //calls insertCard funtion
            break; //terminate loop

            case 2: //if the user choose 2
            system("cls"); system("COLOR 07");  //places cursor and changes the color of the console
            rectangle(4,6,69,50); //for border
            gotoxy(8,8); //places cursor
            printf("VOTE TALLY"); //prints
            tally(); //calls the tally function
            break; //terminate loop

            case 3: //if the user choose 3
            system("cls"); system("COLOR 07"); //places cursor and changes the color of the console
            rectangle(4,6,69,50); //for border
            gotoxy(8,8); //places cursor
            printf("CANDIDATES ELECTED"); //prints
            viewCandidates(); //calls the view candidates function
            break; //terminate loop

            case 4:
            {char ans; //local variable

            gotoxy(18,21); system("COLOR 06"); //places cursor and changes the color of the console
            printf("Are you sure you want to exit?\n"); //asks the user if they really want to exit
            gotoxy(18,22); //places the cursor
            printf("Enter choice [y/n]: "); //ask their answer
            scanf("%s", &ans); //scans their answer
            if(ans=='y' || ans=='Y'){ //if their answer is y for yes
            gotoxy(17,4); system("COLOR 04"); //places the cursor and changes the color of the console
            printf("Thank you for using our program!\n"); //prints
            bye();} //calls the exit function
            else {
            gotoxy(18,24); system("COLOR 02"); //places the cursor and changes the color of the console
            printf("Please wait"); //prints
            delay(); //loading function
            votesys();} //redirect back to the voting system function
            break;} //terminate loop

            default: //if they entered an invalid number or choice
            gotoxy(18,21); system("COLOR 04"); //places the cursor and changes the color of the console
            printf("Select 1-4 only"); //ask them to enter from 1-4 only
            gotoxy(18,22); //places the curser
            system("pause");  //pauses the console
        }
    }
}


int tallyprint(char posloc[31]) //print the tally
{
    int i;//local variable

    for(i=0; i<=l.Clast; i++)
    {if((strcasecmp(l.candidate[i].position, posloc)==0)) //compares strings
        {
            printf("\t%s\t\t%d\n",l.candidate[i].name, l.candidate[i].vote);} //if same then prints
    }
}


void tally() //tally function to see the leading candidates
{
    cREC cr; //local variable for the array of structures
    char temp[MAX][31]; //temporary array for sorting
    int p; //variable declaration

    for(p=0; p<=l.Clast;p++)
    {if (strcasecmp(l.candidate[p].position,l.candidate[p+1].position)==0) //compares strings
        { continue;} //proceed
    else if (strcasecmp(l.candidate[p].position,l.candidate[p+1].position)!=0)  //compares strings
    {   strcpy(temp[p],l.candidate[p].position);  //if not same copies the string to save in the temp
        printf("\n\n\t%s:\n",temp[p]);} //prints output

    tallyprint(temp[p]); //function to print names with the same position
    }
    printf("\n\n\t"); system("pause"); //prints new line and pauses the console
}
