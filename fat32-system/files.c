#include "files.h"

// boot sector = first sector (0) on the volume or disk.
// contains specific information about the rest of organisation of the file system
// including how many copies of the FAT tables are present, how big a sector is (blocksize)
// how many sectors in a cluster (blockcount)
struct bootsection {
	char volume_name[32];
	int blocksize;
	int blockcount;
};


struct direntry {
	char filename[64];
	int size;
	int firstblock;
};

// primary directory of a disk. Has a finite size
// for FAT12, 14 (sectors) *16 (directory entries) = 224 possible entries
struct root_directory {
	struct direntry dir[MAX_DIR_ENTRIES];
};

// two fat & fatr because there are typically two redundant copies of FAT
// table on disk for data security and recovery purposes.
struct FAT_HD {
	struct bootsection boot;
	int fat[DEFAULT_BLOCK_COUNT];
	int fatr[DEFAULT_BLOCK_COUNT]; //reserve
	struct root_directory root;
};

/*
* fat -create name
* fat -stats name
* 
*/

int main(int argc, char* argv[]) {

	char cwd[64];
	char* cpath = _getcwd(cwd, 64);
	printf("Current Working Directory: %s\n", cwd);


	for(int i = 0; i < argc; i++){

		if (strcmp(argv[i], "-create") == 0) {
			
			FILE* f;
			char *volname = argv[i + 1];
			struct FAT_HD fat = { {"3AKIFT",
										DEFAULT_BLOCK_SIZE,
										DEFAULT_BLOCK_COUNT},
										{0},
										{0},
										{0}
										};
			f = fopen(volname, "wb+");
			//write header to volume
			fwrite((void*) &fat, 1, sizeof(struct FAT_HD), f);
			//write date to volume
			/*char help = 0;
			for (int j = 0; j < fat.boot.blockcount * fat.boot.blocksize; j++) {
				fwrite((void*)&help, 1, sizeof(struct FAT_HD), f);
			}*/
			int size = fat.boot.blockcount * fat.boot.blocksize;
			void* pt = malloc(size);
			memset(pt, 0, size);
			fwrite(pt, 1, size, f);
			fclose(f);
			free(pt);
		}

		else if (strcmp(argv[i], "-stats") == 0) {

			FILE* f;
			char* volname = argv[i + 1];
			struct bootsection boot = { 0 }; // alles 0
			f = fopen(volname, "rb");
			fread((void*)&boot, 1, sizeof(struct FAT_HD), f);
			printf("===================================\n");
			printf("volume name: [%s]\n", boot.volume_name);
			printf("blocksize: [%d]\n", boot.blocksize);
			printf("blockcount: [%d]\n", boot.blockcount);
			printf("===================================\n");
			fclose(f);
		}
	}
	
	
	return 0;
}
