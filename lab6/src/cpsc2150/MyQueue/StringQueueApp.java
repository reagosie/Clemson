//Reagan Leonard
//CPSC 2151 Section 002
//Lab 6
//10/12/18

package cpsc2150.MyQueue;
import java.util.*;

public class StringQueueApp {
    private static Scanner in;
    public static void main(String[] args) {
        in = new Scanner(System.in);
        System.out.println("Enter 1 for array implementation or 2 for List implementation"); // ask for user input
        int answer = in.nextInt(); // get user input

        IQueue q; // instantiate the queue

        if (answer == 1) {
            q = new ArrayQueueImp(); // decide whether to use Array or List imp based on user input
        } else {
            q = new ListQueueImp();
        }
        displayMenu(); // display menu
        answer = in.nextInt(); // get user input for action
        in.nextLine(); // clear buffer of new line character
        while (answer != 8) {
            if (answer == 1) { // call addToQueue if user inputs 1
                addToQueue(q);
            } else if (answer == 2) { // call getNext if user inputs 2
                getNext(q);
            } else if (answer == 3) { // call peekInQueue if user inputs 3
                peekInQueue(q);
            } else if (answer == 4) { // call peekAtEnd if user inputs 4
                peekAtEnd(q);
            } else if (answer == 5) { // call insertInQueue if user inputs 5
                insertInQueue(q);
            } else if (answer == 6) { // call getFromQueue if user inputs 6
                getFromQueue(q);
            } else if (answer == 7) { // call removeFromQueue if user inputs 7
                removeFromQueue(q);
            } else {
                System.out.println("Not a valid option!"); // if input is not 1-8, print error message
            }
            System.out.println("Queue is: "); // print current queue
            System.out.println(q.toString());
            System.out.println(" ");
            displayMenu(); // display menu again for further action
            answer = in.nextInt(); // get user input for further action
            in.nextLine(); // clear buffer of new line character
        }
    }
    private static void displayMenu(){ // function that displays the user's options for action upon the queue
        System.out.println("Select an option: ");
        System.out.println("1. Add to the Queue");
        System.out.println("2. Get next number from the Queue");
        System.out.println("3. Peek at the front of the Queue");
        System.out.println("4. Peek at the end of the Queue");
        System.out.println("5. Insert in the Queue");
        System.out.println("6. Get a position in the Queue");
        System.out.println("7. Remove from a position in the Queue");
        System.out.println("8. Quit");
    }

    private static void addToQueue(IQueue q){ // function that adds to the end of the queue
        if (q.size() < q.MAX_DEPTH) {
            System.out.println("What string to add to the Queue?");
            String answer = in.nextLine();
            q.add(answer);
        }
        else System.out.println("Queue is full!");
    }

    private static void getNext(IQueue q){ // function that prints the last element of the queue
        if (q.size() > 0) System.out.println("Next string is " + q.pop());
        else System.out.println("Queue is empty!");
    }

    private static void peekInQueue(IQueue q){ // function that prints the first element in the queue
        if (q.size() > 0) System.out.println("Peek: " + q.peek());
        else System.out.println("Queue is empty!");
    }

    private static void peekAtEnd(IQueue q){ // function that prints the last element in the queue
        if (q.size() > 0) System.out.println("Peek at end: " + q.endOfQueue());
        else System.out.println("Queue is empty!");
    }

    private static void insertInQueue(IQueue q){ // function that inserts an element anywhere in the queue
        if (q.size() < q.MAX_DEPTH) {
            System.out.println("What string to add to the Queue?");
            String str = in.nextLine();
            System.out.println("What position to insert in?");
            int pos = in.nextInt();
            q.insert(str, pos);
        }
        else System.out.println("Queue is full!");
    }

    private static void getFromQueue(IQueue q){ // function that can print an element from any index in the queue
        if (q.size() > 0){
            System.out.println("What position to get from the Queue?");
            int pos = in.nextInt();
            System.out.println(q.get(pos));
        }
        else System.out.println("Queue is empty!");
    }

    private static void removeFromQueue(IQueue q){ // function that removes an element from any index in the queue
        if (q.size() > 0) {
            System.out.println("What position to remove from the Queue?");
            int pos = in.nextInt();
            System.out.println(q.remove(pos));
        }
        else System.out.println("Queue is empty!");
    }
}