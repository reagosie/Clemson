//Reagan Leonard
//CPSC 2151 Section 002
//Lab 5
//9/28/18

package cpsc2150.MyQueue;
    /**
     * Correspondence: this = myQ[0...depth-1], myQ[0] is the front
     of the Queue
     * Correspondence: size = depth
     *
     * @invariant: 0 <= depth <= MAX_DEPTH
     */
    public class ArrayQueueImp implements IntegerQueueI {
        private Integer[] myQ;
        private int depth;

        /**
         * @pre 0 <= size <= MAX_DEPTH, Que is initialized
         * @param Que is the array of integers stored in the queue
         * @param size is the number of Integers in the queue
         * @return returns an intialized ArrayQueue object
         */
        public ArrayQueueImp(Integer[] Que, int size){
            myQ = Que;
            depth = size;
        }

        //Implements IntegerQueueI method add
        public void add(Integer x){
            depth += 1;
            myQ[depth-1] = x;

        }

        //Implements IntegerQueueI method pop
        public Integer pop(){
            Integer re = myQ[0];
            for(int i = 1; i < depth; i++){
                myQ[i-1] = myQ[i];
            }
            depth -= 1;
            return re;
        }

        //Implements IntegerQueueI method size
        public int size(){
            return depth;
        }

    }

