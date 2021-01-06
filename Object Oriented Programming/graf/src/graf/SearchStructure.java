package graf;

public interface SearchStructure<T> {
    T pop();
    void push(T element);
    boolean isEmpty();
}
