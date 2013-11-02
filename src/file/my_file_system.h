
#ifndef _MY_FILE_SYSTEM_H
#define _MY_FILe_SYSTEM_H

#include "file_system.h"
#include "types.h"

#define BLOCK_SIZE 1024


// Ce sont les blocks qui vont contenir les données utiles.
struct BlockData
{
	u8 desc_type:1,
	used:1,
	directory:1,
	opened:1,
	rest:4;
	u8 name_length;
	u32 file_length;
	u32 next;
	u8 data[914];
} __attribute__((packed));


struct BlockLink
{
	u8 desc_type:1,
	used:1,
	rest:6;
	u8 reserved[3];
	u32 next;
	struct
	{
		u32 id;
		u32 adress;
	} link[127] __attribute__((packed));
}__attribute__((packed));


class MyFileSystem : public FileSystem
{
public:
	MyFileSystem();
	bool isGood();

	static int createFile(const char *filename);
	static void removeFile(int id);

	int getIdFromFilename(const char *filename) const;

private:
	int total_blocks;
	int total_map_blocks;
	// Toutes les tailles et les offsets sont en SIZE_BLOCKS octets
	int offset;
	int map_size;
};


#endif
