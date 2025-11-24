#include <stdio.h>
#include <LPC17xx.H>
#include <time.h>
#include "GLCD.h"
#include "KBD.h"
#include <time.h>
#include <stdbool.h>
#include <stdlib.h> 

volatile uint32_t seed_counter = 0;


#define __FI        1   /* Font index 16x24 */
char buffer[50];
int deckCount = 51;


typedef enum{
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
} Suit;

typedef enum {
    ACE = 1,
    TWO, THREE, FOUR, FIVE, SIX,
    SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING
} Rank;

typedef struct {
    Suit suit;
    Rank rank;
} Card;

const char *suitNames[] = {"C", "D", "H", "S"};

const char *rankNames[] = {
    "Ace", "2", "3", "4", "5", "6", "7",
    "8", "9", "10", "Jack", "Queen", "King"
};

void shuffle(Card *deck){
				int i;
        srand(seed_counter); 
    for (i = 52 - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void drawCards(Card *deck, Card *hand, int number){
		int i;
    for(i = 0;i<number;i++){
    hand[i] = deck[deckCount];
    deckCount--;
    }
}

Card mergeCards(Card *river, Card *hand, Card *handTotal){
		int i;
		int j;
    for(i = 0;i<3;i++){
        handTotal[i] = river[i];
    }
    for(j = 0;j<2;j++){
        handTotal[j+3] = hand[j];
    }
}

void sortRanks(int *ranks, int n) {
		int i;
		int j;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (ranks[i] > ranks[j]) {
                int temp = ranks[i];
                ranks[i] = ranks[j];
                ranks[j] = temp;
            }
        }
    }
}

//-----------------------------CHECK FOR COMBINATIONS
bool checkPair(Card *handTotal){
    int pairCount = 0;
		int i;
		int j;
    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            if (handTotal[i].rank == handTotal[j].rank) {
                pairCount++;
            }
        }
    }

    if(pairCount == 1){
        return true;
    }
    else{
        return false;
    }
}

bool checkTwoPair(Card *handTotal){
    int pairCount = 0;
		int i;
		int j;
    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            if (handTotal[i].rank == handTotal[j].rank) {
                pairCount++;
            }
        }
    }

    if(pairCount == 2){
        return true;
    }
    else{
        return false;
    }
}

bool checkThreeOfKind(Card *handTotal) {
		int i;
		int j;
    for (i = 0; i < 5; i++) {
        int count = 1; 

        for (j = 0; j < 5; j++) {
            if (i != j && handTotal[i].rank == handTotal[j].rank) {
                count++;
            }
        }

        if (count == 3) {
            return true; 
        }
    }

    return false; 
}


bool checkStraight(Card *handTotal) {
    int ranks[5];
		int i;
		int j;
    for (i = 0; i < 5; i++) {
        ranks[i] = handTotal[i].rank;
    }

    sortRanks(ranks, 5);

    if (ranks[4] == 13 && ranks[0] == 1 && ranks[1] == 10 && ranks[2] == 11 && ranks[3] == 12) {
        return true;
    }


    for (j = 0; j < 4; j++) {
        if (ranks[j + 1] - ranks[j] != 1) {
            return false;
        }
    }

    return true;
}

bool checkFlush(Card *handTotal){
    int spade = 0;
    int club = 0;
    int heart = 0;
    int diamond = 0;
		int i;
    for(i = 0;i<5;i++){
        if(handTotal[i].suit == 0){
            club++;
        }
        if(handTotal[i].suit == 1){
            diamond++;
        }
        if(handTotal[i].suit == 2){
            heart++;
        }
        if(handTotal[i].suit == 3){
            spade++;
        }
    }

    if(spade == 5 || club == 5|| heart == 5 || diamond == 5){
        return true;
    }
    return false;
}

