#pragma once

void SaveResult(const char* playerName,
    const char* result);

int LoadResults(char results[][64],
    int maxResults);

void ClearResults();