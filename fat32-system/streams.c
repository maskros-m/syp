#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "streams.h"

// Working until -put now. Mustinitialize a value for Dentry.isDir when writing it to the Fat System, else error

int main(int argc, char *argv[]) {
    BOOTSECTOR bootsector = {"3AKIFT", DEFAULT_BLOCK_SIZE, MAX_BLOCK_COUNT};
    FAT_SYSTEM System = {0};

    char *fileName = NULL;

    // utilization: ./streams -create test.bin
    for (int i=0; i<argc; i++) {
        if (strcmp(argv[i], "-create") ==0) {
            fileName = argv[i+1];
            FILE *fd = fopen(fileName, "wb");

            // set total size of mockup fat system
            // total size = size of all memory blocks plus header
            // that contains metadata about the fat system structure
            int totalSize = sizeof(FAT_SYSTEM) + bootsector.blocksize * bootsector.blockcount;

            fwrite(&bootsector, 1, sizeof(BOOTSECTOR), fd);
            fwrite(&System.fat, 1, sizeof(FAT), fd);
            fwrite(&System.rootDir, 1, sizeof(DIR), fd);

            void *ptr = malloc(bootsector.blocksize * bootsector.blockcount);

            if (!ptr)
                return 1;
            // set all values contained in newly created block to 0 with memset,
            // make sure each element is exactly 1 byte in size with fwrite
            memset((void*)ptr, 0, bootsector.blocksize * bootsector.blockcount);
            fwrite((void*)ptr, 1, bootsector.blocksize * bootsector.blockcount, fd);

            int cursorPosition = ftell(fd);
            printf("Creating [%s] with [%d] bytes ... cursor position = [%d]\n",
                    fileName, totalSize, cursorPosition);
            fclose(fd);
        } // if

        // utilization: streams -partition test.bin
        if (strcmp(argv[i], "-partition") == 0) {
            fileName = argv[i+1];
            readPartitionMetadata(fileName, &System);
        }

        // streams -stats {partitionName}
        if (strcmp(argv[i], "-stats") == 0) {
            fileName = argv[i+1];
            printStats(fileName);
        }

        // streams -partition {partitionName} -add {userfile}
        if (strcmp(argv[i], "-add")==0) {
            isFilenameProvided(fileName, "-partition");
            char *userFile = argv[i+1];
            isFilenameProvided(userFile, "-add");

            DIR* d = &(System.rootDir);
            DENTRY* dirEntry = System.rootDir.dirEntries;

            for (int i=0; i < MAX_DIR_ENTRIES; i++) {
                if (d->dirEntries[i].filename[0] == '\0') {
                    
                    int _fileSize = dirEntry[i].size = fileSize(userFile);
                    int blockCnt = getNumberOfBlocksTaken(_fileSize, bootsector.blocksize);
                    int _freeBlocks = getNumberOfFreeBlocks(&System);
                    
                    printf("Attempting to add [%s] to partition [%s] size=[%d]\n", userFile, fileName, _fileSize);
                    printf("# blocks = [%d] free = [%d]\n", blockCnt, _freeBlocks);

                    if (blockCnt <= _freeBlocks) {
                        addNewDentry(userFile, _fileSize, d, i, 0);
                        TrackedBlocks freebls;
                        getArrayWithFreeBlocks(&System, _fileSize, &freebls);
                        writeFiletoPartition(&System, userFile, fileName, &freebls);
                        addUsedBlocksToFAT(&System, &freebls, i);
                        overwriteOldMetadata(fileName, &System);

                        free(freebls.arr);
                    } // if (blockCnt <= _freeBlocks)
                    break;
                }
            }

        }

        // streams -partition {partitionName} -show {userfile}
        if (strcmp(argv[i], "-show")==0) {
            isFilenameProvided(fileName, "-partition");
            char* userFile = argv[i+1];
            isFilenameProvided(userFile, "-show");

            TrackedBlocks usedBlocks;
            findOccupiedBlocks(&System, userFile, &usedBlocks);
            printBlockArray(usedBlocks.arr, usedBlocks.cnt);

            free(usedBlocks.arr);
        } // -show

        // streams -partition test.bin -put laugh.png
        if (strcmp(argv[i], "-put")==0) { 
            isFilenameProvided(fileName, "-partition");
            char* userFile = argv[i+1];
            isFilenameProvided(userFile, "-put");

            writeFileFromPartition (fileName, userFile, &System);
        } // -put

        // streams -partition test.bin -dir
        if (strcmp(argv[i], "-dir")==0) {
            isFilenameProvided(fileName, "-partition");
            char* userDir = argv[i+1];
            // isFilenameProvided(userDir, "-dir");
            char path[255];
            if (userDir==NULL || strcmp(userDir, ".")==0) {
                strcpy(path, fileName);
            } else { strcpy(path, userDir); }
            printf("Partition: [%s]\n\n", path);

            DENTRY* dirEntry = System.rootDir.dirEntries;
            printf("%10s %20s %-10s\n", "EXT", "FILESIZE", "FILENAME");
            printf("%.*s\n", 60, "-------------------------------------------");
            
            for (int i=0; i<MAX_DIR_ENTRIES; i++) {
                if (dirEntry[i].filename[0]) {
                    char extension[16] = "";
                    if (dirEntry[i].isDir) { strcpy(extension, "<DIR>"); }
                    else { strcpy(extension, ""); }
                    printf("%-10s %20d %-10s\n", extension, dirEntry[i].size, dirEntry[i].filename);
                }
            }
        } // -dir

        // streams -partition test.bin -mkdir
        if (strcmp(argv[i], "-mkdir")==0) {
            // added last night before bed
            isFilenameProvided(fileName, "-partition");
            char* userDir = argv[i+1];
            isFilenameProvided(userDir, "-dir");

            DENTRY newDirEntries[MAX_DIR_ENTRIES] = {0};
            printf("%ld\n", sizeof(newDirEntries));
        }

        if (strcmp(argv[i], "-del")==0) {
            isFilenameProvided(fileName, "-partition");
            char* fileToDel = argv[i+1];
            isFilenameProvided(fileToDel, "-del");

            // look for fileToDel in dirEntries to delete
            for (int i=0; i<MAX_DIR_ENTRIES; i++) {
                if (strcmp(System.rootDir.dirEntries[i].filename, fileToDel)==0) {
                    TrackedBlocks usedBlocks;
                    findOccupiedBlocks(&System, fileToDel, &usedBlocks);
                    printBlockArray(usedBlocks.arr, usedBlocks.cnt);

                    deleteFromPartition(fileName, &usedBlocks, &System);
                    deleteFromFATable(&System, &usedBlocks);

                    // TrackedBlocks doubleCheck;
                    // findOccupiedBlocks(&System, fileToDel, &doubleCheck);
                    // printBlockArray(doubleCheck.arr, doubleCheck.cnt);
                    
                    // delete from dirEntries (in rootDir)
                    DENTRY* dirEntry = &System.rootDir.dirEntries[i];
                    resetDirEntry (dirEntry);

                    printf("Entry [%s] at dirEntries index [%d] removed\n", fileToDel, i);

                    // update metadata in partition file
                    overwriteOldMetadata(fileName, &System);
                }
            }            

        } // -del

        if (strcmp(argv[i], "-empty")==0) {            

        }
    } // for
    return 0;
}

