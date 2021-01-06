package cover;

public class FiniteSeries implements Element {
    private final int firstValue;
    private final int lastValue;
    private final int step;

    public FiniteSeries(int firstValue, int lastValue, int step) {
        this.firstValue = firstValue;
        this.lastValue = lastValue;
        this.step = step;
    }

    public int getFirstValue() {
        return firstValue;
    }

    public int getLastValue() {
        return lastValue;
    }

    public int getStep() {
        return step;
    }

    @Override
    public boolean contains(int number) {
        if (number < this.firstValue || number > this.lastValue) {
            return false;
        }

        return (number - this.firstValue) % this.step == 0;
    }

}
