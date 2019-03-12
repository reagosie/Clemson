//Reagan Leonard
//CPSC 2151 Section 002
//Lab 6
//10/12/18

package cpsc2150.MyQueue;
/**
 * Correspondence: this = myQ[0...depth-1], myQ[0] is the front
 of the Queue
 * Correspondence: size = depth
 *
 * @invariant: 0 <= depth <= MAX_DEPTH
 */
public class ArrayQueueImp<T> implements IQueue<T> { // class for my array implementation of the queue
    private T[] myQ;
    private int depth;

    /**
     * @pre MAX_DEPTH must be set
     * @post an array queue is created
     */
    public ArrayQueueImp(){ // constructor
        depth = 0;
        //myQ = new T[MAX_DEPTH];
        myQ = (T[]) new Object[MAX_DEPTH]; // instantiate myQ as an Object
    }

    public void add(T x){ // function to add to the queue
        myQ[depth] = x;
        this.depth++;
    }

    public T pop(){ // function to remove something from the queue
        T i = myQ[0];
        for (int x = 1; x < depth; x++){
            myQ[x - 1] = myQ[x];
        }
        depth--;
        return i;
    }

    public int size(){
        return depth;
    } // function that returns the size of the queue

    @Override
    public String toString(){ // function that overrides toString
        String str = "\n";
        T b;
        for (int i = 0; i < size(); i++){
            b = pop();
            add(b);
            str += b + "\n";
        }
        return str;
    }
}
