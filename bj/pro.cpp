//Instructions before playing game
// Objective:
// The primary goal of the game is to beat the dealer by having a hand value closest to 21 without exceeding it.

// Card Values:

// Number cards (2-10) are worth their face value.
// Face cards (Jack, Queen, King) are each worth 10 points.
// Ace values are 1 point.
// Gameplay:

// Dealing:

// Each player is dealt two cards, face up.
// The dealer receives one card face up and one card face down (the hole card).
// Player's Turn:

// Players can choose to "hit" (take another card) or "stand" (keep their current hand).
// Players can continue to hit until they decide to stand or until the total value of their hand exceeds 21, in which case they "bust" and lose the round.
// Dealer's Turn:

// The dealer reveals the hole card.
// The dealer must hit until their hand value is 17 or higher. Once the dealer reaches 17 or higher, they must stand.
// If the dealer busts, player win.
// Winning:

// If a player's hand is closer to 21 than the dealer's hand without exceeding 21, the player wins.
//If the player wins in the first two dealt cards then player gets 1.5 times the bet
//or else player gets double the bet




#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
// #include <fstream> // File handling

using namespace std;

// Define card ranks and suits
const string ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
const string suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };

// Function to generate a deck of cards
vector<string> generateDeck() {
    vector<string> deck;
    for (const string &suit : suits) {
        for (const string &rank : ranks) {
            deck.push_back(rank + " of " + suit);
        }
    }
    return deck;
}

// Function to shuffle the deck
void shuffleDeck(vector<string> &deck) {
    for (int i = 0; i < deck.size(); i++) {
        int j = rand() % deck.size();
        swap(deck[i], deck[j]);
    }
}

// Function to calculate the value of a hand
int calculateHandValue(const vector<string> &hand) {
    int value = 0;

    for (const string &card : hand) {
        string rank = card.substr(0, card.find(" "));
        if (rank == "A") {
            value += 1; // Always count Ace as 1
        } else if (rank == "K" || rank == "Q" || rank == "J") {
            value += 10;
        } else {
            value += stoi(rank);
        }
    }

    return value;
}

// Function to save player details to a file
// void savePlayerDetails(const string& playerName, int playerMoney, int bet, const vector<string>& playerHand, const vector<string>& dealerHand) {
//     ofstream outFile(playerName + "_details.txt", ios::app);
//     if (outFile.is_open()) {
//         outFile << "Bet: " << bet << endl;
//         outFile << "Player's hand: ";
//         for (const string &card : playerHand) {
//             outFile << card << ", ";
//         }
//         outFile << "Total Value: " << calculateHandValue(playerHand) << endl;

//         outFile << "Dealer's hand: ";
//         for (const string &card : dealerHand) {
//             outFile << card << ", ";
//         }
//         outFile << "Total Value: " << calculateHandValue(dealerHand) << endl;

//         outFile << "Player's Money: " << playerMoney << endl;

//         outFile.close();
//     } else {
//         cout << "Unable to open file for saving details." << endl;
//     }
// }

int main() {
    srand(static_cast<unsigned>(time(0)));

    cout << "Welcome to Blackjack!" << endl;
    int playerMoney = 100;  // Starting money
    int bet = 0;

    cout << "Enter your name: ";
    string playerName;
    cin >> playerName;

    while (playerMoney > 0) {
        vector<string> deck = generateDeck();
        shuffleDeck(deck);

        vector<string> playerHand;
        vector<string> dealerHand;

        // Initial deal
        playerHand.push_back(deck.back());
        deck.pop_back();
        dealerHand.push_back(deck.back());
        deck.pop_back();
        playerHand.push_back(deck.back());
        deck.pop_back();
        dealerHand.push_back(deck.back());
        deck.pop_back();

        cout << "\nYou have $" << playerMoney << ". Place your bet: ";
        cin >> bet;

        if (bet > playerMoney || bet <= 0) {
            cout << "Invalid bet. Please place a valid bet." << endl;
            continue;
        }

        cout << "\nYour hand: ";
        for (const string &card : playerHand) {
            cout << card << ", ";
        }
        cout << "Total Value: " << calculateHandValue(playerHand) << endl;

        cout << "Dealer's face-up card: " << dealerHand[1] << endl;

        if (calculateHandValue(playerHand) == 21) {
            cout << "Blackjack! You win $" << bet * 1.5 << "!" << endl;
            playerMoney += bet * 1.5;
        } else {
            while (true) {
                char choice;
                cout << "Do you want to 'hit' or 'stand'? ";
                cin >> choice;

                if (choice == 'h') {
                    playerHand.push_back(deck.back());
                    deck.pop_back();
                    cout << "Your hand: ";
                    for (const string &card : playerHand) {
                        cout << card << ", ";
                    }
                    cout << "Total Value: " << calculateHandValue(playerHand) << endl;

                    if (calculateHandValue(playerHand) > 21) {
                        cout << "Bust! You lose $" << bet << "!" << endl;
                        playerMoney -= bet;
                        break;
                    }
                } else if (choice == 's') {
                    while (calculateHandValue(dealerHand) < 17) {
                        dealerHand.push_back(deck.back());
                        deck.pop_back();
                    }

                    cout << "\nDealer's hand: ";
                    for (const string &card : dealerHand) {
                        cout << card << ", ";
                    }
                    cout << "Total Value: " << calculateHandValue(dealerHand) << endl;

                    if (calculateHandValue(dealerHand) > 21 || calculateHandValue(dealerHand) < calculateHandValue(playerHand)) {
                        cout << "You win $" << bet << "!" << endl;
                        playerMoney += bet;
                    } else if (calculateHandValue(dealerHand) > calculateHandValue(playerHand)) {
                        cout << "You lose $" << bet << "!" << endl;
                        playerMoney -= bet;
                    } else {
                        cout << "It's a tie. Your money is returned." << endl;
                    }

                    // Save player details to a file
                    // savePlayerDetails(playerName, playerMoney, bet, playerHand, dealerHand);

                    break;
                } else {
                    cout << "Invalid choice. Please enter 'h' for hit or 's' for stand." << endl;
                }
            }
        }
    }


    cout << "Out of money! Game over." << endl;

    return 0;
}