// Functions to update partition file & temp Fat file
void readPartitionMetadata(char *fileName, FAT_SYSTEM* System) {
    FILE* fp = fopen(fileName, "rb");
    checkIfFileExists(fp);
    fread(System, 1, sizeof(FAT_SYSTEM), fp);
    fclose(fp);
}

void overwriteOldMetadata(char *fileName, FAT_SYSTEM* System) {
    FILE* fp = fopen(fileName, "rb+");
    checkIfFileExists(fp);
    fwrite(System, 1, sizeof(FAT_SYSTEM), fp);
    fclose(fp);
}

// Functions to modify partition
void addNewDentry(char* userFile, int fileSize, DIR *d, int index, int isDir) {
    strcpy(&(d->dirEntries[index].filename[0]), userFile);
    d->dirEntries[index].size = fileSize;
    d->dirEntries[index].isDir = isDir;
}

void addUsedBlocksToFAT(FAT_SYSTEM *System, TrackedBlocks *freebls, int freeIndex) {
    printf("Blocks used: \n");
    DENTRY* dirEntry = System->rootDir.dirEntries;
    
    // freebls.cnt refers to size of array inside freeblocks struct
    for (int j=0; j< freebls->cnt; j++) {
        
        if (j==0) { // this is the first block
            // REMEMBER: dirEntry only records where each file starts
            // strcpy(dirEntry[i].filename, userFile);
            // dirEntry[i].size= _fileSize;
            // Fat.rootDir.dirEntries[i].size= _fileSize;
            printf("size = %d\n", dirEntry[freeIndex].size);
            dirEntry[freeIndex].firstblock = freebls->arr[j];
            printf("[%s]: size=[%d], firstblock=[%d]\n", dirEntry[freeIndex].filename
                    , dirEntry[freeIndex].size, dirEntry[freeIndex].firstblock);
        }
        if (j < freebls->cnt - 1) {
            System->fat.fatTable[freebls->arr[j]] = freebls->arr[j+1];
            printf("FAT[%d] points to block [%d]\n", freebls->arr[j], freebls->arr[j+1]);
        }
        else {
            System->fat.fatTable[freebls->arr[j]] = EOF;
        }
    } // for loop
    printf("\n");
}

