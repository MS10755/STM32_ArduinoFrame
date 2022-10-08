#include "lfs.h"
#include "w25q128_user.h"


static int w25q128_read(const struct lfs_config *c, lfs_block_t block,lfs_off_t off, void *buffer, lfs_size_t size){
	if(w25q128_user_read(block * c->block_size + off,buffer,size)){
		return LFS_ERR_IO;
	}
	return LFS_ERR_OK;
}


static int w25q128_prog(const struct lfs_config *c, lfs_block_t block,lfs_off_t off, const void *buffer, lfs_size_t size){
	if(w25q128_user_write(block * c->block_size + off,buffer,size)){
		return LFS_ERR_IO;
	}
	return LFS_ERR_OK;
}


static int w25q128_erase(const struct lfs_config *c, lfs_block_t block){
	if(w25q128_user_erase_sector(block)){
		return LFS_ERR_IO;
	}
	return LFS_ERR_OK;
}


static int w25q128_sync(const struct lfs_config *c){
	return LFS_ERR_OK;
}


const struct lfs_config dev_w25q128_cfg = {
 // block device operations
    .read  = w25q128_read,
    .prog  = w25q128_prog,
    .erase = w25q128_erase,
    .sync  = w25q128_sync,

    // block device configuration
    .read_size = 16,
    .prog_size = 16,
    .block_size = 0x1000,
    .block_count = 0x1000,
    .cache_size = 16,
    .lookahead_size = 16,
    .block_cycles = 500,
};




