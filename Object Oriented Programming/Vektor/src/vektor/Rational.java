package vektor;

public class Rational implements Algebraic<Rational> {
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
