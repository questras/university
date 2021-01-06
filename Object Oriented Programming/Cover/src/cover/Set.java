package cover;

import java.util.ArrayList;

public class Set {
    private final ArrayList<Element> elements;
    private final int index;

    public Set(int index) {
        this.index = index;
        this.elements = new ArrayList<>();
    }

    public int getIndex() {
        return index;
    }

    public void add(Element element) {
        this.elements.add(element);
    }

    /**
     * Check if any element contains number.
     *
     * @param number - number to check,
     * @return True if there is an element that contains number,
     * false otherwise.
     */
    public boolean contains(int number) {
        for (Element element : this.elements) {
            if (element.contains(number)) {
                return true;
            }
        }
        return false;
    }
}