// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27 // 27 N für 26 Buchstaben und das ', jedes N ist ein Pfad in der node

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

void free_all(node *curs);

// Represents a trie
node *root;
int numWords = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // Increment the wordcount (needed in size function)
        numWords++;

        node *startNode = root;

        // get each letter of each word it insert into nodes
        for (int i = 0; i < strlen(word); i++)
        {
            int curLetter;

            // Check if the character is an ', if so assing it to curLetter 27 (last one)
            if (word[i] == 39)
            {
                curLetter = 26;
            }
            else
            {
                curLetter = word[i] - 97;
            }

            if (startNode->children[curLetter] == NULL)
            {
                // Create new Node
                struct node *newNode = malloc(sizeof(node));

                // Check if new node is Null
                if (newNode == NULL)
                {
                    unload();
                    return false;
                }

                newNode->is_word = false;

                // Set all children in newnode to null, prevent memory errors
                for (int k = 0; k < N; k++)
                {
                    newNode->children[k] = NULL;
                }

                startNode->children[curLetter] = newNode;

                // if end of word, new node is word
                if (i == strlen(word) - 1)
                {
                    newNode->is_word = true;
                }

                startNode = newNode;
            }
            else
            {
                startNode = startNode->children[curLetter];
            }
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return numWords;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *cursor = root;

    for (int i = 0; i < strlen(word); i++)
    {
        char lowerCase = tolower(word[i]);

        int curLetter;

        // Check if the character is an ', if so assing it to curLetter 27 (last one)
        if (word[i] == 39)
        {
            curLetter = 26;
        }
        else
        {
            curLetter = lowerCase - 97;
        }

        if (!cursor->children[curLetter])
        {
            return false;
        }
        else
        {
            cursor = cursor->children[curLetter];
        }

        if (i == strlen(word) - 1 && cursor->is_word == true)
        {
            return true;
        }
        else if (i == strlen(word) - 1 && cursor->is_word == false)
        {
            return false;
        }
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    free_all(root);
    return true;
}

// https://stackoverflow.com/questions/34691151/how-to-free-recursive-struct-trie
void free_all(node *curs)
{
    int i;
    if (!curs)
    {
        return;   // safe guard including root node.
    }

    // recursive case (go to end of trie)
    for (i = 0; i < 27; i++)
    {
        free_all(curs->children[i]);
    }

    // base case
    free(curs);
}