void printStats(char *fileName) {
    FAT_SYSTEM System;
    FILE *fd = fopen(fileName, "rb");
    checkIfFileExists(fd);
    fread(&System, 1, sizeof(FAT_SYSTEM), fd);
    fclose(fd);

    printf("==================================\n");
    printf("partition: \t [%s]\n", fileName);
    printf("label: \t [%s]\n", System.bootsector.label);
    printf("# of blocks: \t [%d]\n", System.bootsector.blockcount);
    printf("block size: \t [%d]\n", System.bootsector.blocksize);
    printf("==================================\n");


    for (int i=0; i< MAX_DIR_ENTRIES; i++) {
        DENTRY* dirEntry = &System.rootDir.dirEntries[i];
        if (dirEntry->filename[0] == 0)
            continue;
        printf("[%d] => [%s] size = [%d] First Logical Cluster = [%d]\n", i
                , dirEntry->filename, dirEntry->size, dirEntry->firstblock);
    }
}

int fileSize(char *fileName) {
    FILE* fd = fopen(fileName, "rb");
    fseek(fd, 0, SEEK_END); // parse from beginning until the end of file
    // because each element is exactly 1 byte, the position of the pointer
    // which is end of file will be the same as file size
    int size = ftell(fd);
    fclose(fd);
    return size;
}

int getNumberOfBlocksTaken (int filesize, int blocksize) {
    // if filesize < blocksize count will be 0
    int count = filesize / blocksize;
    if (filesize % blocksize != 0)
        return count + 1;
    else 
        return count;
}

int getNumberOfFreeBlocks(FAT_SYSTEM* System) {
    int free=0;
    printf("total block count: %d\n", System->bootsector.blockcount);
    for (int i=0; i<System->bootsector.blockcount; i++) {
        if (System->fat.fatTable[i] == '\0')
            free++;
    }
    return free;
}

