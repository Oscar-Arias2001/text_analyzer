/* One of my goals in creating this program was to make the transition from volatility (data that disappears when the
program closes) to persistence (data that remains on the hard drive) by creating a ".txt" file to save the results of the
analyses performed. */
#include <stdio.h>
#include <string.h>
#include "cs50.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_WORD_LENGTH 10000

// In C, it is good practice for the name of the struct to describe what the object is.
typedef struct
{
  int vowels_amount;
  int digits_amount;
  int consonants_amount;
  int spaces_amount;
  int punctuation_amount;
  int words_amount;
  int sentences_amount;
  int longest_word_length;
  char *longest_word; // A buffer to physically store the longest word
  float coleman_liau_index;
} textInsights;

// Function prototypes
void trim(char *text);
char *getValidString(void);
bool isVowel(char character);
void updateLongestWord(char *text, textInsights *phrase, int startIndex, int amountOfChars);
bool updateCharsCount(int *amountOfChars);
void findLongestWord(char *text, textInsights *phrase);
float colemanLiauIndex(textInsights *phrase);
void finalizeTextItem(bool *insideTextItem, bool *hasValidContent, int *amountOfTextItems);
void analyzeText(char *user_text, textInsights *phrase);
/* Both the terminal and a text file are “streams.” The terminal is identified as “stdout,” so I've created
a single function that accepts a pointer of type FILE* to call it twice in `main` and avoid duplication
when using `printf` and `fprintf`. */
void printTextAnalysisReport(FILE *append_analysis_file, textInsights *phrase, char *user_text, double total_time_analysis, char *day_buffer_string, char *date_buffer_string);

