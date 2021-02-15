#include "tfs.h"


unsigned int findPrevBlock(unsigned int);

/* implementation of assigned functions */


/* you are welcome to use helper functions of your own */



/* tfs_delete()
 *
 * deletes a closed directory entry having the given file descriptor
 *   (changes the status of the entry to unused) and releases all
 *   allocated file blocks
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is closed
 *
 * postconditions:
 *   (1) the status of the directory entry is set to unused
 *   (2) all file blocks have been set to free
 *
 * input parameter is a file descriptor
 *
 * return value is TRUE when successful or FALSE when failure
 */

unsigned int tfs_delete( unsigned int file_descriptor )
{
unsigned int fd = file_descriptor;
  unsigned int currentBlock, temp;

  // Check preconditions
  if (tfs_check_fd_in_range(fd) && directory[fd].status == CLOSED)
  {
    // If nothing has been written to the file, just change status
    if (directory[fd].first_block != 0)
    {
      // Start at first_block and trace through FAT, freeing blocks
      currentBlock = directory[fd].first_block;

      do
      {
        temp = file_allocation_table[currentBlock];
        file_allocation_table[currentBlock] = FREE;

        // If LAST_BLOCK reached, free it and break
        if (temp == LAST_BLOCK)
        {
          break;
        }

        currentBlock = temp;
      } while (currentBlock != LAST_BLOCK);
    }

    directory[fd].status = UNUSED;

    return TRUE;
  }

  return FALSE;
}


/* tfs_read()
 *
 * reads a specified number of bytes from a file starting
 *   at the byte offset in the directory into the specified
 *   buffer; the byte offset in the directory entry is
 *   incremented by the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * the function will read fewer bytes than specified if the
 *   end of the file is encountered before the specified number
 *   of bytes have been transferred
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *   (3) the file has allocated file blocks
 *
 * postconditions:
 *   (1) the buffer contains bytes transferred from file blocks
 *   (2) the specified number of bytes has been transferred
 *         except in the case that end of file was encountered
 *         before the transfer was complete
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */

unsigned int tfs_read( unsigned int file_descriptor,
                       char *buffer,
                       unsigned int byte_count )
{
  unsigned int fd = file_descriptor;
  unsigned int bytes_transferred = 0;
  unsigned int currentBlock, blocksToTraverse, i, temp;
  unsigned int readFrom = directory[fd].byte_offset % BLOCK_SIZE; // Tells where to begin reading in file

  // Check the preconditions and also see if there are any blocks to read at all
  if (tfs_check_fd_in_range(fd) && tfs_check_file_is_open(fd) && directory[fd].first_block !=0)
  {
    // Calculate number of blocks to traverse to get to offset
    blocksToTraverse = (byte_count / BLOCK_SIZE);

    // If blocksToTraverse is 0, don't account for using first block
    // because there's no traversal occuring
    if (blocksToTraverse != 0)
    {
      // Minus one because we start on the first block, so it must be excluded
      blocksToTraverse -= 1;
    }


    // If required, traverse FAT until desired block is reached
    temp = directory[fd].first_block;
    i = 0;

    while (i < blocksToTraverse)
    {
      temp = file_allocation_table[temp];
      i++;
    }

    currentBlock = temp;


    // Begin reading until byte_count bytes are read
    while (bytes_transferred < byte_count)
    {
      // If end of block, move to next one
      if (readFrom % BLOCK_SIZE == 0)
      {
        currentBlock = file_allocation_table[currentBlock];

        // If end of file, return early
        if (currentBlock == LAST_BLOCK) { break; }
      }

      buffer[bytes_transferred] = blocks[currentBlock].bytes[readFrom];

      readFrom++;
      bytes_transferred++;
    }
  }

  return bytes_transferred;
}

/* tfs_write()
 *
 * writes a specified number of bytes from a specified buffer
 *   into a file starting at the byte offset in the directory;
 *   the byte offset in the directory entry is incremented by
 *   the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * furthermore, depending on the starting byte offset and the
 *   specified number of bytes to transfer, additional file
 *   blocks, if available, will be added to the file as needed;
 *   in this case, the size of the file will be adjusted
 *   based on the number of bytes transferred beyond the
 *   original size of the file
 *
 * the function will read fewer bytes than specified if file
 *   blocks are not available
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *
 * postconditions:
 *   (1) the file contains bytes transferred from the buffer
 *   (2) the specified number of bytes has been transferred
 *         except in the case that file blocks needed to
 *         complete the transfer were not available
 *   (3) the size of the file is increased as appropriate
 *         when transferred bytes extend beyond the previous
 *         end of the file
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */

unsigned int tfs_write( unsigned int file_descriptor,
                        char *buffer,
                        unsigned int byte_count )
{
  unsigned int fd = file_descriptor;
  unsigned int bytes_transferred = 0;
  unsigned int currentBlock, prevBlock = 0;
  unsigned int writeTo = directory[fd].byte_offset; // Tells where to begin writing in file

  // Check for preconditions
  // TODO: might need to make sure that there's a block available for writing before actually writing
  if (tfs_check_fd_in_range(fd) && tfs_check_file_is_open(fd) && tfs_check_block_in_range(tfs_new_block()))
  {
    // Find first available block
    // Immediately after initialization
    if (directory[fd].first_block == 0)
    {
      directory[fd].first_block = tfs_new_block();
      currentBlock = directory[fd].first_block;

      // Set FAT table entry with b to last_block
      // will be expanded as needed
      file_allocation_table[currentBlock] = LAST_BLOCK;
    }
    // Pick up where the last write left off
    else
    {
      currentBlock = findPrevBlock(fd);
    }


    // Write until complete or out of space
    while (bytes_transferred < byte_count)
    {
      // Check to see if block is out of space
      if ((bytes_transferred != 0) && (writeTo % (BLOCK_SIZE)) == 0)
      {

        // If out of space, get a new block and update FAT
        prevBlock = currentBlock;
        file_allocation_table[prevBlock] = tfs_new_block();
        currentBlock = file_allocation_table[prevBlock];

        // Make sure the new block is valid
        if (!tfs_check_block_in_range(currentBlock))
        {
          break;
        }

        // Set as temp last_block; will be expanded later as needed
        file_allocation_table[currentBlock] = LAST_BLOCK;
      }

      // Write from buffer to file_blocks
      blocks[currentBlock].bytes[writeTo % (BLOCK_SIZE)] = buffer[bytes_transferred];

      // Move to next byte to write to and count how many bytes written
      writeTo++;
      bytes_transferred++;
    }

    // When done writing, mark current block as done
    file_allocation_table[currentBlock] = LAST_BLOCK;

    // Save how many bytes traveled into the current block as the offset
    // and add the number of bytes_transferred to the file size
    directory[fd].byte_offset = bytes_transferred % BLOCK_SIZE;
    directory[fd].size += bytes_transferred;
  }

  return bytes_transferred;

}


/* Loop until the last block written to is found */
unsigned int findPrevBlock(unsigned int fd)
{
  // Start at file's first block
  unsigned int i = directory[fd].first_block;

  while (file_allocation_table[i] != LAST_BLOCK)
  {
    i++;
  }

  return i;
}
