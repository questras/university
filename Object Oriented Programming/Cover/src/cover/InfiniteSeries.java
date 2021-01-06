package cover;

public class InfiniteSeries implements Element {
    private final int firstValue;
    private final int step;

    public InfiniteSeries(int firstValue, int step) {
        this.firstValue = firstValue;
        this.step = step;
    }

    @Override
    public boolean contains(int number) {
        if (number < this.firstValue) {
            return false;
        }

        return (number - this.firstValue) % this.step == 0;
    }
}