//Nathaniel Fleming
//Final Project; Console-Based Solitaire-ish game

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct playCard
{
	int value; //1-13
	int cardType; //1-4, even = red, odd = blue
	struct playCard *next;
	struct playCard *previous;
	int visible; //0 is hidden, 1 is shown
};

typedef struct playCard PlayCard;

struct cardStack
{
	int stackNum; //which stack this is (0 is deal stack, 1-4 are the suit stacks, 5-11 are the board stacks
	struct playCard *top; //card that is visible
	struct playCard *bottom; //card at the beginning of the stack
	struct cardStack *next; //next stack
	struct cardStack *previous; // previous card stack
};

struct playCard *current;

typedef struct cardStack CardStack;

void makeDeck(struct playCard *cardDeck)
{

	int i = 0;
	int j = 0;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 13; j++)
		{
			cardDeck[((13 * i) + j)].value = j+1;
			printf("\n I: %d,J: %d - %d",i, j, cardDeck[((13 * i) + j)].value);
			cardDeck[((13 * i) + j)].cardType = i+1;
			cardDeck[((13 * i) + j)].next = NULL;
			cardDeck[((13 * i) + j)].previous = NULL;
			cardDeck[((13 * i) + j)].visible = 0;
		}
	}
}

void shuffle(struct playCard *cardDeck)
{
	int slot1, slot2;
	struct playCard temp;
	int i = 104; //how many times to shuffle deck
	srand((unsigned int)time(NULL)); // seed the current random
	for (i = 104; i >= 0; i--)
	{
	
		//choose slots to swap
		slot1 = rand() % 52;
		printf("\nslot 1: %d - %d", slot1, cardDeck[slot1].value);
		slot2 = rand() % 52;
		printf("\nslot 2: %d - %d", slot2, cardDeck[slot2].value);

		//swap chosen cards
		temp = cardDeck[slot1];
		cardDeck[slot1] = cardDeck[slot2];
		cardDeck[slot2] = temp;

	}
}

void buildBoard(struct cardStack *dealPile)
{
	struct cardStack *cardPile;
	cardPile = calloc(1, sizeof(struct cardStack));

	dealPile->previous = NULL;
	dealPile->next = cardPile;
	dealPile->stackNum = 0;
	dealPile->bottom = NULL;
	dealPile->top = NULL;
	cardPile->previous = dealPile;
	cardPile->next = NULL;
	cardPile->stackNum = 1;
	cardPile->top = NULL;
	cardPile->bottom = NULL;

	int i;
	for (i = 2; i < 13; i++)
	{
		struct cardStack *newPile;
		newPile = calloc(1, sizeof(struct cardStack));

		newPile->previous = cardPile;
		cardPile->next = newPile;
		newPile->next = NULL;
		newPile->stackNum = i;
		newPile->top = NULL;
		newPile->bottom = NULL;

		cardPile = newPile;
	}

}

