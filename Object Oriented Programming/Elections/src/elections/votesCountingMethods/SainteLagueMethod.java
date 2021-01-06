package elections.votesCountingMethods;

import java.util.*;


public class SainteLagueMethod extends QuotientMethod {

    /**
     * In Sainte-Lague method dividers are numberOfMandates odd numbers
     * counted from 1.
     *
     * @param numberOfMandates - number of mandates to give in elections.
     * @return Dividers for Sainte-Lague method.
     */
    @Override
    protected ArrayList<Integer> getDividers(int numberOfMandates) {
        ArrayList<Integer> dividers = new ArrayList<>();

        for (int i = 0; i < numberOfMandates; i++) {
            dividers.add((2 * i) + 1);
        }

        return dividers;
    }


    @Override
    public String toString() {
        return "Sainte-Laguë method.";
    }
}
