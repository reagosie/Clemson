/*  Reagan Leonard
    CPSC 2150
    Lab 7
    10/19/18
 */

package cpsc2150.labs.lab2;

import java.lang.Math;

/**
 * @inv house cost > 0
 * @inv down payment > 0
 * @inv percent down > 0
 * @inv years > 0
 * @inv interest rate > 0
 * @inv payment > 0
 */
public class Mortgage {

    private double houseCost;
    private double downPayment;
    private double percentDown;
    private int years;
    private double interestRate;
    private double payment;
    private Customer customer;

    /**
     * @pre
     * cost > 0, dp >= 0, y > 0
     * @param cost the cost of the house
     * @param dp the amount of the cost paid down
     * @param y years it will take to pay off the mortgage
     * @param cust instance of customer
     */
    Mortgage(double cost, double dp, int y, Customer cust)
    {
        /*
        Sets the house cost, down payment, years and customer. Calculates the percent down, rate, and payment
         */
        houseCost = cost;
        downPayment = dp;
        years = y;
        customer = cust;

        percentDown = downPayment/houseCost;
        calcRate(); //need to implement this function below
        calcPayment(); //need to implement this below as well
    }

    /**
     * @pre interest rate >= 2.5
     * @pre if loan is for 30 years, interest rate >= 3.5
     * @pre if loan is for < 30 years, interest rate >= 3.0
     * @pre loan == 15, 20, 25, or 30 years
     * @post calcRate >= 0
     */
    private void calcRate()
    {
        /*
        Calculates the interest rate to use.
        The rate starts at the base rate of 2.5%
        If the loan is for 30 years, add the normal addition of 1%
        If the loan is for for less than 30 years, add the good addition of .5%
        The loan must be for 15, 20, 25 or 30 years

        Add the rate for the credit score based on the following table
        Credit rating | Credit score | add to rate
        Bad           | 0 - 500      | 10%
        Fair          | 500 - 599    | 5%
        Good          | 600 - 699    | 1%
        Great         | 700 - 749    | .5%
        Excellent     | 750 - 850    | 0%

        If the down payment is less than 20% of the price of the house, add .5%
         */
        interestRate = 0.025;

        if(years == 30){
            interestRate += 0.01;
        }
        else if(years < 30){
            interestRate += 0.005;
        }

        if(customer.getCreditScore() < 500){
            interestRate += 0.1;
        }
        else if(customer.getCreditScore() >= 500 && customer.getCreditScore() <= 599){
            interestRate += 0.05;
        }
        else if(customer.getCreditScore() >= 600 && customer.getCreditScore() <= 699){
            interestRate += 0.01;
        }
        else if(customer.getCreditScore() >= 700 && customer.getCreditScore() <= 749){
            interestRate += 0.005;
        }

        if(percentDown < 0.2){
            interestRate += 0.005;
        }


    }


    /**
     * @pre
     * interestRate >= 0, principal balance > 0, number of payments > 0
     * @post calcPayment >= 0
     */
    private void calcPayment()
    {
        /*
        Calculate the monthly payment
        terms:
        r - monthly rate - the interest rate divided by 12
        p - principal balance - the cost of the house minus the down payment
        n - number of payments - the total number of monthly payments

        monthly payment is (rp)/(1-(1+r)^-n)
         */
        double r = interestRate/12;
        double p = houseCost - downPayment;
        double n = years * 12;

        payment = (r*p)/(1-Math.pow((1+r),-n));
    }

    /**
     * @pre if interestRate >= 10, loan is not approved
     * @pre if downPayment < 3.5% of houseCost, loan is not approved
     * @pre if debt > income * 4, loan is not approved
     * @post loanApproved == true or false
     * @return true if loan is approved or false if not
     */
    public boolean loanApproved()
    {
        /*
        If the interest rate is too high (10% or higher) the loan is denied
        If the down payment is less than 3.5% of the price of the house then the loan is denied

        If the Debt to income ratio is above 40% the loan is denied
        The debt to income ratio is the total monthly debt payments (including the mortgage payment) / monthly income

        Otherwise the loan is approved
         */

        double monthlyIncome = customer.getIncome()/12;
        double debtRatio = customer.getMonthlyDebtPayments()/monthlyIncome;

        if(interestRate >= 0.1){
            return false;
        }
        else if(percentDown < 0.035){
            return false;
        }
        else if(debtRatio >= 0.4){
            return false;
        }
        return true;
    }

    /**
     * @post payment >= 0
     * @return payment
     */
    public double getPayment()
    {
        //return the monthly payment on the loan
        return payment;
    }

    /**
     * @post interestRate >= 0
     * @return interestRate
     */
    public double getRate()
    {
        //return the interest rate on the loan
        return interestRate;
    }


    /**
     * @pre
     * houseCost >= 0, downPayment >= 0, interestRate >= 0, years >= 0, payment >= 0
     * @return the principal amount, interest rate, and monthly payment if loan was approved
     * @return if loan was not approved, print "loan was not approved"
     */
    @Override
    public String toString()
    {
        String str = "";
        if(loanApproved())
        {
            str += "Principal Amount: $" + Math.abs(houseCost - downPayment) + "\n"; //make sure number is positive
            str += "Interest Rate: " + Math.floor(interestRate * 1000.0)/10.0 + "%\n"; //round this percentage
            str += "Term: " + years + " years\n";
            str += "Monthly Payment: $" + Math.abs(payment) + "\n"; //make sure this number is positive
        }
        else
        {
            str += "Loan was not approved";
        }
        return str;
    }

}
