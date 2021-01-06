package graf;

import java.util.LinkedList;
import java.util.Queue;

public class SearchQueue<T> implements SearchStructure<T> {

    private Queue<T> queue;

    public SearchQueue() {
        this.queue = new LinkedList<>();
    }

    @Override
    public T pop() {
        return this.queue.remove();
    }

    @Override
    public void push(T element) {
        this.queue.add(element);
    }

    @Override
    public boolean isEmpty() {
        return this.queue.isEmpty();
    }
}

