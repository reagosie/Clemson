//Reagan Leonard
//CPSC 2151 Section 002
//Lab 6
//10/12/18

package cpsc2150.MyQueue;
import java.util.*;

/**
 * Correspondence: this = myQ. The front of myQ is the front of the Queue
 * Correspondence: size = myQ.size();
 *
 * @invariant: 0 <= depth <= MAX_DEPTH
 */
public class ListQueueImp<T> implements IQueue<T> { // class for my list implementation of the queue
    private List<T> myQ;

    /**
     * @pre there must be enough dynamic memory to allocate for a new ArrayList
     * @post a list queue is created of type ArrayList
     */
    public ListQueueImp(){
        myQ = new ArrayList<>();
    } // constructor

    public void add(T x){
        myQ.add(x);
    } // function to add to the queue

    public T pop(){ // function to remove from the queue
        T i = myQ.get(0);
        myQ.remove(0);
        return i;
    }

    public int size(){
        return myQ.size();
    } // function that returns the size of the queue

    @Override
    public String toString(){ // function that overrides toString
        String str = "\n";
        for (int i = 0; i < size(); i++){
            T b = pop();
            add(b);
            str += b + "\n";
        }
        return str;
    }
}
