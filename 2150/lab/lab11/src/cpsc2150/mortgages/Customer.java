/*  Reagan Leonard
    CPSC 2150
    Lab 11
    11/30/18
 */

package cpsc2150.mortgages;

/**
 * @inv interest rate >= 0
 * @inv monthly payment >= 0
 * @inv monthly debt payments >=0
 * @inv credit score >= 0 and <= 850
 **/
public class Customer {
    private String name;

    private double monthlyDebtPayments;
    private double income;
    private int creditScore;
    private Mortgage loan;

    /**
     * @pre
     * debt >= 0, inc >= 0, score >= 0, n != empty
     * @param debt the customer's monthly debt payment
     * @param inc the customer's monthly income
     * @param score the customer's credit score
     * @param n the customer's name
     */
    Customer( double debt, double inc, int score, String n)
    {
        /*
        Set the monthly debt payments, income, credit score and name
        Credit scores range from 0 - 850
         */
        monthlyDebtPayments = debt;
        income = inc;
        creditScore = score;
        name = n;

    }

    /**
     * @pre
     * downPayment >= 0, houseCost > 0, years > 0
     * @param downPayment the customer's down payment
     * @param houseCost the total cost of the house
     * @param years number of years for the mortgage to be paid off
     * @return true if loan was approved or false if not
     */
    public boolean applyForLoan(double downPayment, double houseCost, int years)
    {
        //Take the parameters and apply for a loan. Save the loan details as the customers current loan
        // Return whether or not the loan was approved
        Customer cust = new Customer(monthlyDebtPayments, income, creditScore, name);
        loan = new Mortgage(downPayment, houseCost, years, cust);
        if(loan.loanApproved()) {
            return true;
        }
        else{return false;}
    }

    /**
     * @post interestRate >= 0
     * @return interestRate
     */
    public double getRate()
    {
        //return the interest rate on the current loan
        return loan.getRate();
    }

    /**
     * @post payment >= 0
     * @return payment
     */
    public double getMonthlyPay()
    {
        //return the monthly payment on the current loan
        return loan.getPayment();
    }

    /**
     * @post debt >= 0
     * @return debt
     */
    public double getMonthlyDebtPayments()
    {
        //return the customers monthly debt payments
        return monthlyDebtPayments;
    }

    /**
     * @post inc >= 0
     * @return inc
     */
    public double getIncome()
    {
        //return the customers income
        return income;
    }

    /**
     * @post 0 <= score <=850
     * @return score
     */
    public int getCreditScore()
    {
        //return the customers credit score
        return creditScore;
    }


    /**
     * @pre
     * houseCost >= 0, downPayment >= 0, interestRate >= 0, years >= 0, payment >= 0
     * @return the principal amount, interest rate, and monthly payment if loan was approved
     * @return if loan was not approved, print "loan was not approved"
     */
    @Override
    public String toString(){
        String str = "";
        str += "Name: " + name + "\n";
        str += "Income: $" + income + "\n";
        str += "Credit Score: " + creditScore + "\n";
        str += "Monthly Debt: $" + monthlyDebtPayments + "\n";
        str += "Mortgage info: \n";
        if( loan != null) {
            str += loan.toString();
        }
        return str;
    }
}

