package cpsc2150.lab2;
import java.io.*;
import java.util.*;
import java.scanner;

public class MatrixApp {
    public static void main(String [] args) {

        System.out.println("How many rows should your matrix have?\n");
        Scanner scanner = new Scanner(System.in);
        int row = scanner.nextLine();
        System.out.println("How many columns should your matrix have?\n");
        int col = scanner.nextLine();

        int [row][col] matrix;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                System.out.println("What number should go in Row: " + i + " Col: " + j);
                matrix[i][j] = scanner.nextLine();
            }
        }

        System.out.println("Your matrix is: \n");
        matrix.print();

        matrix.transpose();
        System.out.println("The transposed matrix is: \n");
        matrix.print();

        matrix.productSum();
        System.out.println("The product sum is: \n");
        matrix.print();

        matrix.average();
        System.out.println("The average is: \n");
        matrix.print();

        matrix.rowSum();
        System.out.println("The sums of each Row are: \n");
        matrix.print();

        matrix.colSum();
        System.out.println("The sums of each Column are: \n");
        matrix.print();
    }

    private void print() {
        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                System.out.print("|" + matrix[i][j]);
            }
            System.out.println("|");
            System.out.println("\n");
        }
    }

    private void transpose() {
        int[][] temp = new int[col][row];
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                temp[i][j] = matrix[i][j];
            }
        }
        matrix = temp;
    }

    private int productSum() {
        int sum = 0;
        int product = 1;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                sum += matrix[j];
            }
            product = product * sum;
        }
        return product;
    }

    private double average() {
        int rowsum, totalsum, avg;
        rowsum, totalsum, avg = 0;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                rowsum += matrix[j];
            }
            totalsum += rowsum;
        }
        avg = totalsum / matrix.size();
        return avg;
    }

    private int[] rowSum() {
        int sum = 0;
        int [row]arr;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                sum += matrix[j];
            }
            arr[i] = sum;
        }
        return arr;
    }

    private int[] colSum() {
        int sum = 0;
        int [col]arr;
        for (int i = 0; i < col; i++) {
            for (int j = 0; j < row; j++) {
                sum += matrix[j];
            }
            arr[i] = sum;
        }
        return arr;
    }
}
