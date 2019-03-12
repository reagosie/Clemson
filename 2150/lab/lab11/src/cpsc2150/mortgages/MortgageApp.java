package cpsc2150.mortgages;

public class MortgageApp {
    public static void main(String [] args){
        IMortgageView view = new MortgageView(); //create view
        MortgageController controller = new MortgageController(view); //create controller
        view.setController(controller); //set controller
    }
}
