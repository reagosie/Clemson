package cpsc2150.mortgages;

public class MortgageController {
    private IMortgageView view;

    public MortgageController(IMortgageView v){
        view = v;
    }

    public void submitApplication(){
        boolean valid = true; //boolean value to control whether load is applied for based on valid input

        String name = view.getName(); //ask user for name
        if (name.isEmpty()) {
            view.printToUser("Name cannot be left blank. Please try again."); //check for error and ask again
            valid = false;
        }

        double income = view.getYearlyIncome(); //ask user for yearly income
        if (income < 0) {
            view.printToUser("Income must be greater than 0."); //check for error and ask again
            valid = false;
        }

        double debt = view.getMonthlyDebt(); //ask user for monthly debt
        if (debt < 0) {
            view.printToUser("Debt must be greater than or equal to 0."); //check for error and ask again
            valid = false;
        }

        int score = view.getCreditScore(); //ask user for credit score
        if (score < 0 || score > 850) {
            view.printToUser("Credit Score must be greater than 0 and less than 850"); //check for error and ask again
            valid = false;
        }

        double cost = view.getHouseCost(); //ask user for house cost
        if (cost < 0) {
            view.printToUser("Cost must be greater than 0."); //check for error and ask again
            valid = false;
        }

        double downpayment = view.getDownPayment(); //ask user for down payment
        if (downpayment < 0 || downpayment >= cost) {
            view.printToUser("Down payment must be greater than 0 and less than the cost of the house."); //check for error and ask again
            valid = false;
        }

        int years = view.getYears(); //ask user for years
        if (years <= 0) {
            view.printToUser("Years must be greater than 0."); //check for error and ask again
            valid = false;
        }

        Customer cust = new Customer(debt, income, score, name); //create customer
        Mortgage mort = new Mortgage(cost, downpayment, years, cust); //create mortgage

        if(valid) { //if ALL input is valid, apply for loan
            view.displayApproved(cust.applyForLoan(downpayment, cost, years)); //apply for loan and print if approved or not
            view.displayPayment(mort.getPayment()); //if approved, print payment
            view.displayRate(mort.getRate()); //if approved, print rate
        }
        else{ //if ANY input is invalid, don't apply for loan. Just print loan denied, payment=0, rate=0
            view.displayApproved(false);
            view.displayPayment(0);
            view.displayRate(0);
        }
    }
}

