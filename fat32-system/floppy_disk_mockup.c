#define MAX_DIR_ENTRIES 64

typedef struct admin { // has size of 6952
    BOOTSECTOR bootsector;
    DIR rootDir;
    FAT fat;
} admin;

typedef struct DENTRY {
    char filename[32];
    int size;
    int firstblock;
    int isDir;
} DENTRY;

typedef struct dir {
    struct DENTRY dirEntries[MAX_DIR_ENTRIES];
} dir;

int main()
{
    admin a = {0};
    
    // create binary file "test.bin"
    FILE* fd = fopen("test.bin", "wb");
    int totalSize = 6952 + 4096 * 1024; // ~4.2Mb
    
    // these together make file header, total size = 6952
    fwrite(&a.bootsector, 1, sizeof(bootsector), fd);
    fwrite(&a.fat, 1, sizeof(fat), fd); 
    fwrite(&a.rootDir, 1, sizeof(dir), fd);
    
    void *pt = malloc(4096 * 1024);
    if (!pt) return 1;
    memset(pt,0,4096 * 1024); // write all bytes with 0
    fwrite(pt, 1, 4096 * 1024, fd);
    fclose(fd);
    
    
    // copy file (binary data) from host computer to "test.bin", e.g. an image "laugh.png"
    FILE *fd = fopen("test.bin", "rb");
    fread(&a, 1, sizeof(admin), fd);
    fclose(fd);
    
    dir *d = &a.rootDir; // contains an array "entries" of 64 elements of type struct DENTRY
        for (int i =0; i<MAX_DIR_ENTRIES; i++) {
                if (d->entries[i].filename[0] == '\0') { // if filename is empty
                    // start writing binary data to "test.bin"
                    unsigned char* buffer = malloc(4096);
    
                    FILE* userFilePtr = fopen("laugh.png", "rb");
                    FILE* partitionPtr = fopen("test.bin", "rb+");
 
                    for (int i=0; i<freebls->cnt; i++) {

                        fread(buffer, 1, System->bootsector.blocksize, userFilePtr);
                        int offset = sizeof(admin) + 4096 * freebls->arr[i];
                        fseek(partitionPtr, offset, SEEK_SET);
                        fwrite(buffer, 1, System->bootsector.blocksize, partitionPtr);

                }
            free(buffer);
            fclose(userFilePtr);
            fclose(partitionPtr);
        }
    return 0;
}
