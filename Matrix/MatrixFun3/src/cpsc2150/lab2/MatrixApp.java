package cpsc2150.lab2;
import java.util.*;

public class MatrixApp {
    public static void main(String [] args) {
        //standard variable to hold data returned by helper functions
        int product;
        int [] rowsum, colsum;
        int [][] transmatrix;
        double average;
        String matrixstr = "";
        String transposestr = "";

        //get user input for size of matrix
        System.out.println("How many rows should your matrix have?");
        Scanner scanner = new Scanner(System.in);
        int row = Integer.parseInt(scanner.nextLine());
        System.out.println("How many columns should your matrix have?");
        int col = Integer.parseInt(scanner.nextLine());

        //create matrix, get values from user, and place them in the matrix
        int [][] matrix;
        matrix = new int [row][col];
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                System.out.println("What number should go in Row: " + i + " Col: " + j);
                matrix[i][j] = Integer.parseInt(scanner.nextLine());
            }
        }

        //print original matrix
        matrixstr = print(matrix, row, col);
        System.out.print("Your matrix is: \n" + matrixstr + "\n");

        //transpose matrix and print
        transmatrix = new int [col][row];
        transmatrix = transpose(matrix, col, row);
        transposestr = print(transmatrix, col, row);
        System.out.print("The transposed matrix is: \n" + transposestr + "\n");

        //calculate product sum of matrix and print
        product = productSum(matrix, row, col);
        System.out.print("The product sum is: \n" + product + "\n");

        //calculate average of all values in the matrix and print
        average = average(matrix, row, col);
        System.out.print("The average is: \n" + average + "\n");

        //calculate the sum of each row and print in 1D array
        rowsum = rowSum(matrix, row, col);
        System.out.print("The sums of each Row are: \n");
        for(int k = 0; k < row; k++){
            System.out.print("|" + rowsum[k]);
        }
        System.out.println("|");

        //calculate the sum of each column and print in 1D array
        colsum = colSum(matrix, row, col);
        System.out.print("The sums of each Column are: \n");
        for(int l = 0; l < col; l++){
            System.out.print("|" + colsum[l]);
        }
        System.out.println("|");
    }

    //helper function to print matrices
    private static String print(int [][] matrix, int row, int col) {
        String s = "";
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                s += "|" + matrix[i][j];
            }
            s += "|" + "\n";
        }
        return s;
    }

    //helper function to transpose the original matrix and return
    private static int [][] transpose(int [][] matrix, int col, int row) {
        int[][] temp = new int[col][row];
        for (int i = 0; i < col; i++) {
            for (int j = 0; j < row; j++) {
                temp[i][j] = matrix[j][i];
            }
        }
        return temp;
    }

    //helper function to calculate the product sum and return
    private static int productSum(int [][] matrix, int row, int col) {
        int sum;
        int product = 1;
        for (int i = 0; i < row; i++) {
            sum = 0;
            for (int j = 0; j < col; j++) {
                sum += matrix[i][j];
            }
            product = product * sum;
        }
        return product;
    }

    //helper function to calculate the average and return
    private static double average(int [][] matrix, int row, int col) {
        int rowsum;
        int totalsum = 0;
        double avg;

        for (int i = 0; i < row; i++) {
            rowsum = 0;
            for (int j = 0; j < col; j++) {
                rowsum += matrix[i][j];
            }
            totalsum += rowsum;
        }
        avg = (double)totalsum / (double)(row * col);
        return avg;
    }

    //helper function to calculate the sum of each row and return as 1D array
    private static int[] rowSum(int [][] matrix, int row, int col) {
        int sum;
        int [] arr;
        arr = new int [row];

        for (int i = 0; i < row; i++) {
            sum = 0;
            for (int j = 0; j < col; j++) {
                sum += matrix[i][j];
            }
            arr[i] = sum;
        }
        return arr;
    }

    //helper function to calculate the sum of each column and return as 1D array
    private static int[] colSum(int [][] matrix, int row, int col) {
        int sum;
        int [] arr;
        arr = new int [col];

        for (int i = 0; i < col; i++) {
            sum = 0;
            for (int j = 0; j < row; j++) {
                sum += matrix[j][i];
            }
            arr[i] = sum;
        }
        return arr;
    }
}