bool checkFullHouse(Card *handTotal) {
    int tripleRank = -1;
    int pairRank = -1;
		int i;
		int j;
		int k;
		int g;
    // Find a triple
    for (i = 0; i < 5; i++) {
        int count = 1;
        for (j = 0; j < 5; j++) {
            if (i != j && handTotal[i].rank == handTotal[j].rank) {
                count++;
            }
        }

        if (count == 3) {
            tripleRank = handTotal[i].rank;
        }
    }

    // Find a pair (with a different rank than the triple)
    for (k = 0; k < 5; k++) {
        int count = 1;
        for (g = 0; g < 5; g++) {
            if (k != g && handTotal[k].rank == handTotal[g].rank) {
                count++;
            }
        }

        if (count == 2 && handTotal[k].rank != tripleRank) {
            pairRank = handTotal[k].rank;
        }
    }

    return (tripleRank != -1 && pairRank != -1);
}

bool checkFourOfKind(Card *handTotal){
		int i;
		int j;
    for (i = 0; i < 5; i++) {
        int count = 1; 

        for (j = 0; j < 5; j++) {
            if (i != j && handTotal[i].rank == handTotal[j].rank) {
                count++;
            }
        }

        if (count == 4) {
            return true; 
        }
    }

    return false; 
}

bool checkStraightFlush(Card *handTotal){
    if(checkFlush(handTotal) && checkStraight(handTotal)){
        return true;
    }
    return false;
}

bool checkRoyalFlush(Card *handTotal){
    bool straightFlush = false;
    bool king = false;
    bool ace = false;
		int i;
    if(checkStraightFlush(handTotal)){
        straightFlush = true;
    }
    for(i = 0;i<5;i++){
        if(handTotal[i].rank == 13){
            king = true;
        }
        if(handTotal[i].rank == 1){
            ace = true;
        }
    }
    
    if(king && ace && straightFlush){
        return true;
    }
    return false;
}
//------------------------------Check Combination Score

int checkScore(Card *handTotal){
    int score;
    if(checkRoyalFlush(handTotal)){
        score = 9;
    }else if(checkStraightFlush(handTotal)){
        score = 8;
    }else if(checkFourOfKind(handTotal)){
        score = 7;
    }else if(checkFullHouse(handTotal)){
        score = 6;
    }else if(checkFlush(handTotal)){
        score = 5;
    }else if(checkStraight(handTotal)){
        score = 4;
    }else if(checkThreeOfKind(handTotal)){
        score = 3;
    }else if(checkTwoPair(handTotal)){
        score = 2;
    }else if(checkPair(handTotal)){
        score = 1;
    }else{                  //High Card
        score = 0;
    }

    return score;
}

int checkWin(int scorePlayer, int scoreDealer, Card handPlayer[], Card handDealer[]){
    if(scorePlayer > scoreDealer){
        return 1;   //Win
    }else if(scorePlayer == scoreDealer && scorePlayer == 0){   //Both High Card
        int highCardPlayer;
        int highCardDealer;

        if(handPlayer[0].rank > handPlayer[1].rank) {
            highCardPlayer = handPlayer[0].rank;
        }else{
            highCardPlayer = handPlayer[1].rank;
        }

        if(handDealer[0].rank > handDealer[1].rank) {
            highCardDealer = handDealer[0].rank;
        }else{
            highCardDealer = handDealer[1].rank;
        }

        if(highCardPlayer > highCardDealer){
            return 1; //Win
        }
    }else if(scorePlayer == scoreDealer){
        return 2; //Tie
    }

    return 0; //Loss

}

