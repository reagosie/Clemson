#include "tfs.h"
#include <stdlib.h>

/* helper function declarations */
unsigned int findFinalBlock(unsigned int fd);

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
  unsigned int fd = file_descriptor;
  unsigned int currBlock = directory[fd].first_block;

  //delete directory (change status to unused)
  directory[fd].status = 0;

  if(directory[fd].first_block != 0){
    //release all allocated file blocks
    while(currBlock != LAST_BLOCK){
      unsigned int nextBlock = file_allocation_table[currBlock];
      file_allocation_table[currBlock] = 0;
      currBlock = nextBlock;
    }
    file_allocation_table[currBlock] = 0;
    return(TRUE);
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
  unsigned int fd = file_descriptor;
  unsigned int currBlock = directory[fd].first_block;
  unsigned int bytesRead = 0;
  unsigned int startReading = directory[fd].byte_offset;
  double startInBlock = (((double)startReading / (double)BLOCK_SIZE) - (startReading / BLOCK_SIZE)) * BLOCK_SIZE;

  if(tfs_check_fd_in_range(fd) && tfs_check_file_is_open(fd)){
    if(directory[fd].first_block == 0){ //if just initialized
      directory[fd].first_block = tfs_new_block(); //find next available block
      currBlock = directory[fd].first_block;
      file_allocation_table[currBlock] = LAST_BLOCK;
    }
    else{
      currBlock += (startReading / BLOCK_SIZE);
      if((startReading % BLOCK_SIZE) != 0){currBlock+=1;}
    }

    while((bytesRead < byte_count) && (startReading < directory[fd].size)){
      if((bytesRead != 0) && (file_allocation_table[currBlock] != LAST_BLOCK)){
        unsigned int nextBlock = file_allocation_table[currBlock];
        currBlock = nextBlock;
        if(!tfs_check_block_in_range(currBlock)){break;}
      }

      // printf("currBlock = %i\n", currBlock);
      // printf("startInBlock = %i\n", (int)startInBlock);
      // printf("bytesRead = %i\n", bytesRead);

      buffer[bytesRead] = blocks[currBlock].bytes[(int)startInBlock];
      // printf("what it should be: %c\n", blocks[currBlock].bytes[(int)startInBlock]);
      // printf("%s\n", buffer);

      bytesRead++;
      startReading++;
      startInBlock++;
    }

    // printf("what it really should be: ");
    // for(int i = 0; i < BLOCK_SIZE; i++){
    //   printf("%c", blocks[currBlock].bytes[i]);
    // }
    // printf("\n");

    file_allocation_table[currBlock] = LAST_BLOCK;

    directory[fd].byte_offset = bytesRead % BLOCK_SIZE;
  }
  return bytesRead;
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

  if(tfs_check_fd_in_range(fd) && tfs_check_file_is_open(fd)){
    if(directory[fd].first_block == 0){
      directory[fd].first_block = tfs_new_block();
      currBlock = directory[fd].first_block;
      file_allocation_table[currBlock] = LAST_BLOCK;
    }
    else{
      currBlock = findFinalBlock(fd);
    }

    while(bytesWritten < byte_count){
      if((bytesWritten != 0) && ((startWriting % BLOCK_SIZE) == 0)){
        lastBlock = currBlock;
        file_allocation_table[lastBlock] = tfs_new_block();
        currBlock = file_allocation_table[lastBlock];
        if(!tfs_check_block_in_range(currBlock)){break;}
        file_allocation_table[currBlock] = LAST_BLOCK;
      }

      blocks[currBlock].bytes[bytesWritten] = buffer[bytesWritten];

      bytesWritten++;
      startWriting++;
    }

    file_allocation_table[currBlock] = LAST_BLOCK;

    directory[fd].byte_offset = bytesWritten % BLOCK_SIZE;
    directory[fd].size += bytesWritten;
  }

  return bytesWritten;
}

/* helper functions */
unsigned int findFinalBlock(unsigned int fd){
  unsigned int i = directory[fd].first_block;
  while(file_allocation_table[i] != LAST_BLOCK){
    i++;
  }
  return i;
}
