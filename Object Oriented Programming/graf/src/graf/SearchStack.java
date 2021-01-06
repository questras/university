package graf;

import java.util.Stack;

public class SearchStack<T> implements SearchStructure<T> {

    private Stack<T> stack;

    public SearchStack() {
        this.stack = new Stack<>();
    }

    @Override
    public T pop() {
        return this.stack.pop();
    }

    @Override
    public void push(T element) {
        this.stack.add(element);
    }

    @Override
    public boolean isEmpty() {
        return this.stack.isEmpty();
    }
}