int main(void)
{
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif

  printf("\n");
  printf(" _____                                                                                                          _____ \n");
  printf("( ___ )--------------------------------------------------------------------------------------------------------( ___ )\n");
  printf(" |   |                                                                                                          |   | \n");
  printf(" |   |  ████████╗███████╗██╗  ██╗████████╗     █████╗ ███╗   ██╗ █████╗ ██╗  ██╗   ██╗███████╗███████╗██████╗   |   | \n");
  printf(" |   |  ╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝    ██╔══██╗████╗  ██║██╔══██╗██║  ╚██╗ ██╔╝╚══███╔╝██╔════╝██╔══██╗  |   | \n");
  printf(" |   |     ██║   █████╗   ╚███╔╝    ██║       ███████║██╔██╗ ██║███████║██║   ╚████╔╝   ███╔╝ █████╗  ██████╔╝  |   | \n");
  printf(" |   |     ██║   ██╔══╝   ██╔██╗    ██║       ██╔══██║██║╚██╗██║██╔══██║██║    ╚██╔╝   ███╔╝  ██╔══╝  ██╔══██╗  |   | \n");
  printf(" |   |     ██║   ███████╗██╔╝ ██╗   ██║       ██║  ██║██║ ╚████║██║  ██║███████╗██║   ███████╗███████╗██║  ██║  |   | \n");
  printf(" |   |     ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝       ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚═╝   ╚══════╝╚══════╝╚═╝  ╚═╝  |   | \n");
  printf(" |___|                                                                                                          |___| \n");
  printf("(_____)--------------------------------------------------------------------------------------------------------(_____)\n");

  printf("\n[ Linguistic Analysis and Readability Tool ]\n");
  printf("-> Detailed text statistics.\n");
  printf("-> Active DoS security.\n");
  printf("-> Automatically saved to \"text_analysis_results.txt\".\n");
  printf("--------------------------------------------------------\n\n");

  char *user_text = getValidString();

  /* Declaring the `textInsights` `phrase` inside the `main` function and passing
  its address (`&phrase`) is the correct way to protect the data. */
  /* When a structure is initialized with {0}, the compiler sets the entire memory block occupied
  by the structure to zero, regardless of the data types it contains.*/
  textInsights phrase = {0};

  /* Number of seconds since January 1, 1970 midnight. the time_t data type is a synonym
  for a long integer and this one comes from time.h library */
  time_t now = time(NULL);

  // local_time is a pointer to the struct tm that contains a pieces of info of the date and time
  struct tm *local_time = localtime(&now);

  char day_buffer_string[100], date_buffer_string[100];

  /* The C library strftime() function is used to format the date and time as a string.
  It allows user to set up customized date and time representations which makes the function more valuable.
  This one returns the string length of the format we chose*/
  strftime(day_buffer_string, 100, "%A", local_time);
  strftime(date_buffer_string, 100, "%x", local_time);

  clock_t start = clock(); // For starting the counter of the analysis

  // In C, `printf` does not write directly to the screen. It writes to a memory “buffer” called `stdout`.
  printf("[We are analyzing the text. Please be patient");
  for (int number = 0; number < 3; number++)
  {
    printf(". ");
    fflush(stdout);
    sleep(1);
  }
  printf("]\n\n");

  // To make the analysis of text that was provided by the user.
  analyzeText(user_text, &phrase);

  clock_t end = clock(); // For finishing the counter of the analysis

  double total_time_analysis = (double)(end - start) / CLOCKS_PER_SEC;

  // Creating .txt file with the results of each analysis that was made it throughout the time
  // In append mode (or “a” mode), I'm creating a chronological log of the analysis
  FILE *append_analysis_file = fopen("text_analysis_results.txt", "a");

  /* This validation is performed because the operating system may deny permission to create a file
  (for example, if the disk is full or the folder is protected). */
  if (append_analysis_file == NULL)
  {
    printf("[FILE ERROR]: Could not open file.\n");
    return 1;
  }

  /* Fun fact: In C, “stdout” is a pointer of type FILE* that points to the terminal.
  That's why we can use the same function for both. Abstraction of streams, based on the
  understanding that a terminal and a file are the same thing to the processor.*/
  printTextAnalysisReport(stdout, &phrase, user_text, total_time_analysis, day_buffer_string, date_buffer_string);
  printTextAnalysisReport(append_analysis_file, &phrase, user_text, total_time_analysis, day_buffer_string, date_buffer_string);

  free(phrase.longest_word);

  /* If, by mistake, someone tried to use `phrase.longest_word` after freeing it,
  the program would crash unpredictably. By setting it to `NULL`, any subsequent
  access will cause a clear and immediate error, which is easier to debug. */
  phrase.longest_word = NULL;

  // Closing the file called append_analysis_file.txt
  fclose(append_analysis_file);
  return 0;
}

// Trim Function: to delete blank spaces at the beginning and end of the string. This function counts each “Letter -> Space” transition as a word, but if there is a “Space -> Space” transition, it does not count it, which makes it robust against multiple spaces.
void trim(char *text)
{
  // 1- If it's an empty string or if it's equal to NULL, do nothing with it and prevent the rest of the function from executing.
  if (text == NULL || text[0] == '\0')
    return;

  // 2- Identify the first and last character of the text with pointers that store their memory addresses.
  char *start = text, *end = &text[strlen(text) - 1];

  // 3- Move forward as long as there are blank spaces at the beginning. The isspace() function detects: space (‘ ’), tab (‘\t’), new line (‘\n’), etc.
  while (*start != '\0' && isspace(*start))
    start++;

  // 4- If we reach the end of the string, it means that it only contained spaces. In that case, we need to establish that the string will be empty.
  if (*start == '\0')
  {
    text[0] = '\0';
    return;
  }

  // 5- Backtrack while there are whitespace characters at the end. The isspace() function detects: space (‘ ’), tab (‘\t’), new line (‘\n’), etc.
  while (end > start && isspace(*end))
    end--;

  // 6- Cut the string and move it. We place the end of the string just after the last valid letter. This expression is similar to: end[1] = '\0'
  *(end + 1) = '\0';

  // 7- Move content to the top, memmove() is the tool we use in C to “push” or “relocate” data within the same memory block without breaking anything.
  memmove(text, start, strlen(start) + 1);
}

