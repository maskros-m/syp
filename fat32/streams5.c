#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "streams5.h"

// Implementation of subdirectories
// working until -rmdir (not yet implemented)

int main(int argc, char *argv[]) {
    BOOTSECTOR bootsector = {"3AKIFT", DEFAULT_BLOCK_SIZE, MAX_BLOCK_COUNT};
    FAT_SYSTEM System = {0};

    char *fileName = NULL;

    // utilization: ./streams -create test.bin
    for (int i=0; i<argc; i++) {
        if (strcmp(argv[i], "-create") ==0) {
            fileName = argv[i+1];
            FILE *fd = fopen(fileName, "wb");

            int totalSize = sizeof(FAT_SYSTEM) + bootsector.blocksize * bootsector.blockcount;

            fwrite(&bootsector, 1, sizeof(BOOTSECTOR), fd);
            fwrite(&System.fat, 1, sizeof(FAT), fd);
            fwrite(&System.rootDir, 1, sizeof(DIR), fd);

            void *ptr = malloc(bootsector.blocksize * bootsector.blockcount);

            if (!ptr)
                return 1;
            memset((void*)ptr, 0, bootsector.blocksize * bootsector.blockcount);
            fwrite((void*)ptr, 1, bootsector.blocksize * bootsector.blockcount, fd);

            int cursorPosition = ftell(fd);
            printf("Creating [%s] with [%d] bytes ... cursor position = [%d]\n",
                    fileName, totalSize, cursorPosition);
            free(ptr);
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
        // streams -partition {partitionName} -add {subDir/userfile}
        if (strcmp(argv[i], "-add")==0) {
            isFilenameProvided(fileName, "-partition");
            // ------ two lines below are old -add (for files, before subdir)
            char *userPath = argv[i+1];
            isFilenameProvided(userPath, "-add");

            DIR* d = &(System.rootDir);
            DENTRY* dirEntries = System.rootDir.dirEntries;

            /* subdir code added Nov 14 --start*/
            int pathLevel = countFilenameInPath(userPath) + 1; // make extra space for "root" @pathArray[0]
            // char** pathArray = malloc(sizeof(char*)*pathLevel);
            char** pathArray = calloc(pathLevel, sizeof(char*));
            char rootDir[] = "root";
            pathArray[0] = rootDir;

            extractPath(userPath, pathArray);
            FILE* userFile = fopen(pathArray[pathLevel-1], "rb");
            checkIfFileExists(userFile, "file");
            fclose(userFile);
            
            for (int i=0; i<pathLevel; i++) {
                printf("%s/", pathArray[i]);
            }
            printf("\n");
            int parentDirIndex = checkValidPath(dirEntries, pathArray, pathLevel);
            char parentDir[32];
            strcpy(parentDir, dirEntries[parentDirIndex].filename);
            if (pathLevel == 2 && strcmp(pathArray[0], "root") == 0)
            { strcpy(parentDir, "root"); }
            else { strcpy(parentDir, pathArray[pathLevel-2]); printf("%s\n", pathArray[pathLevel-2]); }
            printf("parentDir is [%s]\n", dirEntries[parentDirIndex].filename);

            /* --end*/

            for (int i=0; i < MAX_DIR_ENTRIES; i++) {
                if (d->dirEntries[i].filename[0] == '\0') {
                    
                    int _fileSize = fileSize(pathArray[pathLevel-1]);
                    int blockCnt = getNumberOfBlocksTaken(_fileSize, bootsector.blocksize);
                    int _freeBlocks = getNumberOfFreeBlocks(&System);

                    char* userFile = pathArray[pathLevel-1];
                    printf("Attempting to add [%s] to [%s] directory, size=[%d]\n", userFile, parentDir, _fileSize);
                    printf("# blocks = [%d] free = [%d]\n", blockCnt, _freeBlocks);

                    if (blockCnt <= _freeBlocks) {
                        addNewDentry(pathArray[pathLevel-1], _fileSize, parentDir, dirEntries, i, 0);
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
            free(pathArray);
        }

        // streams -partition {partitionName} -show {userfile}
        // streams -partition {partitionName} -show {subDir/userfile}
        if (strcmp(argv[i], "-show")==0) {
            isFilenameProvided(fileName, "-partition");
            char* userPath = argv[i+1];
            isFilenameProvided(userPath, "-show");

            DIR* d = &(System.rootDir);
            DENTRY* dirEntries = System.rootDir.dirEntries;

            int pathLevel = countFilenameInPath(userPath) + 1; // make extra space for "root" @pathArray[0]
            char** pathArray = calloc(pathLevel, sizeof(char*));
            char rootDir[] = "root";
            pathArray[0] = rootDir;

            extractPath(userPath, pathArray);
            char parentDir[32];
            char* ptrParent = parentDir;
            char* userFile = pathArray[pathLevel-1];
            findParentDir(ptrParent, dirEntries, pathArray, pathLevel);
            

            TrackedBlocks usedBlocks;
            newFindOccupiedBlocks(&System, parentDir, userFile, &usedBlocks);
            printBlockArray(usedBlocks.arr, usedBlocks.cnt);

            free(usedBlocks.arr);
            free(pathArray);
        } // -show

        // streams -partition test.bin -put laugh.png
        // streams -partition test.bin -put laugh.png laugh123.png
        // streams -partition {partitionName} -put {subDir/filename} {newfilename -optional-}
        if (strcmp(argv[i], "-put")==0) { 
            isFilenameProvided(fileName, "-partition");
            char* userPath = argv[i+1];
            isFilenameProvided(userPath, "-put");

            char* optionalRename = argv[i+2];

            DIR* d = &(System.rootDir);
            DENTRY* dirEntries = System.rootDir.dirEntries;

            int pathLevel = countFilenameInPath(userPath) + 1; // make extra space for "root" @pathArray[0]
            char** pathArray = calloc(pathLevel, sizeof(char*));
            char rootDir[] = "root";
            pathArray[0] = rootDir;

            extractPath(userPath, pathArray);
            char parentDir[32];
            char* ptrParent = parentDir;
            char* userFile = pathArray[pathLevel-1];
            findParentDir(ptrParent, dirEntries, pathArray, pathLevel);

            int fileExists = 0;
            for (int i=0; i<MAX_DIR_ENTRIES; i++) {
                if (strcmp(dirEntries[i].filename, userFile)==0
                    && !dirEntries[i].isDir
                    && strcmp(dirEntries[i].parentDir, parentDir)==0) {
                    writeFileFromPartition (fileName, parentDir, userFile, &System, optionalRename);
                    fileExists=1;
                    break;
                }
            }
            if (!fileExists) { 
                printf("File cannot be found in current directory\n");
                exit(1);
            }
            free(pathArray);
            
        } // -put

        // streams -partition test.bin -dir
        // streams -partition test.bin -dir ./subDir
        if (strcmp(argv[i], "-dir")==0) {
            isFilenameProvided(fileName, "-partition");
            char* userDirPath = argv[i+1];
            // TODO: check if partition exists

            DENTRY* dirEntry = System.rootDir.dirEntries;
            if (userDirPath==NULL || strcmp(userDirPath, ".")==0) {
                printFormattedDir(dirEntry, "root");
            } else { 
                int pathLevel = countFilenameInPath(userDirPath) + 1; // make extra space for "root" @pathArray[0]
                char** pathArray = calloc(pathLevel, sizeof(char*));
                char rootDir[] = "root";
                pathArray[0] = rootDir;
                extractPath(userDirPath, pathArray);
                for (int i=0; i<pathLevel; i++) {
                    if (pathArray[pathLevel-1] == NULL) { pathLevel--; }
                    printf("%s/", pathArray[i]);
                }
                printf("\n\n");
                int lastChildIndex = checkValidDirPath(dirEntry, pathArray, pathLevel);
                printFormattedDir(dirEntry, dirEntry[lastChildIndex].filename); 

                free(pathArray);
            }
        } // -dir

        // streams -partition test.bin -mkdir testdir
        // streams -partition test.bin -mkdir testdir/subDir
        if (strcmp(argv[i], "-mkdir")==0) {
            // added last night before bed
            isFilenameProvided(fileName, "-partition");
            char* userDir = argv[i+1];
            isFilenameProvided(userDir, "-mkdir");

            DIR* d = &(System.rootDir);
            DENTRY* dirEntry = System.rootDir.dirEntries;

            int pathLevel = countFilenameInPath(userDir) + 1;
            char** pathArray = calloc(pathLevel, sizeof(char*));
            char rootDir[] = "root";
            pathArray[0] = rootDir;

            extractPath(userDir, pathArray);
            int parentDirIndex = checkValidPath(dirEntry, pathArray, pathLevel);
            char parentDir[32];
            // strcpy(parentDir, dirEntries[parentDirIndex].filename);
            if (pathLevel == 2 && strcmp(pathArray[0], "root") == 0)
            { strcpy(parentDir, "root"); }
            else { strcpy(parentDir, pathArray[pathLevel-2]); printf("%s\n", pathArray[pathLevel-2]); }
            printf("parentDir is [%s]\n", dirEntry[parentDirIndex].filename);

            for (int i=0; i < MAX_DIR_ENTRIES; i++) {
                if (d->dirEntries[i].filename[0] == '\0') {

                    DENTRY newDirEntries[MAX_DIR_ENTRIES] = {0};
                    int dirSize = System.bootsector.blocksize;
                    int blockCnt = getNumberOfBlocksTaken(dirSize, bootsector.blocksize);
                    int freeBlocksLeft = getNumberOfFreeBlocks(&System);

                    printf("Attempting to create subfolder [%s] in directory [%s], size=[%d]\n", userDir, fileName, dirSize);
                    printf("# blocks = [%d] free blocks left = [%d]\n", blockCnt, freeBlocksLeft);

                    if (blockCnt < freeBlocksLeft) {
                        printf("printing from -mkdir\n");
                        printf("sizeof blocksize = [%d]\n", System.bootsector.blocksize);
                        addNewDentry(pathArray[pathLevel-1], dirSize, parentDir, dirEntry, i, 1);
                        TrackedBlocks freebls;
                        getArrayWithFreeBlocks(&System, dirSize, &freebls);
                        writeDirtoPartition(&System, fileName, &freebls);
                        addUsedBlocksToFAT(&System, &freebls, i);
                        overwriteOldMetadata(fileName, &System);

                        free(freebls.arr);
                    } // if blockCnt < freeBlocksLeft
                    break;
                } // if there's free space in dirEntries
            }// for
            free(pathArray);
        } // -mkdir

        // streams -partition test.bin -del {filename}
        // streams -partition {partitionName} -del {subDir/fileName}
        if (strcmp(argv[i], "-del")==0) {
            isFilenameProvided(fileName, "-partition");
            char *userPath = argv[i+1];
            // char* fileToDel = argv[i+1];
            isFilenameProvided(userPath, "-del");

            DENTRY* dirEntries = System.rootDir.dirEntries;
            
            int pathLevel = countFilenameInPath(userPath) + 1;
            char** pathArray = calloc(pathLevel, sizeof(char*));
            char rootDir[] = "root";
            pathArray[0] = rootDir;

            extractPath(userPath, pathArray);
            char parentDir[32];
            char* ptrParent = parentDir;
            char* fileToDel = pathArray[pathLevel-1];
            findParentDir(ptrParent, dirEntries, pathArray, pathLevel);

            int fileExists = 0;
            // look for fileToDel in dirEntries to delete
            for (int i=0; i<MAX_DIR_ENTRIES; i++) {
                if (strcmp(dirEntries[i].filename, fileToDel)==0
                    && strcmp(dirEntries[i].parentDir, pathArray[pathLevel-2])==0) {
                    if (dirEntries[i].isDir) {
                        puts("Cannot delete a directory. Please use -rmdir\n");
                        exit(-1);
                    }
                    else {
                        if (strcmp(dirEntries[i].parentDir, pathArray[pathLevel-2])==0) {
                            fileExists = 1;
                            printf("fileToDel: [%s], parentDir: [%s] [%s]\n", dirEntries[i].filename, dirEntries[i].parentDir, pathArray[pathLevel-2]);
                            TrackedBlocks usedBlocks;
                            newFindOccupiedBlocks(&System, parentDir, fileToDel, &usedBlocks);
                            // findOccupiedBlocks(&System, fileToDel, &usedBlocks);
                            printBlockArray(usedBlocks.arr, usedBlocks.cnt);
                            
                            // safe to implement -del for subdir
                            deleteFromPartition(fileName, &usedBlocks, &System);
                            deleteFromFATable(&System, &usedBlocks);

                            // TrackedBlocks doubleCheck;
                            // findOccupiedBlocks(&System, fileToDel, &doubleCheck);
                            // printBlockArray(doubleCheck.arr, doubleCheck.cnt);
                            
                            // delete from dirEntries (in rootDir)
                            DENTRY* dirEntry = &dirEntries[i];
                            resetDirEntry (dirEntry);


                            printf("Entry [%s] at dirEntries index [%d] removed\n", fileToDel, i);
                            overwriteOldMetadata(fileName, &System);
                            break;
                        }
                    }
                    
                }
            } // for

            if (!fileExists) { puts("no such file in partition\n"); }           

        } // -del

        if (strcmp(argv[i], "-rmdir")==0) {            
            // not yet implemented
        }
    } // for
    return 0;
}

// Functions to update partition file & temp Fat file
void readPartitionMetadata(char *fileName, FAT_SYSTEM* System) {
    FILE* fp = fopen(fileName, "rb");
    checkIfFileExists(fp, "partition");
    fread(System, 1, sizeof(FAT_SYSTEM), fp);
    fclose(fp);
}

void overwriteOldMetadata(char *fileName, FAT_SYSTEM* System) {
    FILE* fp = fopen(fileName, "rb+");
    checkIfFileExists(fp, "partition");
    fwrite(System, 1, sizeof(FAT_SYSTEM), fp);
    fclose(fp);
}

// Functions to modify partition
void addNewDentry(char* userFile, int fileSize, char* parentDir, DENTRY* dirEntries, int index, int isDir) {
    strcpy(dirEntries[index].filename, userFile);
    dirEntries[index].size = fileSize;
    dirEntries[index].isDir = isDir;
    strcpy(dirEntries[index].parentDir, parentDir);
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
    checkIfFileExists(fd, "partition");
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
    return 0;
}

int newFindOccupiedBlocks(FAT_SYSTEM *System, char* parentDir, char* userFile, TrackedBlocks* usedBlocks) {

    DENTRY* dirEntry = System->rootDir.dirEntries;
    int* FatEntry = System->fat.fatTable;
    int index = -1;
    for (int i=0; i<MAX_DIR_ENTRIES;i++) {
        if (strcmp(userFile, dirEntry[i].filename)==0
            && strcmp(dirEntry[i].parentDir, parentDir)==0) {
            usedBlocks->cnt = getNumberOfBlocksTaken(dirEntry[i].size, System->bootsector.blocksize);
            index = i;
            break; // find only first file with that name
        }
    }
    if (index == -1 ) { 
		puts("no such file in current directory"); 
		exit(-1);
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
}

int findOccupiedBlocks(FAT_SYSTEM *System, char* userFile, TrackedBlocks* usedBlocks) {
	DENTRY* dirEntry = System->rootDir.dirEntries;
	int* FatEntry = System->fat.fatTable;
	int index = 0;
	for (int i=0; i<MAX_DIR_ENTRIES;i++) {
		// printf("[%d] => %s, size: %d\n", i, dirEntry[i].filename, dirEntry[i].size);
		if (strcmp(userFile, dirEntry[i].filename)==0) {
			usedBlocks->cnt = getNumberOfBlocksTaken(dirEntry[i].size, System->bootsector.blocksize);
			index = i;
			break; // find only first file with that name
		}
		// else {
			// puts("No such file in partition\n");
			// exit(-1);
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
		printf("j=[%d] fatIndex=[%d] FatEntry[fatIndex]=[%d]\n", j, fatIndex, FatEntry[fatIndex]);
	}

	return 0;
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

        int rCnt = fread(buffer, 1, System->bootsector.blocksize, userFilePtr);
        int offset = sizeof(FAT_SYSTEM) + System->bootsector.blocksize * freebls->arr[i];
        fseek(partitionPtr, offset, SEEK_SET);
        wCnt = fwrite(buffer, 1, System->bootsector.blocksize, partitionPtr);
        // TODO: update FAT
    }
    free(buffer);
    fclose(userFilePtr);
    fclose(partitionPtr);
    return wCnt;
}

int writeDirtoPartition(FAT_SYSTEM* System, char* partitionName, TrackedBlocks* freebls) {
    
    unsigned char* dBuffer = malloc(System->bootsector.blocksize*freebls->cnt);
    memset(dBuffer, 0, System->bootsector.blocksize*freebls->cnt);

    FILE* partitionPtr = fopen(partitionName, "rb+");
    int wCnt = 0;
    for (int i=0; i<freebls->cnt; i++) {

        int offset = sizeof(FAT_SYSTEM) + System->bootsector.blocksize * freebls->arr[i];
        fseek(partitionPtr, offset, SEEK_SET);
        wCnt += fwrite(dBuffer, 1, System->bootsector.blocksize, partitionPtr);
        printf("written bytes to partition: %d\n", wCnt);
        // TODO: update FAT
    }
    free(dBuffer);
    fclose(partitionPtr);
    return wCnt;
}

int writeFileFromPartition (char* partitionName, char* parentDir, char* userFile, FAT_SYSTEM* System, char* optionalRename) {
    char destinationPath[255] = "copiedFrom_";
    if (optionalRename==NULL) {
        prepareOutputFilePath(destinationPath, partitionName, userFile);
     }
    else {
        strcpy(destinationPath, optionalRename);
    }
    // prepareOutputFilePath(destinationPath, partitionName, userFile);

    int singleBlocksize = System->bootsector.blocksize;
    TrackedBlocks usedBlocks;
    // findOccupiedBlocks(System, userFile, &usedBlocks);
    newFindOccupiedBlocks(System, parentDir, userFile, &usedBlocks);
    // printBlockArray

    FILE *destinationFile = fopen(destinationPath, "wb+");
    FILE *fp = fopen(partitionName, "rb");

    char* buffer = (char*) malloc(singleBlocksize);
    int byteCount = 0;
    for (int i=0; i<usedBlocks.cnt; i++) { 
        int offset = sizeof(FAT_SYSTEM) + singleBlocksize * usedBlocks.arr[i];
        // printf("Index [%d] block no: %d => offset %d\n", i, usedBlocks.arr[i], offset);
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

void checkIfFileExists(FILE* fileName, char* fileType) {
    if (fileName == NULL) {
        if (strcmp(fileType, "partition")==0)
            printf("Error: no such partition\n");
        else if (strcmp(fileType, "file")==0)
            printf("Error: no such file\n");
        else if (strcmp(fileType, "dir")==0)
            printf("Error: no such directory\n");
        else { printf("Error: please specify a valid path\n"); }
        exit(-1);
    }
}

// void checkIfFileExists(FILE* fileName) {
//     if (fileName == NULL) {
//         printf("Error: no such partition\n");
//         exit(-1);
//     }
// }

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
    dirEntry->filename[0] = '\0';
    dirEntry->size = 0;
    dirEntry->firstblock = -1;
    dirEntry->isDir = 0;
    dirEntry->parentDir[0] = '\0';
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

void printFormattedDir(DENTRY* dirEntry, char* dirName) {
    printf("%10s %20s %-10s\n", "EXT", "FILESIZE", "FILENAME");
    printf("%.*s\n", 60, "-------------------------------------------");
    
    for (int i=0; i<MAX_DIR_ENTRIES; i++) {
        if (dirEntry[i].filename[0] 
            && strcmp(dirEntry[i].parentDir, dirName)==0) {
            char extension[16] = "";
            if (dirEntry[i].isDir) { 
                strcpy(extension, "<DIR>"); 
                printf("%10s %20s %-10s\n", extension, "", dirEntry[i].filename);
            }
            else { 
                strcpy(extension, ""); 
                printf("%10s %20d %-10s\n", extension, dirEntry[i].size, dirEntry[i].filename);
            }
        }
    }
}

// Processing path with subdirectories
int countFilenameInPath(char *path) {
    char* pathCopy = strdup(path);
    int count = 0;
    char* token;
    token = strtok(pathCopy, "/");
    
    while(token !=NULL) {
        count++;
        token = strtok(NULL, "/");
    }
    free(pathCopy);
    return count;
}

void extractPath (char* originalPath, char** buffer) {
	// takes a path and returns split path by a delimiter
	// stores return values in an external buffer

	char *fileName = strtok(originalPath, "/");
	int i = 1;
	while (fileName !=NULL) {
        if (strcmp(fileName, ".")==0) { }
		else {
            buffer[i++] = fileName;
        }
        fileName = strtok(NULL, "/");
	}
}

int checkValidPath(DENTRY* dirEntries, char** pathArray, int pathLevel) {
    // check if any part of the path exists
    // returns dirEntries index of the last subdirectory if path is fully valid
    int parentIndex = -1; // obviously invalid index number
    for (int j=1; j<pathLevel-1; j++){
        for (int i=0; i<MAX_DIR_ENTRIES; i++){
            if (i == MAX_DIR_ENTRIES-1 && strcmp(dirEntries[i].filename, pathArray[j])) {
                // if @ last index and filename still doesn't match
                printf("Error: directory [%s] doesn't exist in current directory\n", pathArray[j]);
                free(pathArray);
                exit(1);
            }
            if (strcmp(dirEntries[i].filename, pathArray[j])==0 && dirEntries[i].isDir) { // if subdir exists
                // printf("found [%s] in [%s]!\n", dirEntries[i].filename, dirEntries[i].parentDir);
                // dirEntries[i].filename
                parentIndex = i;
                break;
            }                    
        }
    }
    return parentIndex;
}

int checkValidDirPath(DENTRY* dirEntries, char** pathArray, int pathLevel) {
    // check if all dirs in path exist
    // returns dirEntries index of the last subdirectory if path is fully valid
    // TODO: checkValidDirPath & checkValidPath overlap for the most part
    // Except for upper limit used in outer for loop. Need to refactor
    int lastChildIndex = -1;
    for (int j=1;j<pathLevel; j++) {
        for (int i=0; i<MAX_DIR_ENTRIES; i++) {
            if (i==MAX_DIR_ENTRIES-1 && strcmp(dirEntries[i].filename, pathArray[j]) && lastChildIndex == -1) {
                printf("Error: directory [%s] doesn't exist in current directory\n", pathArray[j]);
                free(pathArray);
                exit(1);
            }
            if (strcmp(dirEntries[i].filename, pathArray[j])==0 
                && dirEntries[i].isDir
                && strcmp(dirEntries[i].parentDir, pathArray[j-1])==0) {
                    lastChildIndex = i;
                    break;
            }
        }
    }
    return lastChildIndex;
}

void findParentDir(char* parentDir, DENTRY* dirEntries, char** pathArray, int pathLevel) {
    int parentDirIndex = checkValidPath(dirEntries, pathArray, pathLevel);
    if (parentDirIndex != -1) {
        strcpy(parentDir, dirEntries[parentDirIndex].filename);
        // printf("parentDir is [%s]\n", dirEntries[parentDirIndex].filename);
    }
    else { }
    if (pathLevel == 2 && strcmp(pathArray[0], "root") == 0)
    { strcpy(parentDir, "root"); }
    else { 
        strcpy(parentDir, pathArray[pathLevel-2]); 
        // printf("%s\n", pathArray[pathLevel-2]); 
    }
    // printf("parentDirIndex is [%d]\n", parentDirIndex);
    
}
