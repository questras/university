package onp;

import java.util.ArrayList;
import java.util.Arrays;

public class Main {
    public static void test(String[] data, int expectedResult)
            throws UnknownSymbolException,
            TooComplexExpressionException,
            TooFewOperatorsException,
            TooManyOperatorsException {

        int result = new ONP().oblicz(data);

        if (result == expectedResult) {
            System.out.print("OK ");
        }
        else {
            System.out.print("FAILED: ");
        }

        System.out.println("Data: " + Arrays.toString(data) + " Actual: " + result + " Expected: " + expectedResult);
    }

    public static void main(String[] args)
            throws UnknownSymbolException,
            TooComplexExpressionException,
            TooFewOperatorsException,
            TooManyOperatorsException {

        // Correct tests
        String[] test1 = {"2", "3", "+", "4", "*"};
        test(test1, 20);
        String[] test2 = {"3", "5", "2", "*", "+"};
        test(test2, 13);
        String[] test3 = {"2", "2", "+"};
        test(test3, 4);
        String[] test4 = {"7", "3", "+", "5", "2", "+", "2", "*", "*"};
        // (7 + 3) * ((5 + 2) * 2)
        test(test4, 140);

        // Exceptions tests

        // TooComplexExpressionException
        ArrayList<String> test5 = new ArrayList<>();
        for (int i = 0; i < 200; i++) {
            test5.add(Integer.toString(i));
        }
        try {
            new ONP().oblicz(test5.toArray(String[]::new));
            System.out.println("FAILED - expected exception.");
        } catch (TooComplexExpressionException e) {
            System.out.println("OK - TooComplexExpressionException.");
        }

        // TooManyOperatorsException
        String[] test6 = {"2", "2", "+", "+"};
        try {
            new ONP().oblicz(test6);
            System.out.println("FAILED - expected exception.");
        } catch (TooManyOperatorsException e) {
            System.out.println("OK - TooManyOperatorsException.");
        }

        // TooFewOperatorsException
        String[] test7 = {"2", "2", "2", "+"};
        try {
            new ONP().oblicz(test7);
            System.out.println("FAILED - expected exception.");
        } catch (TooFewOperatorsException e) {
            System.out.println("OK - TooFewOperatorsException.");
        }

        // UnknownSymbolException
        String[] test8 = {"2", "2", "2", "+", "w"};
        try {
            new ONP().oblicz(test8);
            System.out.println("FAILED - expected exception.");
        } catch (UnknownSymbolException e) {
            System.out.println("OK - UnknownSymbolException.");
        }
    }
}