// To get a valid input from user. It's not allowed an empty string.
char *getValidString(void)
{
  char *phrase_string = NULL;

  do
  {
    phrase_string = get_string("Enter the text you want to analyze and press ENTER to start the analysis\n The text is = ");

    if (phrase_string == NULL)
    {
      printf("An error occurred while reading text input.\n");
      /* exit() terminates the program immediately, regardless of the current function call stack depth.
      Using exit(1) after detecting a NULL is the correct way to protect the program against unexpected memory errors. */
      exit(1);
    }

    printf("\n");

    trim(phrase_string);

    if (phrase_string[0] == '\0')
    {
      printf("\n***********************************************************\n");
      printf("*Please enter a valid string. Empty string is not allowed.*\n");
      printf("***********************************************************\n");
    }
  } while (phrase_string[0] == '\0');

  return phrase_string;
}

// To check if a character is a vowel.
bool isVowel(char character)
{
  switch (tolower((unsigned char)character))
  {
  case ('a'):
  case ('e'):
  case ('i'):
  case ('o'):
  case ('u'):
    return true;
  default:
    return false;
  }
}

/* Helper Function for findLongestWord.  Instead of terminating the program, an error code (e.g., -1) is returned,
and the main function would simply display a message to the user. */
bool updateCharsCount(int *amountOfChars)
{
  /* 10,000 isn't always used as the limit. The number is chosen arbitrarily based on the context,
  but setting a limit of 10,000 serves as a “sanity check”: no one is going to type a legitimate word
  that's 1 million characters long. It's an “insurmountable barrier”; for security reasons,
  we have to be strict. */
  /* In this case, false is returned to indicate that the counter will not be updated because
  the limit set in the program has been exceeded. */
  // Safety: Ensuring that the counter is >= MAX_WORD_LENGTH guarantees that it will never exceed the allowed range.
  if (*amountOfChars >= MAX_WORD_LENGTH)
    return false;

  (*amountOfChars)++;

  return true;
}

// Helper Function for findLongestWord
void updateLongestWord(char *text, textInsights *phrase, int startIndex, int amountOfChars)
{
  // To update the amount of chars of the longest word inside the struct
  phrase->longest_word_length = amountOfChars;

  // Free up the memory of the previous word, if it existed. Was there something there before? If the pointer isn't NULL, we free it because we no longer need that old word.
  if (phrase->longest_word != NULL)
    free(phrase->longest_word);

  phrase->longest_word = malloc(amountOfChars + 1);
  if (phrase->longest_word == NULL)
  {
    printf("Error: Memory allocation failed\n");
    exit(1);
  }

  /* The strncpy() function is used to safely copy a specific number (n)
  of characters from a source string to a destination string. */
  strncpy(phrase->longest_word, &text[startIndex], amountOfChars);
  phrase->longest_word[amountOfChars] = '\0'; // Securing the string closure.
}

