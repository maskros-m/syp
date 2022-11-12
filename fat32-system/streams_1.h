#ifndef STREAMS_1_H
#define STREAMS_1_H
#pragma once

typedef struct BOOTSECTOR {
#define DEFAULT_BLOCK_SIZE 4096
    char label[32];
    int blocksize;
    int blockcount;
} BOOTSECTOR;

typedef struct DENTRY {
    char filename[32];
    int size;
    int firstblock;
} DENTRY;

typedef struct DIR {
#define MAX_DIR_ENTRIES 64
    struct DENTRY dirEntries[MAX_DIR_ENTRIES];
} DIR;

typedef struct FAT {
#define MAX_BLOCK_COUNT 1024
    int fatTable[MAX_BLOCK_COUNT];
} FAT;

typedef struct FAT_SYSTEM {
    BOOTSECTOR bootsector;
    DIR rootDir;
    FAT fat;
} FAT_SYSTEM;

typedef struct TrackedBlocks {
    int *arr;
    int cnt;
} TrackedBlocks;

// Functions to update partition file & temp Fat file
void readPartitionMetadata(char *fileName, FAT_SYSTEM* System);
void overwriteOldMetadata(char *fileName, FAT_SYSTEM* System);

// Functions to modify partition
void addNewDentry(char* userFile, int fileSize, DIR *d, int index);
void addUsedBlocksToFAT(FAT_SYSTEM *Fat, TrackedBlocks *freebls, int freeIndex);

void printStats(char *fileName);
int fileSize(char *fileName);
int getNumberOfBlocksTaken (int filesize, int blocksize);
int getNumberOfFreeBlocks(FAT_SYSTEM* System);
int getArrayWithFreeBlocks(FAT_SYSTEM* System, int filesize, TrackedBlocks* freeblocks);
int findOccupiedBlocks(FAT_SYSTEM *System, char* userFile, TrackedBlocks* usedBlocks);
void printBlockArray (int* arr, int size);
int writeFiletoPartition(FAT_SYSTEM* System, char* fileName, char* partitionName, TrackedBlocks* freebls);

void checkIfFileExists(FILE* fileName);
void isFilenameProvided(char* fileName, char* command);
int writeFileFromPartition (char* partitionName, char* userFile, FAT_SYSTEM* System);
void prepareOutputFilePath(char* destinationPath, char* partitionName, char* userFile);
#endif