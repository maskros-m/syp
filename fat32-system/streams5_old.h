#ifndef STREAMS5_OLD_H
#define STREAMS5_OLD_H
#pragma once

// implementation of subdirectories
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
    int isDir;
    char parentDir[32];
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

// Functions to work with file header
void readPartitionMetadata(char *fileName, FAT_SYSTEM* System);
void overwriteOldMetadata(char *fileName, FAT_SYSTEM* System);

// Functions to modify temp file header
void addNewDentry(char* userFile, int fileSize,char* parentDir, DENTRY* dirEntries, int index, int isDir);
void resetDirEntry (DENTRY* dirEntry);
void addUsedBlocksToFAT(FAT_SYSTEM *Fat, TrackedBlocks *freebls, int freeIndex);
void deleteFromFATable(FAT_SYSTEM* System, TrackedBlocks* usedBlocks);

void printStats(char *fileName);
int fileSize(char *fileName);
int getNumberOfBlocksTaken (int filesize, int blocksize);
int getNumberOfFreeBlocks(FAT_SYSTEM* System);
int getArrayWithFreeBlocks(FAT_SYSTEM* System, int filesize, TrackedBlocks* freeblocks);
int findOccupiedBlocks(FAT_SYSTEM *System, char* userFile, TrackedBlocks* usedBlocks);
void printBlockArray (int* arr, int size);

// Functions to modify partition
int writeFiletoPartition(FAT_SYSTEM* System, char* fileName, char* partitionName, TrackedBlocks* freebls);
int writeFileFromPartition (char* partitionName, char* userFile, FAT_SYSTEM* System);
void deleteFromPartition(char* partitionName, TrackedBlocks* usedBlocks, FAT_SYSTEM* System);
int writeDirtoPartition(FAT_SYSTEM* System, char* partitionName, TrackedBlocks* freebls);

// File management functions
// void checkIfFileExists(FILE* fileName);
void checkIfFileExists(FILE* fileName, char* fileType);
void isFilenameProvided(char* fileName, char* command);
void prepareOutputFilePath(char* destinationPath, char* partitionName, char* userFile);

// void printFormattedDir(DENTRY* dirEntry);
void printFormattedDir(DENTRY* dirEntry, char* dirName);

// Processing path with subdirectories
int countFilenameInPath(char *path);
void extractPath (char* originalPath, char** buffer);
int checkValidPath(DENTRY* dirEntries, char** pathArray, int pathLevel);
int checkValidDirPath(DENTRY* dirEntries, char** pathArray, int PathLevel);
int lastMatchingChildIndex(DENTRY* dirEntries, char** pathArray, int pathLevel, int startIndex);

#endif