// To encounter the longest word inside of a text
void findLongestWord(char *text, textInsights *phrase)
{
  // Declararing startIndex variable to store the first index of a specific word.
  // Declararing amountOfChars variable to store the amount of chars of a specific word.
  int startIndex = 0, amountOfChars = 0;
  bool insideWord = false, hasAlphaLetter = false;

  for (int index = 0; text[index] != '\0'; index++)
  {
    // Storing the current char of the string that's being evaluated
    char character_evaluated = (unsigned char)text[index];

    // Creating Boolean Flags to avoid repetetitive ctype.h function calls
    bool isAlphabetLetter = isalpha(character_evaluated);
    bool isBlank = isspace(character_evaluated);
    bool isNumber = isdigit(character_evaluated);
    bool isValidPunctuation = (ispunct(character_evaluated) && character_evaluated != '-' && character_evaluated != '\'');

    if (isBlank || isNumber || isValidPunctuation)
    {
      if (amountOfChars > phrase->longest_word_length && hasAlphaLetter)
        updateLongestWord(text, phrase, startIndex, amountOfChars);

      // Reset the flag states and variables when a space is encountered
      startIndex = 0, amountOfChars = 0;
      insideWord = false, hasAlphaLetter = false;
    }
    else
    {
      // If the current character is not a space, that means we are within a potential word
      insideWord = true;

      if (isAlphabetLetter && !hasAlphaLetter)
      {
        startIndex = index;
        hasAlphaLetter = true;
      }

      if (isAlphabetLetter)
      {
        if (!updateCharsCount(&amountOfChars))
        {
          /* “If something goes wrong, I report it and move on”.
          This is a result of the “last known valid state” effect. */
          printf("[SECURITY ERROR]: The maximum length for a single word has been exceeded. Potential DoS detected.\n");
          return;
        }
      }
      else if ((character_evaluated == '-' || character_evaluated == '\'') && hasAlphaLetter)
      {
        // We check that it is not the first character (index > 0). Ensuring that memory access only occurs if the index is valid.
        if (index > 0)
        {
          // Relative Indexing Technique
          bool isPrevAlpha = isalpha((unsigned char)text[index - 1]), isNextAlpha = isalpha((unsigned char)text[index + 1]);

          if (isPrevAlpha && isNextAlpha)
          {
            if (!updateCharsCount(&amountOfChars))
            {
              /* “If something goes wrong, I report it and move on”.
              This is a result of the “last known valid state” effect. */
              printf("[SECURITY ERROR]: The maximum length for a single word has been exceeded. Potential DoS detected.\n");
              return;
            }
          }
        }
      }
    }
  }

  /* Finally, if the string does not end with a space, we check to see if there is actually a word
  that contains at least one valid alphabetic character, so that we can add it to our counter.*/
  if (amountOfChars > phrase->longest_word_length && hasAlphaLetter)
    updateLongestWord(text, phrase, startIndex, amountOfChars);
}

// To calculate The Coleman-Liau readability index
float colemanLiauIndex(textInsights *phrase)
{
  float gradeReadingLevel = 0.0;

  if (phrase->words_amount > 0)
  {
    // Storing in amountOfLetters variable the amount of letters in the text
    float amountOfLetters = (float)(phrase->vowels_amount + phrase->consonants_amount);

    // Calculating the average number of letters and sentences per 100 words in the text.
    float averageLetters = (amountOfLetters / phrase->words_amount) * 100.0;
    float averageSentences = (phrase->sentences_amount / (float)phrase->words_amount) * 100.0;

    // 1- Coleman–Liau assigned the constants (0.0588 and 0.296) through data-based mapping—examining many texts and their readability levels.
    // 2- The constant of 15.8 at the end adjusts the score to align the result with U.S. grade levels. Without the constant, the formula would produce results that mismatch real-world readability levels.
    gradeReadingLevel = 0.0588 * averageLetters - 0.296 * averageSentences - 15.8;
  }

  return gradeReadingLevel;
}

// Helper Function For analyzeText(). In this context when I say text item this one could be a sentence or a word
void finalizeTextItem(bool *insideTextItem, bool *hasValidContent, int *amountOfTextItems)
{
  // If we came from a word and that word had at least one letter, then we should increment our counter by one.
  if (*insideTextItem && *hasValidContent)
    (*amountOfTextItems)++;

  // Reset the flag states when a space or end of sentence is encountered
  *insideTextItem = false, *hasValidContent = false;
}

