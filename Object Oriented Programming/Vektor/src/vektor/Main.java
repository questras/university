package vektor;

import java.util.ArrayList;
import java.util.Arrays;

public class Main {

    public static Vektor<AlgebraicInteger> fromArray(int[] array) {
        ArrayList<AlgebraicInteger> elements = new ArrayList<>();
        for (int number : array) {
            elements.add(new AlgebraicInteger(number));
        }

        return new Vektor<AlgebraicInteger>(elements);
    }

    public static Vektor<Rational> fromRationalArray(Rational[] array) {
        ArrayList<Rational> elements = new ArrayList<>(Arrays.asList(array));

        return new Vektor<Rational>(elements);
    }

    public static void main(String[] args) {
        //
	    // wektory rozmiaru 2
        //

        // AlgebraicInteger
        Vektor<AlgebraicInteger> v1 = fromArray(new int[]{1, 2});
        Vektor<AlgebraicInteger> v2 = fromArray(new int[]{5, 11});

        System.out.println("Dla: ");
        System.out.println(v1);
        System.out.println(v2);
        System.out.println("Wynik: ");
        System.out.println(v1.add(v2));
        System.out.println(v1.scalarProduct(v2));

        System.out.println("------------------------------");

        // Rational
        Rational[] v3Elements = {
                new Rational(1, 2),
                new Rational(3, 4)
        };
        Rational[] v4Elements = {
                new Rational(5, 6),
                new Rational(2)
        };
        Vektor<Rational> v3 = fromRationalArray(v3Elements);
        Vektor<Rational> v4 = fromRationalArray(v4Elements);

        System.out.println("Dla: ");
        System.out.println(v3);
        System.out.println(v4);
        System.out.println("Wynik: ");
        System.out.println(v3.add(v4));
        System.out.println(v3.scalarProduct(v4));

        System.out.println("------------------------------");

        //
        // wektory rozmiaru 2 i 3
        //

        // AlgebraicInteger
        Vektor<AlgebraicInteger> v5 = fromArray(new int[]{1, 2});
        Vektor<AlgebraicInteger> v6 = fromArray(new int[]{1, 2, 3});

        try {
            v5.add(v6);
        } catch (DifferentLengthException e) {
            System.out.println("błąd.");
        }

        // Rational
        Rational[] v7Elements = {
                new Rational(1, 2),
                new Rational(3, 4)
        };
        Rational[] v8Elements = {
                new Rational(5, 6),
                new Rational(2),
                new Rational(3)
        };
        Vektor<Rational> v7 = fromRationalArray(v7Elements);
        Vektor<Rational> v8 = fromRationalArray(v8Elements);
        try {
            v7.add(v8);
        } catch (DifferentLengthException e) {
            System.out.println("błąd.");
        }

    }
}
