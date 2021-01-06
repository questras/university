package vektor;

public class AlgebraicInteger implements Algebraic<AlgebraicInteger> {

    private int value;

    public AlgebraicInteger(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    @Override
    public AlgebraicInteger add(AlgebraicInteger other) {
        return new AlgebraicInteger(this.value + other.getValue());
    }

    @Override
    public AlgebraicInteger multiply(AlgebraicInteger other) {
        return new AlgebraicInteger(this.value * other.getValue());
    }

    @Override
    public String toString() {
        return Integer.toString(this.value);
    }
}