// The “phrase” structure is passed by reference; any changes made to it by the functions are permanent.
void analyzeText(char *user_text, textInsights *phrase)
{
  int amountOfWords = 0, amountOfSentences = 0;
  bool insideWord = false, hasAlphaLetter = false;
  bool insideSentence = false, hasContent = false;

  for (int index = 0; user_text[index] != '\0'; index++)
  {
    // Storing the current char of the string that's being evaluated
    char character_evaluated = (unsigned char)user_text[index];

    // Creating Boolean Flags to avoid repetetitive ctype.h function calls
    bool isAlphabetLetter = isalpha(character_evaluated);
    bool isNumber = isdigit(character_evaluated);
    bool isBlank = isspace(character_evaluated);
    bool isEndSentence = (character_evaluated == '?' || character_evaluated == '.' || character_evaluated == '!');

    /* The flow of the character through the “analyzeText” function must be hierarchical;
    in other words, it must create a conditional structure that progressively eliminates possibilities. */
    if (isAlphabetLetter)
    {
      isVowel(character_evaluated) ? phrase->vowels_amount++ : phrase->consonants_amount++;

      // If the current character is not a space, that means we are within a potential word
      insideWord = true, hasAlphaLetter = true;

      // If the current character is not a ., ? or !, that means we are within a potential sentence
      insideSentence = true, hasContent = true;
    }
    else if (isNumber)
    {
      // To add one to digits_amount counter when we find a digit
      phrase->digits_amount++;

      // If the current character is not a ., ? or !, that means we are within a potential sentence
      insideSentence = true, hasContent = true;

      // If we came from a word and that word had at least one letter, then we should increment our counter by one.
      // After that we must reset the flag states when a space is encountered
      finalizeTextItem(&insideWord, &hasAlphaLetter, &amountOfWords);
    }
    else if (isBlank)
    {
      // To add one to spaces_amount counter when we find a space
      phrase->spaces_amount++;

      // If we came from a word and that word had at least one letter, then we should increment our counter by one.
      // After that we must reset the flag states when a space is encountered
      finalizeTextItem(&insideWord, &hasAlphaLetter, &amountOfWords);
    }
    else if (isEndSentence)
    {
      // To add one to punctuation_amount counter when we find a punctuation symbol
      phrase->punctuation_amount++;

      // “Wait, before I finish the sentence, let me see if I was typing a word”
      // If we came from a word and that word had at least one letter, then we should increment our counter by one.
      // After that we must reset the flag states when a space is encountered
      finalizeTextItem(&insideWord, &hasAlphaLetter, &amountOfWords);

      // If we came from a sentence and that sentence had at least one letter or digit, then we should increment our counter by one.
      // Reset the flag states when a ., ? or ! is encountered
      finalizeTextItem(&insideSentence, &hasContent, &amountOfSentences);
    }
    else if (character_evaluated > 32)
    {
      // To add one to punctuation_amount counter when we find a punctuation symbol
      phrase->punctuation_amount++;

      if (character_evaluated == '-' || character_evaluated == '\'')
      {
        /* We check that it is not the first character (index > 0). Ensuring that memory access only occurs if the index is valid.
        The `if (index > 0)` condition is essential in C to prevent a segmentation fault or reading garbage memory when attempting to access `text[-1]`. */
        if (index > 0)
        {
          // Relative Indexing Technique
          bool isPrevAlpha = isalpha((unsigned char)user_text[index - 1]), isNextAlpha = isalpha((unsigned char)user_text[index + 1]);

          // The key point is that the hyphen and apostrophe should not enclose the word if they are in the middle of letters, but they should still count as punctuation
          if (!(isPrevAlpha && isNextAlpha))
          {
            /* If we came from a word and that word had at least one letter, then we should increment our counter by one.
            After that we must reset the flag states when a punctuation (not - or ') is encountered */
            finalizeTextItem(&insideWord, &hasAlphaLetter, &amountOfWords);
          }
        }

        continue;
      }

      finalizeTextItem(&insideWord, &hasAlphaLetter, &amountOfWords);
    }
  }

  /* Finally, if the string does not end with a space, we check to see if there is actually a word
  that contains at least one valid alphabetic character, so that we can add it to our counter.*/
  if (insideWord && hasAlphaLetter)
    amountOfWords++;

  // To determine the current amount of words inside the text
  phrase->words_amount = amountOfWords;

  /* Finally, if the string does not end with a ., ? or !, we check to see if there is actually a sentence
  that contains at least one valid alphabetic character or digit, so that we can add it to our counter.*/
  if (insideSentence && hasContent)
    amountOfSentences++;

  // To determine the current amount of sentences inside the text
  phrase->sentences_amount = amountOfSentences;

  findLongestWord(user_text, phrase);

  // To calculate The Coleman-Liau readability index
  phrase->coleman_liau_index = colemanLiauIndex(phrase);
}

