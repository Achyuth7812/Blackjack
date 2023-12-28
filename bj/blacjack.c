#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Define card ranks and suits
const char *ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
const char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};

// Define a structure for a card
typedef struct Card {
    char rank[3];
    char suit[10];
    struct Card *next;
} Card;

// Function to generate a deck of cards using a linked list
Card *generateDeck() {
    Card *head = NULL;
    Card *current = NULL;

    for (int suit_index = 0; suit_index < 4; suit_index++) {
        for (int rank_index = 0; rank_index < 13; rank_index++) {
            Card *newCard = (Card *)malloc(sizeof(Card));
            strcpy(newCard->rank, ranks[rank_index]);
            strcpy(newCard->suit, suits[suit_index]);
            newCard->next = NULL;

            if (head == NULL) {
                head = newCard;
                current = newCard;
            } else {
                current->next = newCard;
                current = newCard;
            }
        }
    }

    return head;
}

// Function to shuffle the deck using Fisher-Yates algorithm
void shuffleDeck(Card **deckArray, int deck_size) {
    for (int i = deck_size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card *temp = deckArray[i];
        deckArray[i] = deckArray[j];
        deckArray[j] = temp;
    }
}

// Function to calculate the value of a hand
int calculateHandValue(Card *hand) {
    int value = 0;
    // int aceCount = 0;

    while (hand != NULL) {
        if (strcmp(hand->rank, "A") == 0) {
            value += 1; // Ace value is 1
            // aceCount++;
        } else if (strcmp(hand->rank, "K") == 0 || strcmp(hand->rank, "Q") == 0 || strcmp(hand->rank, "J") == 0) {
            value += 10;
        } else {
            value += atoi(hand->rank);
        }

        hand = hand->next;
    }

    // while (value > 21 && aceCount > 0) {
    //     value -= 10; // Change Ace value from 11 to 1
    //     aceCount--;
    // }

    return value;
}

// Function to free memory allocated for the deck
void freeDeck(Card *head) {
    while (head != NULL) {
        Card *temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    srand((unsigned)time(NULL));

    printf("Welcome to Blackjack!\n");

    while (1) {
        // Generate the deck using a linked list
        Card *deckList = generateDeck();

        // Convert the linked list to an array for shuffling
        int deck_size = 52;
        Card *deckArray[deck_size];
        Card *current = deckList;
        for (int i = 0; i < deck_size; i++) {
            deckArray[i] = current;
            current = current->next;
        }

        // Shuffle the array
        shuffleDeck(deckArray, deck_size);

        Card *playerHand = NULL;
        Card *dealerHand = NULL;

        // Initial deal
        for (int i = 0; i < 2; i++) {
            Card *newCardPlayer = deckArray[--deck_size];
            newCardPlayer->next = playerHand;
            playerHand = newCardPlayer;

            Card *newCardDealer = deckArray[--deck_size];
            newCardDealer->next = dealerHand;
            dealerHand = newCardDealer;
        }

        printf("\nYour hand: %s of %s, %s of %s - Total Value: %d\n", playerHand->rank, playerHand->suit,
               playerHand->next->rank, playerHand->next->suit, calculateHandValue(playerHand));
        printf("Dealer's face-up card: %s of %s\n", dealerHand->rank, dealerHand->suit);

        if (calculateHandValue(playerHand) == 21) {
            printf("Blackjack!\n");
        } else {
            while (1) {
                char choice;
                printf("Do you want to 'hit' or 'stand'? ");
                scanf(" %c", &choice);

                if (choice == 'h') {
                    Card *newCard = deckArray[--deck_size];
                    newCard->next = playerHand;
                    playerHand = newCard;

                    printf("Your hand: %s of %s, %s of %s - Total Value: %d\n", playerHand->rank, playerHand->suit,
                           playerHand->next->rank, playerHand->next->suit, calculateHandValue(playerHand));

                    if (calculateHandValue(playerHand) > 21) {
                        printf("Bust!\n");
                        break;
                    }
                } else if (choice == 's') {
                    while (calculateHandValue(dealerHand) < 17) {
                        Card *newCard = deckArray[--deck_size];
                        newCard->next = dealerHand;
                        dealerHand = newCard;
                    }

                    printf("\nDealer's hand: %s of %s, %s of %s - Total Value: %d\n", dealerHand->rank,
                           dealerHand->suit, dealerHand->next->rank, dealerHand->next->suit, calculateHandValue(dealerHand));

                    if (calculateHandValue(dealerHand) > 21 ||
                        (calculateHandValue(dealerHand) < calculateHandValue(playerHand))) {
                        printf("You win!\n");
                    } else if (calculateHandValue(dealerHand) > calculateHandValue(playerHand)) {
                        printf("You lose!\n");
                    } else {
                        printf("It's a tie.\n");
                    }

                    break;
                } else {
                    printf("Invalid choice. Please enter 'h' for hit or 's' for stand.\n");
                }
            }
        }

        // Free memory allocated for the linked list
        freeDeck(deckList);

        // Ask the player if they want to play another round
        char playAgain;
        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &playAgain);

        if (playAgain != 'y' && playAgain != 'Y') {
            break; // Exit the loop if the player doesn't want to play again
        }
    }

    printf("Thanks for playing! Goodbye.\n");

    return 0;
}
