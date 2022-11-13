/* 13 Oct 2022 */
#define _CRT_SECURE_NO_WARNINGS_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "streams.h"


int main (int argc, char* argv[]) {
    bootsector b = {"4AKIFT", DEDAULT_BLOCK_SIZE, MAX_BLOCK_COUNT};
    admin a = {0};

    char* fileName = NULL;

    // create a file 
    // utilisation: stream.c -create test.bin
    for (int i = 0; i < argc; i++)
    {
        if (strcmp("-create", argv[i])==0) 
        {
            // char* fileName = argv[i+1]; // later replaced by char* fileName = NULL
            fileName = argv[i+1];
            // create a new binary file, erase old one if already exists
            FILE* fd = fopen(fileName, "wb");// fd = file directory
            int totalSize = sizeof(admin) + b.blocksize * b.blockcount;
            fwrite(&b, 1, sizeof(bootsector), fd);
            fwrite(&a.fat, 1, sizeof(fat), fd); // attention stream! continues from current cursor position in stream
            fwrite(&a.rootDir, 1, sizeof(dir), fd);
            void *pt = malloc(b.blocksize * b.blockcount);
            if (!pt) return 1;
            memset(pt,0,b.blocksize * b.blockcount); // write all bytes with 0
            fwrite(pt, 1, b.blocksize * b.blockcount, fd);
            int cursorPos = ftell(fd);

            printf("\ncreating [%s] with [%d] bytes ... cursor = [%d]\n", fileName, totalSize, cursorPos);
            fclose(fd);
        }

        // create a file ...
        // utilisation: streams -partition test.bin
        if ((strcmp("-partition", argv[i])==0))
        {
            fileName = argv[i+1];
            FILE *fd = fopen(fileName, "rb");
            fread(&a, 1, sizeof(admin), fd);
            fclose(fd);
        }
        if ((strcmp("-stats", argv[i])==0))
        {
            printStats(fileName);
        }

        // streams -partition {partname}
        if ((strcmp("-add", argv[i]) == 0))
        {
            if (fileName == NULL)
            {
                printf("\nError: no partition specified.\n");
                exit(-1);
            }
            char* userFile = argv[i+1];

            // search empty entry in root-dir ...
            printf("\ntesting\n");
            dir *d = &a.rootDir;
            for (int i =0; i<MAX_DIR_ENTRIES; i++)
            {
                if (d->entries[i].filename[0] == '\0') // yippe, found one ...
                {
                    // printf("found one\n");
                    strcpy(&(d->entries[i].filename[0]), userFile);
                    // printf("filesize of userFile is: %zu\n", fileSize(userFile));
                    // d->entries[i].size = fileSize(userFile);
                    int _fileSize = d->entries[i].size = fileSize(userFile);
                    int bCnt = calcNumberOfBlocks(d->entries[i].size, b.blocksize);
                    int _freeBlocks = getNumberOfFreeBlocks(&a);

                    printf("added %s to partition %s size = [%d] #blocks [%d] free=[%d]\n", 
                            userFile, fileName, d->entries[i].size, bCnt, _freeBlocks);

                    if (bCnt <= _freeBlocks) // cool, geht sich aus...
                    {
                        freeBlocks fb;
                        getArrayWithFreeBlocks(&a, _fileSize, &fb);
                        writeFileToPartition(&a, userFile, fileName, &fb);

                    }

                    break;
                }
            }
        }
    }
}

