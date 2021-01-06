package com.company;

class Rational {
    private int numerator;
    private int denominator;

    public static int abs(int a) {
        if (a < 0) {
            return -a;
        }
        return a;
    }

    // Greatest common divisor of numbers a and b.
    public static int gcd(int a, int b) {
        if (b == 0) {
            return a;
        }

        return gcd(b, a % b);
    }

    public Rational(int n) {
        this.numerator = n;
        this.denominator = 1;
    }

    public Rational(int p, int q) {
        if (q == 0) {
            throw new ArithmeticException();
        }

        this.numerator = p;
        this.denominator = q;
        this.reduce();

        if (this.denominator < 0) {
            this.numerator *= -1;
            this.denominator *= -1;
        }
    }

    public static Rational zero() {
        return new Rational(0);
    }

    public static Rational one() {
        return new Rational(1);
    }

    public int numerator() {
        return this.numerator;
    }

    public int denominator() {
        return this.denominator;
    }

    public int sign() {
        if (this.numerator == 0) {
            return 0;
        }
        else if (this.numerator * this.denominator < 0) {
            return -1;
        }
        else {
            return 1;
        }
    }

    private void reduce() {
        int divisor = gcd(this.numerator, this.denominator);
        if (divisor != 1) {
            this.numerator /= divisor;
            this.denominator /= divisor;
            this.reduce();
        }
    }

    public Rational add(Rational number) {
        if (this.denominator != number.denominator()) {
            int new_denominator = this.denominator * number.denominator();
            int numerator1 = this.numerator * number.denominator();
            int numerator2 = number.numerator() * this.denominator;
            int new_nominator = numerator1 + numerator2;

            return new Rational(new_nominator, new_denominator);
        }
        else {
            return new Rational(this.numerator + number.numerator(), this.denominator);
        }
    }

    public Rational subtract(Rational number) {
        return this.add(number.opposite());
    }

    public Rational multiply(Rational number) {
        int new_numerator = this.numerator * number.numerator();
        int new_denominator = this.denominator * number.denominator();

        return new Rational(new_numerator, new_denominator);
    }

    public Rational divide(Rational number) {
        return this.multiply(number.inverse());
    }

    public Rational opposite() {
        return new Rational(-this.numerator, this.denominator);
    }

    public Rational inverse() {
        return new Rational(this.denominator, this.numerator);
    }

    int compareTo(Rational number) {
        if (this.sign() > number.sign()) {
            return 1;
        }
        else if (this.sign() < number.sign()) {
            return -1;
        }
        else if (number.denominator() == this.denominator) {
            if (this.numerator > number.numerator()) {
                return 1;
            }
            else if (this.numerator < number.numerator()) {
                return -1;
            }
            else {
                return 0;
            }
        }
        else if (number.numerator() == this.numerator) {
            if (this.denominator > number.denominator()) {
                return -1;
            }
            else if (this.denominator < number.denominator()) {
                return 1;
            }
            else {
                return 0;
            }
        }
        else {
            int numerator1 = this.numerator * abs(number.denominator());
            int numerator2 = number.numerator() * abs(this.denominator);

            if (numerator1 > numerator2) {
                return 1;
            }
            else if (numerator1 < numerator2) {
                return -1;
            }
            else {
                return 0;
            }
        }
    }

    public String toString() {
        return this.numerator + "/" + this.denominator;
    }


}

public class Main {

    public static void main(String[] args) {
        // Tests
        // Constructor
        System.out.println("Constructor:");
        Rational r1 = new Rational(5, 4);
        System.out.println(r1.toString());
        Rational r2 = new Rational(3);
        System.out.println(r2.toString());
        Rational r3 = new Rational(8, 6);
        System.out.println(r3.toString());
        Rational r4 = new Rational(8, -6);
        System.out.println(r4.toString());
        Rational r5 = new Rational(-4, 3);
        System.out.println(r5.toString());
        Rational r6 = new Rational(-4, -3);
        System.out.println(r6.toString());
        Rational r7 = new Rational(-4, -2);
        System.out.println(r7.toString());

        Rational ans;
        Rational correct_ans;

        // Adding
        System.out.println("\nAdding");
        ans = r1.add(new Rational(6, 5));
        correct_ans = new Rational(49, 20);
        System.out.println(ans.toString().equals(correct_ans.toString()));

        // subtracting
        System.out.println("\nSubtracting");
        ans = r1.subtract(new Rational(6, 5));
        correct_ans = new Rational(1, 20);
        System.out.println(ans.toString().equals(correct_ans.toString()));

        // multiplying
        System.out.println("\nMultiplying");
        ans = r1.multiply(new Rational(6, 5));
        correct_ans = new Rational(3, 2);
        System.out.println(ans.toString().equals(correct_ans.toString()));

        // Dividing
        System.out.println("\nDividing");
        ans = r1.divide(new Rational(6, 5));
        correct_ans = new Rational(25, 24);
        System.out.println(ans.toString().equals(correct_ans.toString()));

        // Inverse
        System.out.println("\nInverse");
        ans = r1.inverse();
        correct_ans = new Rational(4, 5);
        System.out.println(ans.toString().equals(correct_ans.toString()));

        // Opposite
        System.out.println("\nOpposite");
        ans = r1.opposite();
        correct_ans = new Rational(-5, 4);
        System.out.println(ans.toString().equals(correct_ans.toString()));

        // Numerator
        System.out.println("\nNumerator");
        System.out.println(r1.numerator() == 5);

        // Denominator
        System.out.println("\nDenominator");
        System.out.println(r1.denominator() == 4);

        // Sign
        System.out.println("\nSign");
        System.out.println(r1.sign() == 1);
        System.out.println(Rational.zero().sign() == 0);
        System.out.println(r4.sign() == -1);

        // Compare to
        System.out.println("\ncompareTo");
        System.out.println(r1.compareTo(r4) == 1);
        System.out.println(r1.compareTo(r3) == -1);
        System.out.println(r1.compareTo(r1) == 0);

        // exceptions
//        Rational err = new Rational(5, 0);
//        r1.divide(new Rational(0, 5));
//        Rational.zero().inverse();
    }
}
