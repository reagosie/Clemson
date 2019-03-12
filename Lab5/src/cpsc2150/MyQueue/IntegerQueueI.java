//Reagan Leonard
//CPSC 2151 Section 002
//Lab 5
//9/28/18

package cpsc2150.MyQueue;
    /**
     * A queue containing integers.
     * A queue is a data structure where the first item added to the
     structure is the first item removed from the structure
     * This queue is bounded by MAX_DEPTH
     *
     * Initialization ensures the queue is empty
     * Defines: size:Z
     * Constraints: 0 <= size <= MAX_DEPTH
     */
    public interface IntegerQueueI {
        int MAX_DEPTH = 100;
        /**
         * Adds Integer x to the end of the queue
         * @pre Queue has been initialized
         * @param x the Integer being added to the end of the queue
         */
        public void add(Integer x);

        /**
         * returns the first Integer in the queue
         * @pre: Queue has been initialized
         * @return the Integer at the front of the queue
         */
        public Integer pop();

        /**
         * returns the size of the Queue
         * @pre: Queue has been initialized
         * @return IntgerQueueI.size
         */
        public int size();
    }