void deal(struct playCard *cardDeck, struct cardStack *dealPile)
{
	struct cardStack *workingPile;//pile to be manipulated
	struct playCard *workingCard, *tempCard;//card to be worked with

	//height go 1-7 from left to right
	int i = 0;
	int dc = 0; //counter for where in deck array we are
	int cur = 12; //current level on
	workingCard = cardDeck;
	for (cur = 11; cur > 4; cur--) //iterate through every dealing level
	{
		printf("\n---------\nCur: %d", cur);
		for (i = cur; i > 4; i--)//deal rows out
		{
			printf("\ni: %d", i);
			
			workingPile = dealPile;
			printf("\n defined working card and pile");
			while (workingPile->stackNum != i)
			{
				workingPile = workingPile->next;
			}
			
			//printf("\n working pile is set to farthest one");
			printf("\nWorking Pile -> bottom: %d\n ->top: %d", workingPile->bottom, workingPile->top);
			
			if (workingPile->bottom == NULL)//if the stack is empty
			{
				printf("\n working pile was empty");
				printf("\nWorking card %d", workingCard);
				workingPile->bottom = workingCard;
				workingPile->top = workingPile->bottom;
				workingPile->top->visible = 1;
				workingCard++;
			}
			else //if it isnt empty
			{
				printf("\n working pile wasnt empty");
				printf("\nWorking card %d", workingCard);
				(workingPile->top)->next = workingCard;
				//printf("\n(workingPile->top)->next %d", (workingPile->top)->next);
				//printf("\n 1");
				workingCard->previous = workingPile->top;
				//printf("\n 2");
				workingPile->top->visible = 0;
				workingPile->top = workingCard;
				workingPile->top->visible = 1;
				(workingPile->top)->next = NULL;
				//printf("\n 3");
				workingCard++;
			}
		}
	}
	
	i = 0;
	workingPile = dealPile;
	//build deck to deal from
	//circular list to be able to keep cycling through
	while (workingCard != &cardDeck[51])
	{
		if (i == 0)
		{
			printf("\nDeal pile was empty");
			workingPile->top = workingCard;
			tempCard = workingCard;
			workingCard->visible = 1;
			i++;
		}
		else
		{
			printf("\nDeal pile wasn't empty");
			tempCard->previous = workingCard;
			workingCard->next = tempCard;
			workingPile->top->next = workingCard;
			workingCard->previous = workingPile->top;
			tempCard = workingCard;
			workingCard->visible = 1;
		}
		workingCard++;
	}
	printf("\nEnd or array of cards");
	tempCard->previous = workingCard;
	workingCard->next = tempCard;
	workingPile->top->next = workingCard;
	workingCard->previous = workingPile->top;
	workingCard->visible = 1;
}

