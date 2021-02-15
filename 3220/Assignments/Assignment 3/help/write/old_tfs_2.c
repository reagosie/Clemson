#include "tfs.h"
#include <stdlib.h>

/* implementation of assigned functions */

/* tfs_delete()
 * -deletes a closed directory entry having the given file descriptor (changes the status of the entry to unused) and releases all
 *   allocated file blocks
 * -preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is closed
 * -postconditions:
 *   (1) the status of the directory entry is set to unused
 *   (2) all file blocks have been set to free
 * -input parameter is a file descriptor
 * -return value is TRUE when successful or FALSE when failure
 */
unsigned int tfs_delete( unsigned int file_descriptor ){
  unsigned int fd;
  //what do I actually loop through? or do I even need to loop?
  //could I just do this:
    //directory[file_descriptor].status = 0;
  for(fd = 1; fd < N_DIRECTORY_ENTRIES; fd++){
    if(file_descriptor == fd){
      //delete directory (change status to unused)
      directory[fd].status = 0;
      //release all allocated file blocks
      for(int i = directory[fd].first_block; i < directory[fd].size; i++){
        free(&blocks[i]);
      }
      return(TRUE);
    }
  }
  return(FALSE);
}

/* tfs_read()
 * -reads a specified number of bytes from a file starting at the byte offset in the directory into the specified
 *   buffer; the byte offset in the directory entry is incremented by the number of bytes transferred
 * -depending on the starting byte offset and the specified number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 * -the function will read fewer bytes than specified if the end of the file is encountered before the specified number
 *   of bytes have been transferred
 * -preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *   (3) the file has allocated file blocks
 * -postconditions:
 *   (1) the buffer contains bytes transferred from file blocks
 *   (2) the specified number of bytes has been transferred except in the case that end of file was encountered
 *         before the transfer was complete
 * -input parameters are a file descriptor, the address of a buffer of bytes to transfer, and the count of bytes to
 *   transfer
 * -return value is the number of bytes transferred
 */
unsigned int tfs_read( unsigned int file_descriptor, char *buffer, unsigned int byte_count ){
  //still need to implement 1) ability to cross file blocks and 2) check for EOF
  unsigned int fd = file_descriptor;
  for(unsigned int i = directory[fd].byte_offset; i < byte_count; i++){
    if(blocks[fd].bytes[i]){ //check for EOF? aka no bytes left to read
      //read in bytes from file (bytes) to buffer
      buffer[i] = blocks[fd].bytes[i];
    }
  }

  directory[fd].byte_offset += byte_count;

  return byte_count;
}

/* tfs_write()
 * -writes a specified number of bytes from a specified buffer into a file starting at the byte offset in the directory;
 *   the byte offset in the directory entry is incremented by the number of bytes transferred
 * -depending on the starting byte offset and the specified number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 * -furthermore, depending on the starting byte offset and the specified number of bytes to transfer, additional file
 *   blocks, if available, will be added to the file as needed; in this case, the size of the file will be adjusted
 *   based on the number of bytes transferred beyond the original size of the file
 * -the function will read fewer bytes than specified if file blocks are not available
 * -preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 * -postconditions:
 *   (1) the file contains bytes transferred from the buffer
 *   (2) the specified number of bytes has been transferred except in the case that file blocks needed to
 *         complete the transfer were not available
 *   (3) the size of the file is increased as appropriate when transferred bytes extend beyond the previous
 *         end of the file
 * -input parameters are a file descriptor, the address of a buffer of bytes to transfer, and the count of bytes to
 *   transfer
 * -return value is the number of bytes transferred
 */
unsigned int tfs_write( unsigned int file_descriptor, char *buffer, unsigned int byte_count ){
  unsigned int fd = file_descriptor;
  unsigned int currBlock, lastBlock = 0;
  unsigned int bytesWritten = 0;
  unsigned int startWriting = directory[fd].byte_offset;
  //int writeSuccessful = TRUE;

  if(tfs_check_fd_in_range(fd) && tfs_check_file_is_open(fd)){
    if(directory[fd].first_block == 0){
      directory[fd].first_block = tfs_new_block();
      currBlock = directory[fd].first_block;
      file_allocation_table[currBlock] = LAST_BLOCK;
    }
    else{
      currBlock = findLastBlock(fd);
    }

    while(bytesWritten < byte_count){
      if((bytesWritten != 0) && ((startWriting % BLOCK_SIZE) == 0)){
        lastBlock = currBlock;
        file_allocation_table[lastBlock] = tfs_new_block();
        currBlock = file_allocation_table[lastBlock];
        if(!tfs_check_block_in_range(currBlock)){break;}
        file_allocation_table[currBlock] = LAST_BLOCK;
      }

      blocks[i].bytes[bytesWritten] = buffer[bytesWritten];

      bytesWritten++;
      startWriting++;
    }
    directory[fd].byte_offset += bytesWritten;
  }
/*
  if(writeSuccessful){
    for(i = directory[fd].byte_offset; i < directory[fd].byte_offset+byte_count; i++){
      //check if still in valid range. if not, check to see if the next block is available, continue to write if so; if not, break
      if(!tfs_check_block_in_range(currBlock)){
        printf("Write was unsuccessful because block was out of range. Please try again.\n");
        break;
      }
      if(tfs_new_block()){
        unsigned char nextBlock = file_allocation_table[currBlock] = tfs_new_block(); //point this block to next block
        currBlock = nextBlock; //increment block?
        continue; //in new block
      }
      else{
        writeSuccessful = FALSE;
        break;
      }
      //read in bytes from file (bytes) to buffer
      blocks[currBlock].bytes[i] = buffer[i];
    }
    file_allocation_table[currBlock] = 1; //once write is done, point final file block to 1
  }
  else{
    printf("Write was unsuccessful. Please try again.\n");
  }
  directory[fd].byte_offset += byte_count;
*/
  return bytesWritten;
}
