package cover;

public class Number implements Element {
    private final int value;

    public Number(int value) {
        this.value = value;
    }

    @Override
    public boolean contains(int number) {
        return this.value == number;
    }
}