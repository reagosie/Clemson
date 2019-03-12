//Reagan Leonard
//CPSC 2151 Section 002
//Lab 5
//9/28/18

package cpsc2150.MyQueue;
import java.util.Scanner;
import java.util.*;

public class QueueApp {
    //Main method
    public static void main(String args[]){
        IntegerQueueI q = null;

        //Get user input to determine which implementation is used
        Scanner scan  = new Scanner(System.in);
        int lcv1 = 0, lcv2 = 0;
        while(lcv1 == 0) {
            System.out.println("Enter 1 for an array implementation, or enter 2 for a list implementation");
            int test = scan.nextInt();

            //Array Implementation
            if (test == 1) {
                int size = 0;
                System.out.println("Enter size of the array");
                size = scan.nextInt();
                //Initialize array
                Integer[] arr = new Integer[100];
                for(int i = 0; i < size; i++){
                    System.out.println("Enter the next element in the array");
                    Integer temp = scan.nextInt();
                    arr[i] = temp;
                }
                q = new ArrayQueueImp(arr, size);
                lcv1 = 1;
            }

            //List Implementation
            else if (test == 2) {
                int size = 0;
                System.out.println("Enter size of the list");
                size = scan.nextInt();
                //Initialize List
                List<Integer> list = new ArrayList<Integer>();
                for(int i = 0; i < size; i++){
                    System.out.println("Enter the next element in the list");
                    Integer temp = scan.nextInt();
                    list.add(temp);
                }
                q = new ListQueueImp(list);
                lcv1 = 1;
            }

            //If bad input recieved, return to line 16 and get input again
            else {
                System.out.println("Invalid input, please enter a valid choice");
            }
        }

        Integer x = 42;
        q.add(x);
        x = 17;
        q.add(x);
        x = 37;
        q.add(x);
        x = 36;
        q.add(x);
        x = 12;
        q.add(x);

        //Print Queue
        int size = q.size();
        for(int i = 0; i < size; i++){
            System.out.print(q.pop());
            if(i != size-1)
                System.out.print("-");
        }
    }
}