void printCombination(int score, int row){
    if(score == 9){
				GLCD_DisplayString(row, 4, __FI, "Royal Flush    ");
    }else if(score == 8){
				GLCD_DisplayString(row, 4, __FI, "Straight Flush ");
    }else if(score == 7){
				GLCD_DisplayString(row, 4, __FI, "Four of a Kind ");
    }else if(score == 6){
				GLCD_DisplayString(row, 4, __FI, "Full House     ");
    }else if(score == 5){
				GLCD_DisplayString(row, 4, __FI, "Flush          ");
    }else if(score == 4){
				GLCD_DisplayString(row, 4, __FI, "Straight       ");
    }else if(score == 3){
				GLCD_DisplayString(row, 4, __FI, "Three of a Kind");
    }else if(score == 2){
        GLCD_DisplayString(row, 4, __FI, "Two Pair       ");
    }else if(score == 1){
        GLCD_DisplayString(row, 4, __FI, "Pair           ");   
    }else if(score == 0){
        GLCD_DisplayString(row, 4, __FI, "High Card      ");
    }
}

//------------------------------MAIN
int pokergame(){
    int chips = 1000;
    int chipsToBet = 100;
    int play = 1;
		uint32_t key, last_key = 0xFF;
		
    while(play){
				int result;
				int s;
				int r;
				int scorePlayer;
				int scoreDealer;
        int deckCount = 51;
        Card deck[52];
        int i = 0;
				Card handPlayer[2];
        Card handDealer[2];
				Card handTotal1[5];
        Card handTotal2[5];
				Card river[3];
			
				GLCD_Clear(White);
				GLCD_SetBackColor(White);
				GLCD_SetTextColor(Red);
			
        //Create Deck
        for(s = 1;s<5;s++){
            for(r = 1;r<14;r++){
                deck[i].suit = (Suit)s;
                deck[i].rank = (Rank)r;
                i++;
            }
        }

        //Shuffle Deck
        shuffle(deck);
        
        //Create Hands of cards
        
        drawCards(deck, handPlayer, 2);
        drawCards(deck, handDealer, 2);

        
        drawCards(deck, river, 3);

        
        mergeCards(river, handPlayer, handTotal1);
        mergeCards(river, handDealer, handTotal2);
        
        //Print Every Card in Deck
        /*
        for (int j = 0; j < 52; j++) {
        printf("Card %2d: Rank %2d, Suit %d\n", j + 1, deck[j].rank, deck[j].suit);
        }*/
				
				//PRINT PLAYER CARDS
				sprintf(buffer, "%s of %s", rankNames[handPlayer[0].rank - 1], suitNames[handPlayer[0].suit - 1]);
				GLCD_DisplayString(0, 0, __FI, (unsigned char*)"Player:");
				GLCD_DisplayString(0, 7, __FI, (unsigned char*)buffer);
				
        sprintf(buffer, "%s of %s", rankNames[handPlayer[1].rank - 1], suitNames[handPlayer[1].suit - 1]);
				GLCD_DisplayString(1, 0, __FI, (unsigned char*)"Player:");
				GLCD_DisplayString(1, 7, __FI, (unsigned char*)buffer);
        
        //PRINT RIVER
				
				sprintf(buffer, "%s of %s", rankNames[river[0].rank - 1], suitNames[river[0].suit - 1]);
				GLCD_DisplayString(3, 0, __FI, (unsigned char*)"River:");
				GLCD_DisplayString(3, 6, __FI, (unsigned char*)buffer);
				
				sprintf(buffer, "%s of %s", rankNames[river[1].rank - 1], suitNames[river[1].suit - 1]);
				GLCD_DisplayString(4, 0, __FI, (unsigned char*)"River:");
				GLCD_DisplayString(4, 6, __FI, (unsigned char*)buffer);
				
				sprintf(buffer, "%s of %s", rankNames[river[2].rank - 1], suitNames[river[2].suit - 1]);
				GLCD_DisplayString(5, 0, __FI, (unsigned char*)"River:");
				GLCD_DisplayString(5, 6, __FI, (unsigned char*)buffer);
				
				//PRINT CHIPS
        sprintf(buffer, "%d", chips);
				GLCD_DisplayString(7, 0, __FI, (unsigned char*)"Chips:");
				GLCD_DisplayString(7, 6, __FI, (unsigned char*)buffer);

        

				//BET
        while(1){
					
					sprintf(buffer, "%4d", chipsToBet);
					GLCD_DisplayString(8, 0, __FI, "Bet:");
					GLCD_DisplayString(8, 6, __FI, (unsigned char*)buffer);
          
					key = get_button();
					if (key != last_key) {
						last_key = key;
						if (key == KBD_UP && chipsToBet < chips) {
							chipsToBet = chipsToBet + 100;
						} else if (key == KBD_DOWN && chipsToBet > 100) {
							chipsToBet = chipsToBet - 100;
						} else if (key == KBD_SELECT) {
							chips = chips - chipsToBet;
							break; 
						}
					}
           
        }
				
				//CHECK WIN
        scorePlayer = checkScore(handTotal1);
        scoreDealer = checkScore(handTotal2);

				GLCD_Clear(White);
				//PRINT DEALER
				sprintf(buffer, "%s of %s", rankNames[handDealer[0].rank - 1], suitNames[handDealer[0].suit - 1]);
				GLCD_DisplayString(0, 0, __FI, (unsigned char*)"Dealer:");
				GLCD_DisplayString(0, 7, __FI, (unsigned char*)buffer);
				
        sprintf(buffer, "%s of %s", rankNames[handDealer[1].rank - 1], suitNames[handDealer[1].suit - 1]);
				GLCD_DisplayString(1, 0, __FI, (unsigned char*)"Dealer:");
				GLCD_DisplayString(1, 7, __FI, (unsigned char*)buffer);

        if(checkWin(scorePlayer, scoreDealer,handPlayer,handDealer) == 1){
            GLCD_DisplayString(3, 0, __FI, "Win-");
            chips = chips + (chipsToBet*2);
        }else if(checkWin(scorePlayer, scoreDealer,handPlayer,handDealer) == 0){
            GLCD_DisplayString(3, 0, __FI, "Loss-");
        }else{
            GLCD_DisplayString(3, 0, __FI, "Tie-");
            chips = chips + chipsToBet;
        }
				
				//PRINT SCORE
				GLCD_DisplayString(4, 0, __FI, "Player:");
        printCombination(scorePlayer, 5);
        GLCD_DisplayString(7, 0, __FI, "Dealer:");
        printCombination(scoreDealer, 8);

        if(chips > 0){
            char again = 'Y';

         //PLAY AGAIN?
						while(1){
							
							sprintf(buffer, "%4d", chips);
							GLCD_DisplayString(9, 0, __FI, "Chips:");
							GLCD_DisplayString(9, 6, __FI, (unsigned char*)buffer);
							
							sprintf(buffer, "%c", again);
							GLCD_DisplayString(9, 11, __FI, "Again?");
							GLCD_DisplayString(9, 18, __FI, (unsigned char*)buffer);
							
							key = get_button();
							if (key != last_key) {
								last_key = key;
								if (key == KBD_UP) {
									again = 'Y';
								} else if (key == KBD_DOWN) {
									again = 'N';
								} else if (key == KBD_SELECT) {
									if(again == 'Y'){
										break; 
									}else{
										play = 0;
										break; 
									}
									
								}
							}
							 
						}
           
        }else{
					
						GLCD_DisplayString(9, 1, __FI, "select to continue");
						while(1){
						key = get_button();
								if (key != last_key) {
									last_key = key;
									if (key == KBD_SELECT) {
										break;
									}  
								}
					
						}
						GLCD_Clear(White);
            GLCD_DisplayString(5, 0, __FI, "Not Enough Chips");
						GLCD_DisplayString(6, 0, __FI, "Select to Return");
						while(1){
						key = get_button();
								if (key != last_key) {
									last_key = key;
									if (key == KBD_SELECT) {
										play = 0;
										break;
									}  
								}
					
        }
			}
    }
    
    
    return 0;
    
}