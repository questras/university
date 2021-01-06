package vektor;

import java.util.ArrayList;

class DifferentLengthException extends RuntimeException {
    public DifferentLengthException(Vektor v1, Vektor v2) {
        super("Different length of vectors: " + v1.getLength() + " and " + v2.getLength());
    }
}

public class Vektor<T extends Algebraic<T>> {
    private final ArrayList<T> elements;

    public Vektor(ArrayList<T> elements) {
        this.elements = elements;
    }

    public int getLength() {
        return this.elements.size();
    }

    public T getElement(int index) {
        if (index >= 0 && index < this.getLength()) {
            return this.elements.get(index);
        }

        return null;
    }

    public Vektor<T> add(Vektor<T> other) {
        if (this.getLength() != other.getLength()) {
            throw new DifferentLengthException(this, other);
        }

        ArrayList<T> newVektorElements = new ArrayList<>();
        int length = this.getLength();

        for (int i = 0; i < length; i++) {
            T newElement = this.getElement(i).add(other.getElement(i));
            newVektorElements.add(newElement);
        }

        return new Vektor<T>(newVektorElements);
    }

    public T scalarProduct(Vektor<T> other) {
        if (this.getLength() != other.getLength()) {
            throw new DifferentLengthException(this, other);
        }
        else if (this.getLength() == 0) {
            return null;
        }

        T sumOfProducts = this.getElement(0).multiply(other.getElement(0));
        int length = this.getLength();

        for (int i = 1; i < length; i++) {
            sumOfProducts = sumOfProducts.add(this.getElement(i).multiply(other.getElement(i)));
        }

        return sumOfProducts;
    }

    @Override
    public String toString() {
        return "Vektor{" +
                "elements=" + elements +
                '}';
    }
}