int getArrayWithFreeBlocks(FAT_SYSTEM* System, int filesize, TrackedBlocks* freeblocks) {
    int _blockCount = getNumberOfBlocksTaken(filesize, System->bootsector.blocksize);
    freeblocks->cnt = _blockCount; // why record no. of block taken into TrackedBlocks freeblocks.cnt?
    int* blockArray = malloc(sizeof(int) * _blockCount);
    freeblocks->arr = blockArray;

    for (int i = 0, j=0; i<System->bootsector.blockcount;i++) {
        // register starting block of newly added file into block array
        if (System->fat.fatTable[i] == '\0') { blockArray[j++] = i; }
        // j starts from index 0, so j = _blockCount - 1
        if (j >= _blockCount) break;
    }
    printf("_blockCount is [%d]\n", _blockCount);
    printBlockArray(blockArray, _blockCount);
    // printBlockArray(freeblocks->arr, _blockCount);
    return 0;
}

int findOccupiedBlocks(FAT_SYSTEM *System, char* userFile, TrackedBlocks* usedBlocks) {

    DENTRY* dirEntry = System->rootDir.dirEntries;
    int* FatEntry = System->fat.fatTable;
    int index = 0;
    for (int i=0; i<MAX_DIR_ENTRIES;i++) {
        printf("[%d] => %s, size: %d\n", i, dirEntry[i].filename, dirEntry[i].size);
        if (strcmp(userFile, dirEntry[i].filename)==0) {
            usedBlocks->cnt = getNumberOfBlocksTaken(dirEntry[i].size, System->bootsector.blocksize);
            index = i;
            break; // find only first file with that name
        } 
        // else { 
        //     puts("No such file in partition\n");
        //     exit(-1);
        // }
    }
    int* blockArray = malloc(sizeof(int) * usedBlocks->cnt);
    usedBlocks->arr = blockArray;
    int j = 0;
    blockArray[j++] = dirEntry[index].firstblock;
    int fatIndex = dirEntry[index].firstblock;
    int nextIndex = 0;
    while (FatEntry[fatIndex] != EOF) {
        blockArray[j++] = FatEntry[fatIndex];
        fatIndex = FatEntry[fatIndex];
        printf("j=[%d]  fatIndex=[%d]  FatEntry[fatIndex]=[%d]\n", j, fatIndex, FatEntry[fatIndex]);
    }
    return 0;
    // return dirEntry[index].size; // returns size of file
}

void printBlockArray (int* arr, int size) {
    for (int i=0; i< size; i++)
        printf("%d -> at block=[%d]\n", i, arr[i]);
}

int writeFiletoPartition(FAT_SYSTEM* System, char* fileName, char* partitionName, TrackedBlocks* freebls) {
    unsigned char* buffer = malloc(System->bootsector.blocksize);

    FILE* userFilePtr = fopen(fileName, "rb");
    FILE* partitionPtr = fopen(partitionName, "rb+");
    int wCnt;
    for (int i=0; i<freebls->cnt; i++) {
        // read a single block and copy its content into buffer array
        int rCnt = fread(buffer, 1, System->bootsector.blocksize, userFilePtr);
        // calculate where to write this block on the partition
        // the partition contains a header with the size = size of FAT_SYSTEM,
        // so this block has to be skipped
        // offset is the keyword!!
        int offset = sizeof(FAT_SYSTEM) + System->bootsector.blocksize * freebls->arr[i];
        fseek(partitionPtr, offset, SEEK_SET);
        // copy buffer's content into partition (e.g. test.bin) = fwrite
        wCnt = fwrite(buffer, 1, System->bootsector.blocksize, partitionPtr);
        // TODO: update FAT
    }
    free(buffer);
    fclose(userFilePtr);
    fclose(partitionPtr);
}

