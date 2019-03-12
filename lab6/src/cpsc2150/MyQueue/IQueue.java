//Reagan Leonard
//CPSC 2151 Section 002
//Lab 6
//10/12/18

package cpsc2150.MyQueue;
/**
 * A queue containing any data type.
 * A queue is a data structure where the first item added to the
 structure is the first item removed from the structure
 * This queue is bounded by MAX_DEPTH
 *
 * Initialization ensures the queue is empty
 * Defines: size: Z
 * Constraints: 0 <= size <= MAX_DEPTH
 */
public interface IQueue<T> { // generic interface for the queue
    int MAX_DEPTH = 100;
    /**
     * Adds element x to the end of the queue
     * @pre Queue has been initialized
     * @param x the element being added to the end of the queue
     */
    void add(T x); // function to add to the end of the queue

    /**
     * returns the first element in the queue
     * @pre: Queue has been initialized
     * @return the element at the front of the queue
     */
    T pop(); // function that removes the element at the front of the queue

    /**
     * returns the size of the Queue
     * @pre: Queue has been initialized
     * @return IQueue.size
     */
    int size(); // function that returns the size of the queue

    /**
     * @pre a queue must be constructed with at least one element in it
     * @return the element at the front of the queue
     * @post returns the element at the front of the queue
     */
    default T peek(){ // function that allows the user to see the element at the front of the queue without popping it
        T a = pop();
        add(a);
        for (int i = 0; i < size() - 1; i++){
            T b = pop();
            add(b);
        }
        return a;
    }

    /**
     * @pre a queue must be constructed with at least one element in it
     * @return the element at the end of the queue
     * @post returns the element at the end of the queue
     */
    default T endOfQueue(){ // function that allows the user to see the last element in the queue without popping it
        for (int i = 0; i < size() - 1; i++){
            T b = pop();
            add(b);
        }
        T a = pop();
        add(a);
        return a;
    }

    /**
     * @pre queue must be constructed already
     * @pre x must be an element
     * @pre pos must be a positive element that exists as a location in the queue
     * @param x element that the user wants to insert into the queue
     * @param pos the position that the user wants to insert x at in the queue
     * @post inserts x and position pos in the queue
     */
    default void insert(T x, int pos){ // function that can insert an element at any index in the queue
        for (int i = 1; i < pos; i++){
            T b = pop();
            add(b);
        }
        add(x);
        for (int i = pos; i < size(); i++){
            T b = pop();
            add(b);
        }
    }

    /**
     * @pre queue must be constructed with at least one element in it
     * @pre pos must be a positive element that exists as a location in the queue
     * @param pos the position that the user wants to remove an element from the queue
     * @return the element removed
     * @post removes the element at position pos
     * @post returns the element removed
     */
    default T remove(int pos){ // function that can remove an element at any index in the queue
        for (int i = 1; i < pos; i++){
            T b = pop();
            add(b);
        }
        T a = pop();
        for (int i = pos; i < size(); i++){
            T b = pop();
            add(b);
        }
        return a;
    }

    /**
     * @pre pos must be a positive element that exists as a location in the queue
     * @pre queue must be constructed with at least one element in it
     * @param pos the position that the user wants to view in the queue
     * @return the element searched for
     * @post returns the element as position pos for the user to view
     */
    default T get(int pos){ // function that can return an element at any index in the queue without popping it
        for (int i = 1; i < pos; i++){
            T b = pop();
            add(b);
        }
        T a = pop();
        add(a);
        for (int i = pos; i < size(); i++){
            T b = pop();
            add(b);
        }
        return a;
    }
}
