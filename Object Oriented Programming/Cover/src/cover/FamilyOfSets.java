package cover;

import java.util.ArrayList;

public class FamilyOfSets {
    private final ArrayList<Set> sets;

    public FamilyOfSets() {
        this.sets = new ArrayList<>();
    }

    public Set getSet(int index) {
        return this.sets.get(index);
    }

    public ArrayList<Set> getSets() {
        return this.sets;
    }

    public int getSize() {
        return this.sets.size();
    }

    public ArrayList<Integer> getSetsIndexes() {
        ArrayList<Integer> indexes = new ArrayList<>();
        for (Set set : this.sets) {
            indexes.add(set.getIndex());
        }

        return indexes;
    }

    public void add(Set set) {
        this.sets.add(set);
    }

    /**
     * Check if any set contains number.
     *
     * @param number - number to check,
     * @return True if there is set that contains number, false otherwise.
     */
    public boolean contains(int number) {
        for (Set set : this.sets) {
            if (set.contains(number)) {
                return true;
            }
        }
        return false;
    }
}