int writeFileFromPartition (char* partitionName, char* userFile, FAT_SYSTEM* System) {
    char destinationPath[255] = "copiedFrom_";
    prepareOutputFilePath(destinationPath, partitionName, userFile);

    int singleBlocksize = System->bootsector.blocksize;
    TrackedBlocks usedBlocks;
    findOccupiedBlocks(System, userFile, &usedBlocks);
    // printBlockArray

    FILE *destinationFile = fopen(destinationPath, "wb+");
    FILE *fp = fopen(partitionName, "rb");

    char* buffer = (char*) malloc(singleBlocksize);
    int byteCount = 0;
    for (int i=0; i<usedBlocks.cnt; i++) { 
        int offset = sizeof(FAT_SYSTEM) + singleBlocksize * usedBlocks.arr[i];
        printf("Index [%d] block no: %d => offset %d\n", i, usedBlocks.arr[i], offset);
        fseek(fp, offset, SEEK_SET);
        
        fread(buffer, 1, singleBlocksize, fp);
        byteCount += fwrite(buffer, 1, singleBlocksize, destinationFile);
    }

    free(buffer);
    free(usedBlocks.arr);
    fclose(destinationFile);
    fclose(fp);
    return byteCount;
}

// void checkIfFileExists(FILE* fileName, char* fileType) {
//     if (fileName == NULL) {
//         if (strcmp(fileType, "partition")==0)
//             printf("Error: no such partition\n");
//         else if (strcmp(fileType, "file")==0)
//             printf("Error: no such file\n");
//         else if (strcmp(fileType, "dir")==0)
//             printf("Error: no such directory\n");
//         else { printf("Error: please specify a valid path\n"); }
//         exit(-1);
//     }
// }
void checkIfFileExists(FILE* fileName) {
    if (fileName == NULL) {
        printf("Error: no such partition\n");
        exit(-1);
    }
}

void isFilenameProvided(char* fileName, char* command) {
    if (fileName == NULL) {
        printf("Error: No file name near `%s` specified\n", command);
        exit(-1);
    }  
}

void prepareOutputFilePath(char* destinationPath, char* partitionName, char* userFile) {
    strcat(destinationPath, partitionName);
    strcat(destinationPath, "_");
    strcat(destinationPath, userFile);
    
    printf("destination: %s\n", destinationPath);
}

void resetDirEntry (DENTRY* dirEntry) {
    strcpy(dirEntry->filename, "\0");
    dirEntry->size = 0;
    dirEntry->firstblock = -1;
    dirEntry->isDir = 0;
}

void deleteFromFATable(FAT_SYSTEM* System, TrackedBlocks* usedBlocks) {
    int* FATable = System->fat.fatTable;
    int FATindex = 0;
    for (int i=0; i<usedBlocks->cnt; i++) {
        FATindex = usedBlocks->arr[i];
        printf("FATindex to be deleted = [%d], nextBlock value: [%d]\n", FATindex, FATable[FATindex]);
        FATable[FATindex] = 0;
    }
}

void deleteFromPartition(char* partitionName, TrackedBlocks* usedBlocks, FAT_SYSTEM* System) {
    FILE* fp = fopen(partitionName, "rb+");
    if (fp == NULL) {
        printf("Error: no such partition\n");
        exit(1);
    }
    int writtenBlocksize = System->bootsector.blocksize;
    size_t totalBytesOverwritten = 0;
    void* buffer = malloc(writtenBlocksize);
    memset((void*) buffer, 0, writtenBlocksize);
    for (int i=0; i<usedBlocks->cnt; i++) {
        int offset = sizeof(FAT_SYSTEM) + writtenBlocksize * usedBlocks->arr[i];
        printf("round [%d]\n", i);
        printf("sizeof FAT_SYSTEM=[%ld]\n", sizeof(FAT_SYSTEM));
        printf("sizeof writtenBlocksize=[%d]\n", writtenBlocksize);
        printf("value at usedBlocks array[%d]=[%d]", i, usedBlocks->arr[i]);
        printf("offset=[%d]\n", offset);
        fseek(fp, offset, SEEK_SET);
        printf("fp at %ld\n", ftell(fp));
        totalBytesOverwritten = fwrite((void*) buffer, 1, writtenBlocksize, fp);
        // printf("Overwritten bytes = [%ld]\n", totalBytesOverwritten);
    }
    free(buffer);
    fclose(fp);
}