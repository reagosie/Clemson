package cpsc2150.labs.lab2;

import java.util.*;
import java.lang.*;

public class MortgageApp {

    public static void main(String [] args)
    {
        Scanner in = new Scanner(System.in);
        System.out.println("Please enter your customer information.");  //ask for user input
        System.out.println("Please enter your monthly debt payments.");
        double debt = in.nextDouble();
        while(debt < 0){
            System.out.println("Debt payments cannot be less than 0. Please try again."); //check to make sure input is valid
            debt = in.nextDouble();
        }
        System.out.println("Please enter your income."); //ask for user input
        double inc = in.nextDouble();
        while(inc < 0){
            System.out.println("Income cannot be less than 0. Please try again."); //check to make sure input is valid
            inc = in.nextDouble();
        }
    System.out.println("Please enter your credit score."); //ask for user input
        int score = in.nextInt();
        while(score < 0 || score > 850){
            System.out.println("Credit score cannot be less than 0 or greater than 850. Please try again."); //check to make sure input is valid
            score = in.nextInt();
        }
        in.nextLine();
        System.out.println("Please enter your name."); //ask for user input
        String name = in.nextLine();
        while(name.isEmpty()){
            System.out.println("Name cannot be left blank. Please try again."); //check to make sure input is valid
            name = in.nextLine();
        }
        Customer cust = new Customer(debt, inc, score, name); //construct customer based on info given

        System.out.println("You will now be asked to enter in your loan application information.");
        System.out.println("Please enter the cost of the house."); //ask for user input
        double cost = in.nextDouble();
        while(cost < 0){
            System.out.println("House cost cannot be less than 0. Please try again."); //check to make sure input is valid
            cost = in.nextDouble();
        }
        System.out.println("Please enter your down payment on the house."); //ask for user input
        double dp = in.nextDouble();
        while(dp < 0 || dp > cost){
            System.out.println("Down payment cannot be less than 0 or greater than the cost of the house. Please try again."); //check to make sure input is valid
            dp = in.nextDouble();
        }
        System.out.println("Please enter how many years the mortgage is for."); //ask for user input
        int y = in.nextInt();
        while(y != 15 && y != 20 && y != 25 && y != 30){
            System.out.println("Years must be 15, 20, 25, or 30. Please try again."); //check to make sure input is valid
            y = in.nextInt();
        }

        cust.applyForLoan(dp, cost, y); //apply for loan using information given
        System.out.println(cust.toString()); //print customer info followed by mortgage info

    }
}