void printBoard(struct cardStack *curStack)
{
	/*card designs

	/----------\
	|8   Hearts|
	\----------/

	/----------\
	|8   Spades|
	\----------/

	/----------\
	|8    Clubs|
	\----------/

	/----------\
	|8  Diamond|
	\----------/

	/----------\
	|   None   |
	\----------/

	*/

	struct cardStack *workingStack;
	workingStack = curStack;
	struct playCard *workingCard;
	int i, j;
	printf("(1 --- 2 --- 3 --- 4)");
	//Draw 4 top, main stacks first. 
	for (j = 0; j < 4; j++)//iterate through the rows of cards
	{
		printf("\n");
		for (i = 1; i <= 4; i++)//fill each column of current row
		{
			workingStack = curStack;

			switch (j)
			{
			case 1:
				printf("/----------\\ ");
				break;

			case 2:
				while (workingStack->stackNum < i)//get to correct stack to output;
				{
					workingStack = workingStack->next;
				}
				if (workingStack->top == NULL) // no card in the stack
				{
					printf("|   ");
					switch (workingStack->stackNum)
					{
					case 1:
						printf(" Spades| ");
						break;

					case 2:
						printf(" Hearts| ");
						break;

					case 3:
						printf("  Clubs| ");
						break;

					case 4:
						printf("Diamond| ");
						break;
					}
				}
				else // a card is in the stack
				{ //print out card value end of display
					workingCard = workingStack->top;
					if ((workingCard)->value >= 10)
					{
						switch ((workingCard)->value)
						{
						case 10:
							printf("|%d ", (workingCard)->value);
							break;

						case 11:
							printf("|J  ");
							break;

						case 12:
							printf("|Q  ");
							break;

						case 13:
							printf("|K  ");
							break;
						}
					}
					else
					{
						if ((workingCard)->value == 1)
						{
							printf("|A  ");
						}
						else
						{
							printf("|%d  ", (workingCard)->value);
						}
					}

					//print out card class end of display
					switch((workingCard
)->cardType)
					{
					case 1:
						printf(" Spades| ");
						break;

					case 2:
						printf(" Hearts| ");
						break;

					case 3:
						printf("  Clubs| ");
						break;

					case 4:
						printf("Diamond| ");
						break;
					}
				}
				break;

			case 3:
				printf("\\----------/ ");
				break;
			}
			
		}
	}
	
	printf("\n\n");
	int k;
	//print the 7 normal stacks
	for (k = 0; k < 13; k++) //traverse down rows
	{
		for (j = 0; j < 4; j++)
		{
			printf("\n");
			for (i = 11; i > 4; i--) //fill row
			{
				workingStack = curStack;

				switch (j)
				{
				case 1:
					printf("/----------\\ ");
					break;

				case 2:
					while (workingStack->stackNum < i)//get to correct stack to output;
					{
						workingStack = workingStack->next;
						//printf("stackNum: %d  ", workingStack->stackNum);
					}
					if (workingStack->top == NULL) // no card in the stack
					{
						printf("|   None   | ");
					}
					else // a card is in the stack
					{ //print out card value end of display

						int count = 0;
						workingCard = workingStack->bottom;
						
						for (count = 0; count < k; count++)
						{
							if(workingCard!= NULL)
								workingCard = workingCard->next;
						}
						if (workingCard == NULL)
						{
							printf("|   None   | ");
						}
						else if (k > 0 && workingCard->previous == NULL)
						{
							printf("|   None   | ");
						}
						else
						{
							if (workingCard->visible == 0)
							{
								printf("|  ??????  | ");
							}
							else
							{
								if ((workingCard)->value >= 10)
								{
									switch ((workingCard)->value)
									{
									case 10:
										printf("|%d ", (workingCard)->value);
										break;

									case 11:
										printf("|J  ");
										break;

									case 12:
										printf("|Q  ");
										break;

									case 13:
										printf("|K  ");
										break;
									}
								}
								else
								{
									if ((workingCard)->value == 1)
									{
										printf("|A  ");
									}
									else
									{
										printf("|%d  ", (workingCard)->value);
									}
								}

								//print out card class end of display
								switch ((workingCard)->cardType)
								{
								case 1:
									printf(" Spades| ");
									break;

								case 2:
									printf(" Hearts| ");
									break;

								case 3:
									printf("  Clubs| ");
									break;

								case 4:
									printf("Diamond| ");
									break;
								}
							}
						}
					}
					break;

				case 3:
					printf("\\----------/ ");
					break;
				}
			}
		}
	}
	// ??????
	//	NONE

	//Print the deal section

	printf("\nDealt Cards (0 --- 1 --- 2):");
	for (j = 1; j < 4; j++)//iterate through the rows of cards
	{
		//printf("current: %d, ->value %d, ->next %d", current, current->value, current->next);
		printf("\n");
		for (i = 1; i <= 3; i++)//fill each column of current row
		{
			
			//printf("workingStack->top: %d", workingStack->top->value);
			switch (j)
			{

			case 0:
				//current = curStack->top;
				break;
			case 1:
				printf("/----------\\ ");
				break;

			case 2:
				if (workingStack->top == NULL) // no card in the stack
				{
					printf("|   None   | ");
				}
				else // a card is in the stack
				{ //print out card value end of display
					if (current->value >= 10)
					{
						switch (current->value)
						{
						case 10:
							printf("|%d ", current->value);
							break;

						case 11:
							printf("|J  ");
							break;

						case 12:
							printf("|Q  ");
							break;

						case 13:
							printf("|K  ");
							break;
						}
					}
					else
					{
						if (current->value == 1)
						{
							printf("|A  ");
						}
						else
						{
							printf("|%d  ", current->value);
						}
					}

					//print out card class end of display
					switch (current->cardType)
					{
					case 1:
						printf(" Spades| ");
						break;

					case 2:
						printf(" Hearts| ");
						break;

					case 3:
						printf("  Clubs| ");
						break;

					case 4:
						printf("Diamond| ");
						break;
					}
					current = current->next;
				}
				break;

			case 3:
				printf("\\----------/ ");
				break;
			}
			
		}
	}

}

