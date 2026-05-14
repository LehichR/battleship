#include "results.h"

#include <stdio.h>
#include <string.h>

void SaveResult(const char* playerName,
    const char* result)
{
    FILE* file =
        fopen("results.txt", "a");

    if (!file)
    {
        return;
    }

    fprintf(file,
        "%s %s\n",
        playerName,
        result);

    fclose(file);
}

int LoadResults(char results[][64],
    int maxResults)
{
    FILE* file =
        fopen("results.txt", "r");

    if (!file)
    {
        return 0;
    }

    int count = 0;

    while (count < maxResults &&
        fgets(results[count],
            64,
            file))
    {
        results[count][strcspn(
            results[count],
            "\n")] = '\0';

        count++;
    }

    fclose(file);

    return count;
}

void ClearResults()
{
    FILE* file =
        fopen("results.txt", "w");

    if (!file)
    {
        return;
    }

    fclose(file);
}