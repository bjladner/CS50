#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void sort_reduced_pairs(int length);
void lock_pairs(void);
bool create_cycle(int winner, int loser, int original_winner);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Search for the candidate's name in 'candidates[]'
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate is found, set the rank in 'ranks[]' and return true
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    // If no match, return false
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Initialize internal counter
    int j = 0;
    // Start with the next highest ranked candidate
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // Increment the preferences over lower ranked candidates
        for (j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Initialize internal counter
    int j = 0;
    // Start with the next highest ranked candidate
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // Go through all the lower ranked candidates
        for (j = i + 1; j < candidate_count; j++)
        {
            // Add winner and loser to pairs[]
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser  = j;
                // Increment the number of pairs
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser  = i;
                // Increment the number of pairs
                pair_count++;

            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    sort_reduced_pairs(pair_count);
}

// Recursive sorting function to sort pairs
void sort_reduced_pairs(int length)
{
    // Initialize variables to find next highest preference pairS
    int current_index = pair_count - length;
    int highest_pref_index = current_index;
    int prefer_winner = preferences[pairs[current_index].winner][pairs[current_index].loser];
    int prefer_loser  = preferences[pairs[current_index].loser][pairs[current_index].winner];
    int highest_pref = prefer_winner - prefer_loser;
    pair temp_pair;
    // Loop through the remaining pairs
    for (int i = current_index + 1; i < pair_count; i++)
    {
        prefer_winner = preferences[pairs[i].winner][pairs[i].loser];
        prefer_loser  = preferences[pairs[i].loser][pairs[i].winner];
        // Find the highest preference pair remaining
        if ((prefer_winner - prefer_loser) > highest_pref)
        {
            highest_pref_index = i;
            highest_pref = prefer_winner - prefer_loser;
        }
    }
    // If it is not already in the right place, move it
    if (highest_pref_index != current_index)
    {
        temp_pair = pairs[current_index];
        pairs[current_index] = pairs[highest_pref_index];
        pairs[highest_pref_index] = temp_pair;
    }
    // If there are more than 2 pairs remaining, repeat this function
    if (length > 2)
    {
        length--;
        sort_reduced_pairs(length);
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // For each pair
    for (int i = 0; i < pair_count; i++)
    {
        // Check to see if creating the locked pair will create a cycle
        if (!create_cycle(pairs[i].winner, pairs[i].loser, pairs[i].winner))
        {
            // If not, then create the locked pair
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
bool create_cycle(int winner, int loser, int original_winner)
{
    // If the loser is the same as the original winner, then a loop has been formed and return true
    if (loser == original_winner)
    {
        return true;
    }
    //  Loop through each candidate for the looser and check for any wins
    for (int i = 0; i < MAX; i++)
    {
        // If there is a win ...
        if (locked[loser][i] == true)
        {
            // Check to see if the new winner, loser and original_winner creates a cycle
            if (create_cycle(loser, i, original_winner) == true)
            {
                // Continue to return true if a cycle has been created
                return true;
            }
        }
    }
    // If no more candidates to check, return false
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    bool found_loss;

    for (int i = 0; i < MAX; i++)
    {
        found_loss = false;
        for (int j = 0; j < MAX; j++)
        {
            // If there is a win ...
            if (locked[j][i] == true)
            {
                found_loss = true;
                break;
            }
        }
        if (!found_loss)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}

