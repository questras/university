package elections.votesCountingMethods;

import java.util.*;


public class DHondtMethod extends QuotientMethod {

    /**
     * In D'Hondt Method dividers are all numbers from
     * 1 to numberOfMandates.
     *
     * @param numberOfMandates - number of mandates to give in elections.
     * @return Dividers for D'Hondt Method.
     */
    @Override
    protected ArrayList<Integer> getDividers(int numberOfMandates) {
        ArrayList<Integer> dividers = new ArrayList<>();

        for (int i = 1; i <= numberOfMandates; i++) {
            dividers.add(i);
        }

        return dividers;
    }


    @Override
    public String toString() {
        return "D'Hondt Method.";
    }
}
