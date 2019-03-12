//Reagan Leonard
//CPSC 2151 Section 002
//Lab 5
//9/28/18

package cpsc2150.MyQueue;
import java.util.*;

    /**
     * Correspondence: this = myQ. The front of myQ is the front of the Queue
     * Correspondence: size = myQ.size();
     *
     * @invariant: 0 <= depth <= MAX_DEPTH
     */
    public class ListQueueImp implements IntegerQueueI {
        private List<Integer> myQ;

        /**
         * @pre que is initialized
         * @param que is the list of Integers that will make the Queue
         */
        public ListQueueImp(List<Integer> que){
            myQ = que;
        }

        //Implements IntegerQueueI method add
        public void add(Integer x){
            myQ.add(x);
        }

        //Implements IntegerQueueI method pop
        public Integer pop(){
            return myQ.remove(0);
        }

        //Implements IntegerQueueI method size
        public int size(){
            return myQ.size();
        }

    }


