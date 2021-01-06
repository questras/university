package cover;

import java.util.ArrayList;

public class NaiveCover extends CoverAlgorithm {
    @Override
    protected FamilyOfSets findCoveringSets(FiniteSeries toCover,
                                            FamilyOfSets familyOfSets) {
        FamilyOfSets currentSets = new FamilyOfSets();
        ArrayList<Set> sets = familyOfSets.getSets();

        int start = toCover.getFirstValue();
        int stop = toCover.getLastValue();
        int step = toCover.getStep();

        // Take every set from [sets] that contains a number
        // from [toCover] that is not in [currentSets].
        for (Set set : sets) {
            for (int number = start; number <= stop; number += step) {
                if (set.contains(number) && !currentSets.contains(number)) {
                    currentSets.add(set);
                    break;
                }
            }
        }

        return currentSets;
    }
}