void MoveCards(struct cardStack *curStack)
{
	//printf("\ncurStack->bottom: %d", curStack->bottom);

	int cardX, cardY, cStack;

	printf("\nPlease select the card you want to move. Select the card based on the grid position.\n0,0 is top right corner of the main board.");
	printf("\n-1 is the y of the dealt cards.These cards have their x coordinate listed above them.");
	printf("\nPlease Enter desired card X coordinate : ");
	scanf("%d", &cardX);
	printf("\nPlease enter Desired card Y coordinate: ");
	scanf("%d", &cardY);
	printf("please enter what column to move the card to (1-4 are the upper stacks in the order they are labelled, 5-11 are the normal columns with 5 being farthest right): ");
	scanf("%d", &cStack);

	printf("cardYb: %d", cardY);
	struct cardStack *workingStack;
	struct playCard *workingCard;
	//printf("cardYa: %d", cardY);
	workingStack = curStack;
	int f = 0;
	//printf("cardY: %d", cardY);
	if (cardY != -1) //card is part of the main set
	{
		printf("\nCard is not part of dealing section");
		//printf("\nworkingStack->stackNum: %d, cardX: %d", workingStack->stackNum, cardX);
		//printf("\nworkingStack->bottom: %d", workingStack->bottom);

		while (workingStack->stackNum != cardX + 5)
		{
			workingStack = workingStack->next;
			printf("\nworkingStack->stackNum: %d, cardX+5: %d", workingStack->stackNum, cardX+5);
			//printf("\nworkingStack->bottom: %d", workingStack->bottom);
		}
		//printf("\nworkingStack->bottom: %d",workingStack->bottom);
		workingCard = workingStack->bottom; //
		//printf("\n workingCard: %d\ncardY: %d", workingCard, cardY);

		printf("\ncardY: %d\nworkingCard->value: %d", cardY, workingCard->value);

		for (f = 0; f < cardY; f++)
		{
			if (workingCard->next != NULL)
			{
				workingCard = workingCard->next;
				printf("\nf: %d,workingCard->value: %d,  ->next: %d", f, workingCard->value, workingCard->next);
			}
			else
			{
				//printf("\nInvalid card selection");
				f = 20;
			}
		}
		if (workingCard->visible == 0)
			f = 20;
		printf("\nf: %d,workingCard->value: %d,  ->next: %d", f, workingCard->value, workingCard->next);
	}
	else//card is part of the dealing section
	{
		printf("\nCard is part of dealing section, cardX: %d", cardX);
		if (cardX < 3)
		{
			int aaaa;
			for (aaaa = 0; aaaa < (3 - cardX); aaaa++)
			{
				current = current->previous;
			}
			workingCard = current;
			printf("\ncurrent: %d, ->value: %d, ->suit: %d", current, current->value, current->cardType);
		}
		else
		{
			printf("\n dealing section selection out of bounds");
			f = 20;
		}
	}

	printf("\nf: %d,workingCard->value: %d", f, workingCard->value);

	if (f >= 20)//check if card was valid
	{
		printf("\nInvalid card selection. Returning to action selection...\n");
	}
	//card is a good selection
	else
	{
		//printf("\n: workingCard");
		printf("\ncStack: %d",cStack);
		//workingStack = curStack;
		if (cStack > 4) // check if destination is a suit stack or not
		{//not a suit stack
			
			if (workingCard == current)
			{//if current
				current = current->next;
				workingStack = curStack;
				while (workingStack->stackNum != cStack)
				{
					workingStack = workingStack->next;
				}
				printf("\ncard %d of %d is being insterted into column %d", workingCard->value, workingCard->cardType, workingStack->stackNum);
				printf("\n\nworkingStack: %d, ->top %d, ->top->value: %d, ->top->type: %d, \nworkingCard: %d, workingCard->value: %d", workingStack->stackNum, workingStack->top, workingStack->top->value, workingStack->top->cardType, workingCard, workingCard->value);

				if (workingStack->bottom == NULL) //if stack is empty
				{
					workingCard->next->previous = workingCard->previous;
					workingCard->previous->next = workingCard->next;
					workingCard->next = NULL;
					workingStack->bottom = workingCard;
					workingCard->previous = NULL;

					workingStack->top = workingCard;
				}
				else if ((((workingCard->cardType) + (workingStack->top->cardType)) % 2 != 0) && ((workingCard->value) == ((workingStack->top->value) - 1))) //check if top card is compatible with current card
				{
					workingStack->top->next = workingCard;
					workingCard->next->previous = workingCard->previous;
					workingCard->previous->next = workingCard->next;
					workingCard->next = NULL;
					workingCard->previous = workingStack->top;
					workingStack->top = workingCard;
					printf("\nworkingCard: %d, workingStack->top: %d", workingCard, workingStack->top);
				}
				else
				{
					printf("\n Invalid destination. Returning to action selection.");
				}
			}
			else
			{//not current
				if(workingCard->previous != NULL)
					workingCard->previous->visible = 1;
				workingStack->top = workingCard->previous;
				workingStack = curStack;
				while (workingStack->stackNum != cStack)
				{
					workingStack = workingStack->next;
				}
				printf("\ncard %d of %d is being insterted into column %d", workingCard->value, workingCard->cardType, workingStack->stackNum);
				printf("\n\nworkingStack: %d, ->top %d, ->top->value: %d, ->top->type: %d, \nworkingCard: %d, workingCard->value: %d", workingStack->stackNum, workingStack->top, workingStack->top->value, workingStack->top->cardType, workingCard, workingCard->value);

				if (workingStack->bottom == NULL) //if stack is empty
				{
					workingStack->bottom = workingCard;
					if (workingCard->previous != NULL)
					{
						workingCard->previous->next = NULL;
						workingCard->previous = NULL;
					}
					workingStack->top = workingCard;
				}
				else if ((((workingCard->cardType) + (workingStack->top->cardType)) % 2 != 0) && ((workingCard->value) == ((workingStack->top->value) - 1))) //check if top card is compatible with current card
				{
					workingStack->top->next = workingCard;
					if(workingCard->previous != NULL)
						workingCard->previous->next = NULL;
					workingCard->previous = workingStack->top;
					workingStack->top = workingCard;
					printf("\nworkingCard: %d, workingStack->top: %d", workingCard, workingStack->top);
				}
				else
				{
					printf("\n Invalid destination. Returning to action selection.");
				}

			}

		}
		//if it is a suit stack
		else if(cStack > 0)
		{
			if (((workingCard->next == NULL) && (workingCard->cardType == cStack)) || workingCard == current)
			{
				if (workingCard == current)
				{
					printf("\nworkingCard == current");
					workingStack = curStack;
					while (workingStack->stackNum != cStack)
					{
						workingStack = workingStack->next;
					}
					printf("\nworkingStack->num: %d, ->bottom: %d",workingStack->stackNum, workingStack->bottom);
					if ((workingStack->bottom == NULL) && (workingCard->value == 1))
					{
						printf("\nInserting card at bottom of stack");
						current->next->previous = current->previous;
						current->previous->next = current->next;
						current = current->next;
						workingStack->bottom = workingCard;
						workingStack->top = workingCard;
						workingCard->previous = NULL;
						workingCard->next = NULL;
						printf("\nWorkingCard->value: %d, ->next: %d", workingCard->value, workingCard->next);
						printf("\nInserted card at bottom of stack");
					}
					else if ((workingStack->bottom != NULL) && (workingCard->value == (workingStack->top->value + 1)))
					{
						printf("\nInserting card at top of stack");
						current->next->previous = current->previous;
						current->previous->next = current->next;
						current = current->next;
						workingStack->top->next = workingCard;
						workingCard->previous = workingStack->top;
						workingStack->top = workingCard;
						workingCard->next = NULL;
						printf("\nInserted card at top of stack");
					}
					else
					{
						printf("\n Invalid destination A. Returning to action selection.");
					}
				}
				else
				{
					workingStack->top = workingCard->previous;
					workingStack = curStack;
					while (workingStack->stackNum != cStack)
					{
						workingStack = workingStack->next;
					}

					if ((workingStack->bottom == NULL) && (workingCard->value == 1))
					{
						if (workingCard->previous != NULL)
						{
							workingCard->previous->visible = 1;
							workingCard->previous->next = NULL;
						}
						workingCard->previous = NULL;
						workingStack->bottom = workingCard;
						workingStack->top = workingCard;
						workingCard->next = NULL;
					}
					else if ((workingStack->bottom != NULL) && (workingCard->value == (workingStack->top->value + 1)))
					{
						workingStack->top->next = workingCard;
						if (workingCard->previous != NULL)
						{
							workingCard->previous->visible = 1;
							workingCard->previous->next = NULL;
						}
						workingCard->previous = workingStack->top;
						workingStack->top = workingCard;
						workingCard->next = NULL;
					}
					else
					{
						printf("\n Invalid destination A. Returning to action selection.");
					}
				}
			}
			else
			{
				printf("\n Invalid destination B. Returning to action selection.");
			}
		}
		else
		{
			printf("\n Invalid destination C. Returning to action selection.");
		}
	}
}
	
