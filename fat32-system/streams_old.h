#pragma once
#ifndef STREAMS_H
#define STREAMS_H

#define DEDAULT_BLOCK_SIZE 4096
#define DEDAULT_BLOCK_COUNT 1024
struct bootsector {

    char label[32];
    int blocksize;
    int blockcount;
};

typedef struct bootsector bootsector;

typedef struct direntry {
    char filename[32];
    int size;
    int firstblock;
} direntry;

typedef struct dir {
    #define MAX_DIR_ENTRIES 64
    struct direntry entries[MAX_DIR_ENTRIES];
} dir;

typedef struct fat {
    #define MAX_BLOCK_COUNT 1024
    int f[MAX_BLOCK_COUNT];
} fat;

typedef struct admin {
    bootsector b;
    dir rootDir;
    fat fat;

} admin;

typedef struct freeBlocks {
    int *arr;
    int cnt;
} freeBlocks;

size_t fileSize(char* fileName)
{
    FILE* fd = fopen(fileName, "rb");
    if (fd) {
        fseek(fd, 0, SEEK_END); // seek to end ...
        size_t size = ftell(fd);
        fclose(fd);
        return size;
    }
    else {
        return -1;
    }
}

void printStats(char* fileName) {
    admin a;
    FILE *fd = fopen(fileName, "rb");
    fread(&a, 1, sizeof(admin), fd);
    fclose(fd);
    
    printf("\n\n===================================");
    printf("\npartition: \t[%s]", fileName);
    printf("\nlabel=\t\t[%s]", a.b.label);
    printf("\n# of blocks:\t[%d]", a.b.blockcount);
    printf("\nblock size:\t[%d]", a.b.blocksize);
    printf("\n===================================\n\n");

}

int calcNumberOfBlocks(int filesize, int blocksize) {
    int q = filesize / blocksize;
    if (filesize % blocksize != 0) {return q + 1; }
    else { return q; }
}

int getNumberOfFreeBlocks(admin* a) {
    int free = 0;
    for (int i = 0; i < a->b.blockcount; i++) {
        if (a->fat.f[i] == 0) free++;
    }
    return free;
}

void printBlockArray(int* arr, int size) {
    for (int i=0;i<size;i++) 
        printf("\n%d => block = [%d]", i, arr[i]);
}

int getArrayWithFreeBlocks(admin* a, int filesize, freeBlocks *fb) { // returns pointer to first element in array
    int _blockCount = calcNumberOfBlocks(filesize, a->b.blocksize); 
    fb->cnt = _blockCount; // return this info to the caller ...
    int *blockArray = malloc(sizeof(int)*_blockCount);
    fb->arr = blockArray;

    // let's seek some free blocks and collect the block numbers in an array
    for (int i = 0, j = 0; i < a-> b.blockcount; i++) {
        if (a->fat.f[i] == '\0') {
            blockArray[j++] = i;
        }
        if (j>=_blockCount) // that's enough...
            break;
    }
    printBlockArray(blockArray, _blockCount);
    // return blockArray;

}

/* 
this function reads the file to be integrated in our partition
block by block and writes those blocks to the previously calculated blocks
(FreeBlocks structure)
*/
int writeFileToPartition(admin *a, char *fileName, char* partName, freeBlocks *fb) {
    // open
    unsigned char *buffer = malloc(a->b.blocksize);

    FILE *fdUserFile = fopen(fileName, "rb");
    FILE *fdPart = fopen(partName, "rb");
    int wCnt;
    for (int i=0; i<fb->cnt; i++) {
        int rCnt = fread(buffer, 1, a->b.blocksize, fdUserFile); // read a single block; rCnt returns 4096 something
        // calculate where to write this block on the partition
        int offset = sizeof(admin) + a->b.blocksize * fb->arr[i];

        // set cursor position in partition file stream accordingly ...
        fseek(fdPart, offset, SEEK_SET); //offset bytes from Ursprung entfernt
        wCnt = fwrite(buffer, 1, a->b.blocksize, fdPart); // write content to partition file
        // don't forget the fat entry!!

    }
    free(buffer);
    fclose(fdUserFile);
    fclose(fdPart);

}
#endif
