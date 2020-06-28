#define Export __declspec(dllexport)

#include "win32api.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>

// Adapted from: https://stackoverflow.com/questions/67273/how-do-you-iterate-through-every-file-directory-recursively-in-standard-c#answer-67336
// Not using wstring_t (WIN32_FIND_DATA, FindFirstFile, FindNextFile, wcscmp)

Export
int win32_get_directory_files(char **dest, char *dir, char *mask) {
    int ctr = 0;
    
    HANDLE handle = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAA find;
    char spec[100] = "";
    sprintf(spec, "%s\\%s", dir, mask);

    handle = FindFirstFileA(spec, &find);
    if(handle == INVALID_HANDLE_VALUE) {
        printf("Spec: %s\n", spec);
        return -1;
    }

    do {
        if(strcmp(find.cFileName, ".") != 0 && strcmp(find.cFileName, "..") != 0) {
            if((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
                if(ctr < MAX_DIR_FILES) {
                    dest[ctr] = malloc(sizeof(char) * MAX_FILENAME_LENGTH);
                    strcpy(dest[ctr], find.cFileName);
                    ++ctr;
                }
            }
        }
    } while(FindNextFileA(handle, &find) != 0);

    FindClose(handle);

    return ctr;
}

Export
void win32_free_directory_list(char **src, int ctr)
{
    for(int i = 0; i < ctr; ++i) {
        free(src[i]);
    }
}

Export
double win32_timer_frequency()
{
    unsigned __int64 freq;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    return (1.0/freq);
}

Export
unsigned __int64 win32_query_performance_counter()
{
    unsigned __int64 time;
    QueryPerformanceCounter((LARGE_INTEGER *)&time);
    return time;
}