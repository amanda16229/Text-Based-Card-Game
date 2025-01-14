#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_PLAYERS 4
#define NUM_CARDS 52
#define NUM_ATTRIBUTES 2

// Structure for Attribute
typedef struct 
{
    char name[20];
    int value;
} Attribute;

// Structure for Card
typedef struct 
{
    Attribute *attributes;
    int num_attributes;
} Card;

// Structure to represent a player
typedef struct 
{
    Card *deck; 
    int num_cards;
    char type[20]; 
} Player;

// Function to initialize a deck of cards with random attribute values
void initialize_deck(Card *deck, int num_cards, int num_attributes) 
{
    for (int i = 0; i < num_cards; i++) 
    {
        deck[i].num_attributes = num_attributes;
        deck[i].attributes = (Attribute *)malloc(num_attributes * sizeof(Attribute));
        
        // Initialize bachelor attributes
        sprintf(deck[i].attributes[0].name, "Sense of humor");
        deck[i].attributes[0].value = rand() % 10;
        
        sprintf(deck[i].attributes[1].name, "Style");
        deck[i].attributes[1].value = rand() % 10;
    }
}

// Function to find the index of the player with the highest value of the chosen attribute
int find_winner(Player *players, int num_players, int attribute_index) 
{
    int max_value = players[0].deck[0].attributes[attribute_index].value;
    int winner_index = 0;
    
    for (int i = 1; i < num_players; i++) 
    {
        if (players[i].deck[0].attributes[attribute_index].value > max_value) 
        {
            max_value = players[i].deck[0].attributes[attribute_index].value;
            winner_index = i;
        }
    }
    
    return winner_index;
}

// Function to play a round of the game
void play_round(Player *players, int num_players, int attribute_index) 
{
    int winner_index = find_winner(players, num_players, attribute_index);
    
    printf("Player %d wins the round!\n", winner_index + 1);
    
    // Move cards to the winner's deck
    for (int i = 0; i < num_players; i++) 
    {
        if (i != winner_index && players[i].num_cards > 0) 
        {
            players[winner_index].deck[players[winner_index].num_cards++] = players[i].deck[0];
            players[i].num_cards--;
            
            // Shift remaining cards in loser's deck
            for (int j = 0; j < players[i].num_cards; j++) 
            {
                players[i].deck[j] = players[i].deck[j + 1];
            }
        }
    }
}

int main() 
{
    srand(time(NULL)); // Seed for random number generation
    printf("Welcome to the bachelor card game!\n");
    Player players[NUM_PLAYERS];
    
    // Initialize players' decks with cards
    for (int i = 0; i < NUM_PLAYERS; i++) 
    {
        players[i].deck = (Card *)malloc(NUM_CARDS / NUM_PLAYERS * sizeof(Card));
        initialize_deck(players[i].deck, NUM_CARDS / NUM_PLAYERS, NUM_ATTRIBUTES);
        players[i].num_cards = NUM_CARDS / NUM_PLAYERS;
        strcpy(players[i].type, "Bachelor");
    }
    
    int current_player = 0;
    int round = 1;
    
    while (1) 
    {
        
        printf("Round %d\n", round++);
        
        // Display each player's top card
        for (int i = 0; i < NUM_PLAYERS; i++) 
        {
            printf("Player %d: Card with attributes: ", i + 1);
            for (int j = 0; j < players[i].deck[0].num_attributes; j++) 
            {
                printf("%s = %d ", players[i].deck[0].attributes[j].name, players[i].deck[0].attributes[j].value);
            }
            printf("\n");
        }
        
        // Prompt current player to choose an attribute
        int chosen_attribute;
        printf("Player %d, choose an attribute to play with (0 to %d): ", current_player + 1, NUM_ATTRIBUTES - 1);
        scanf("%d", &chosen_attribute);
        
        // Play the round
        play_round(players, NUM_PLAYERS, chosen_attribute);
        
        // Check if the game is over
        int remaining_players = 0;
        int winner_index = -1;
        for (int i = 0; i < NUM_PLAYERS; i++) 
        {
            if (players[i].num_cards > 0) 
            {
                remaining_players++;
                winner_index = i;
            }
        }
        if (remaining_players == 1) 
        {
            printf("Player %d wins the game!\n", winner_index + 1);
            break;
        }
        
        // Move to the next player
        current_player = (current_player + 1) % NUM_PLAYERS;
    }
    
    // Free dynamically allocated memory
    for (int i = 0; i < NUM_PLAYERS; i++) 
    {
        free(players[i].deck);
    }
    
    return 0;
}
