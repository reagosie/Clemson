/*  Reagan Leonard
    CPSC 2150
    Lab 10
    11/16/18
 */

package cpsc2150.mortgages;

import static java.lang.System.exit;

public class MortgageController {
    private MortgageView view;
    public boolean newMortgage = true;
    public boolean newCustomer = true;

    MortgageController(MortgageView view){
        this.view = view;
    }

    public void run(){
        //run the program
        while(newCustomer) {
            String name = view.getName();
            while (name.isEmpty()) {
                view.printToUser("Name cannot be left blank. Please try again.");
                name = view.getName();
            }
            double income = view.getYearlyIncome();
            while (income < 0) {
                view.printToUser("Income must be greater than 0.");
                income = view.getYearlyIncome();
            }
            double debt = view.getMonthlyDebt();
            while (debt < 0) {
                view.printToUser("Debt must be greater than or equal to 0.");
                debt = view.getMonthlyDebt();
            }
            int score = view.getCreditScore();
            while (score < 0 || score > 850) {
                view.printToUser("Credit Score must be greater than 0 and less than 850");
                score = view.getCreditScore();
            }
            while (newMortgage) {
                double cost = view.getHouseCost();
                while (cost < 0) {
                    view.printToUser("Cost must be greater than 0.");
                    cost = view.getHouseCost();
                }
                double downpayment = view.getDownPayment();
                while (downpayment < 0 || downpayment >= cost) {
                    view.printToUser("Down payment must be greater than 0 and less than the cost of the house.");
                    downpayment = view.getDownPayment();
                }
                int years = view.getYears();
                while (years <= 0) {
                    view.printToUser("Years must be greater than 0.");
                    years = view.getYears();
                }

                Customer cust = new Customer(debt, income, score, name);
                Mortgage mort = new Mortgage(cost, downpayment, years, cust);
                cust.applyForLoan(downpayment, cost, years);

                view.printToUser(cust.toString());
                view.printToUser(mort.toString());

                if (!view.getAnotherMortgage()) {
                    newMortgage = false;
                }
            }

            if (!view.getAnotherCustomer()) {
                newCustomer = false;
            }
        }
    }
}
