/*  Reagan Leonard
    CPSC 2150
    Lab 10
    11/16/18
 */

package cpsc2150.mortgages;

import java.util.Scanner;

public class MortgageView {
    Scanner in = new Scanner(System.in);

    //constructor
    MortgageView(){
        //MortgageView view = new MortgageView();
    }

    public double getHouseCost(){
        System.out.println("How much does the house cost?");
        double c = in.nextDouble();
        return c;
    }

    public double getDownPayment(){
        System.out.println("How much is the down payment?");
        double c = in.nextDouble();
        return c;
    }

    public int getYears(){
        System.out.println("How many years?");
        int c = in.nextInt();
        //in.nextLine();
        return c;
    }

    public double getMonthlyDebt(){
        System.out.println("How much are your monthly debt payments?");
        double c = in.nextDouble();
        return c;
    }

    public double getYearlyIncome(){
        System.out.println("How much is your yearly income?");
        double c = in.nextDouble();
        return c;
    }

    public int getCreditScore(){
        System.out.println("What is your credit score?");
        int c = in.nextInt();
        return c;
    }

    public String getName(){
        System.out.println("What's your name?");
        String c = in.nextLine();
        return c;
    }

    public void printToUser(String message){
        System.out.println(message);
    }

    public boolean getAnotherMortgage(){
        System.out.println("Would you like to apply for another mortgage? Y/N");
        String choice = in.next();
        if(choice.equals("Y") || choice.equals("y")){
            return true;
        }
        else{return false;}
    }

    public boolean getAnotherCustomer(){
        System.out.println("Would you like to consider another customer? Y/N");
        String choice = in.next();
        in.nextLine();
        if(choice.equals("Y") || choice.equals("y")){
            return true;
        }
        else{return false;}
    }
}