void checkWin(struct cardStack *curStack)
{
	struct cardStack *workingStack;
	workingStack = curStack;
	int win = 1;
	workingStack = workingStack->next;//go to first suit stack
	//printf("\nA");
	int i = 0;
	for (i = 1; i < 5; i++)
	{
		//printf("\nB");
		//printf(" Ba, %d", workingStack->stackNum);
		if (workingStack->top != NULL)
		{
			if (workingStack->top->value == 13 && win == 1)
			{
				//printf("\nC, %d", workingStack->top->value);
				win = 1;
			}
			else
			{
				win = 0;
			}
		}
		else
		{
			win = 0;
		}
		workingStack = workingStack->next;
	}

	if (win != 0)
	{
		printf("Congratulations, you have successfully completed the game!/n");
	}
	else
	{
		printf("You have yet to complete the game, sorry.\n");
	}

}

void gameController(struct cardStack *curStack)
{
	int option;
	printf("Please select an option: 1-Deal 2-Move cards 3-Check for win 4-Quit: ");
	scanf("%d", &option);

	switch (option)
	{
	case 1: 
		system("cls");
		printBoard(curStack);
		gameController(curStack);
		break;

	case 2:

		MoveCards(curStack);

		printf("\nCards Moved");
		system("cls");
		current = current->previous;
		current = current->previous;
		current = current->previous;
		printBoard(curStack);
		
		gameController(curStack);
		break;

	case 3:
		checkWin(curStack);
		gameController(curStack);
		break;

	case 4:
		system("cls");
		printf("Thank You For Playing!");
		break;

	default:
		printf("\nInvalid input\n");
		gameController(curStack);
		break;
	}
}

