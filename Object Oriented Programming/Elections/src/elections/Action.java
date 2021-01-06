package elections;

import java.util.ArrayList;


public class Action {
    private final ArrayList<Integer> description;
    private int cost;

    public Action(ArrayList<Integer> description) {
        this.description = description;
        this.cost = 0;

        // Cost of action is sum of absolute values in description.
        for (int number : this.description) {
            this.cost += Math.abs(number);
        }
    }

    public ArrayList<Integer> getDescription() {
        return new ArrayList<>(this.description);
    }

    public int getCost() {
        return this.cost;
    }
}