/* Both the terminal and a text file are “streams.” The terminal is identified as “stdout,” so I've created
a single function that accepts a pointer of type FILE* to call it twice in `main` and avoid duplication
when using `printf` and `fprintf`. */
void printTextAnalysisReport(FILE *append_analysis_file, textInsights *phrase, char *user_text, double total_time_analysis, char *day_buffer_string, char *date_buffer_string)
{
  int userTextLength = strlen(user_text), totalAlphabetChars = phrase->vowels_amount + phrase->consonants_amount;
  int colemanLiauIndex = round(phrase->coleman_liau_index);

  float charsPerWords = phrase->words_amount > 0 ? (float)totalAlphabetChars / phrase->words_amount : 0.0;
  float charsPerSentences = phrase->sentences_amount > 0 ? (float)totalAlphabetChars / phrase->sentences_amount : 0.0;
  float wordsPerSentences = phrase->sentences_amount > 0 ? (float)phrase->words_amount / phrase->sentences_amount : 0.0;

  fprintf(append_analysis_file, "===============================================================\n");
  fprintf(append_analysis_file, "                      TEXT ANALYZER REPORT                     \n");
  fprintf(append_analysis_file, "===============================================================\n");
  fprintf(append_analysis_file, "%-30s %s\n", "Analyzed Text:", user_text);
  // fprintf(append_analysis_file, "---------------------------------------------------------------\n");
  fprintf(append_analysis_file, "%-30s %s\n", "Day Of Analysis:", day_buffer_string);
  fprintf(append_analysis_file, "%-30s %s\n", "Date Of Analysis:", date_buffer_string);
  fprintf(append_analysis_file, "%-30s %fs\n", "Analysis Time:", total_time_analysis);
  fprintf(append_analysis_file, "---------------------------------------------------------------\n");

  fprintf(append_analysis_file, "%-30s %d\n", "Amount Of Words:", phrase->words_amount);
  fprintf(append_analysis_file, "%-30s %d\n", "Amount Of Vowels:", phrase->vowels_amount);
  fprintf(append_analysis_file, "%-30s %d\n", "Amount Of Consonants:", phrase->consonants_amount);
  fprintf(append_analysis_file, "%-30s %d\n", "Amount Of Spaces:", phrase->spaces_amount);
  fprintf(append_analysis_file, "%-30s %d\n", "Number Of Digits:", phrase->digits_amount);
  fprintf(append_analysis_file, "%-30s %d\n", "Number Of Punctuation:", phrase->punctuation_amount);
  fprintf(append_analysis_file, "%-30s %d\n", "Characters (With Spaces):", userTextLength);
  fprintf(append_analysis_file, "%-30s %d\n", "Characters (Without Spaces):", (userTextLength - phrase->spaces_amount));
  fprintf(append_analysis_file, "%-30s %.2f\n", "Characters Per Words:", charsPerWords);
  fprintf(append_analysis_file, "%-30s %d\n", "Alphabetic Characters:", totalAlphabetChars);
  fprintf(append_analysis_file, "%-30s %d\n", "Amount Of Sentences:", phrase->sentences_amount);
  fprintf(append_analysis_file, "%-30s %.2f\n", "Characters Per Sentences:", charsPerSentences);
  fprintf(append_analysis_file, "%-30s %.2f\n", "Words Per Sentences:", wordsPerSentences);
  fprintf(append_analysis_file, "%-30s %s with %i characters\n", "Longest Word:", phrase->longest_word, phrase->longest_word_length);

  if (colemanLiauIndex < 1)
  {
    fprintf(append_analysis_file, "%-30s Before Grade 1\n", "Coleman-Liau Index:");
  }
  else if (colemanLiauIndex >= 16)
  {
    fprintf(append_analysis_file, "%-30s Grade 16+\n", "Coleman-Liau Index:");
  }
  else
  {
    fprintf(append_analysis_file, "%-30s Grade %d\n", "Coleman-Liau Index:", colemanLiauIndex);
  }

  fprintf(append_analysis_file, "\n\n");
}