void main()
{
	struct playCard *cardDeck;
	cardDeck = calloc(52, sizeof(struct playCard));
	printf("\nBuilding Deck...");
	makeDeck(cardDeck);

	int c = 0;
	for (c = 0; c < 52; c++)
	{
		printf("\nSlot %d - %d",c, cardDeck[c].value);
	}

	printf("\n\nShuffling Deck...");
	shuffle(cardDeck);

	printf("\n\nBuilding Board...");
	struct cardStack *dealPile;
	dealPile = calloc(1, sizeof(struct cardStack));
	buildBoard(dealPile);

	printf("\n\nDealing Deck...");
	deal(cardDeck, dealPile);

	current = dealPile->top;

	system("cls");

	struct cardStack *tempStack; //debug output to see what is in each stack
	tempStack = dealPile;
	struct playCard *tempCard;
	while (tempStack->next != NULL)
	{
		printf("\nStack Num: %d",tempStack->stackNum);
		if (tempStack->bottom != NULL)
		{
			tempCard = tempStack->bottom;
			while (tempCard->next!=NULL)
			{
				printf("\ncard is %d of class %d",tempCard->value,tempCard->cardType);
				tempCard = tempCard->next;
			}
			printf("\ncard is %d of class %d. next card is NULL", tempCard->value, tempCard->cardType);
			tempStack = tempStack->next;
		}
		else
		{
			printf("\nEmpty Stack");
			tempStack = tempStack->next;
		}
		
	}
	
	printf("\nPrinting Initial Board...");
	system("cls");

	printBoard(dealPile);
	gameController(dealPile);
}