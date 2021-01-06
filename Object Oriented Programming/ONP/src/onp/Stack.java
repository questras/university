package onp;

class OverflowException extends Exception {
    public OverflowException() {
        super("Exceeded maximum size of stack.");
    }
}

class EmptyException extends Exception {
    public EmptyException() {
        super("Stack is empty.");
    }
}

public class Stack {
    private final int maxSize = 100;
    private final int[] elements;
    private int top_element_index;

    public Stack() {
        this.elements = new int[this.maxSize];
        this.top_element_index = -1;
    }

    public void add(int number) throws OverflowException {
        if (this.hasMaximumElements()) {
            throw new OverflowException();
        }

        this.top_element_index++;
        this.elements[this.top_element_index] = number;
    }

    public int pop() throws EmptyException {
        if (this.isEmpty()) {
            throw new EmptyException();
        }

        int popped_number = this.elements[this.top_element_index];
        this.top_element_index--;

        return popped_number;
    }

    private boolean hasMaximumElements() {
        return this.top_element_index == this.maxSize - 1;
    }

    public boolean isEmpty() {
        return this.top_element_index == -1;
    }

    public int getSize() {
        return this.top_element_index + 1;
    }

    public String toString() {
        if (this.isEmpty()) {
            return "Empty";
        }

        StringBuilder result = new StringBuilder("TOP\n");
        for (int i = this.top_element_index; i >= 0; i--) {
            result.append(this.elements[i]);
            result.append('\n');
        }

        return result.toString();
    }
}
