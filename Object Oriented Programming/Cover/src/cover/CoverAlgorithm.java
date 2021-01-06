package cover;

import java.util.ArrayList;
import java.util.Collections;

public abstract class CoverAlgorithm {
    /**
     * Check if familyOfSets covers toCover.
     *
     * @param toCover      - series to cover,
     * @param familyOfSets - sets that should cover the series,
     * @return True if toCover is covered by familyOfSets,
     * False otherwise.
     */
    protected boolean isCovered(FiniteSeries toCover,
                                FamilyOfSets familyOfSets) {
        int start = toCover.getFirstValue();
        int stop = toCover.getLastValue();
        int step = toCover.getStep();

        // Check if all numbers from [toCover] are in [currentSets]
        for (int number = start; number <= stop; number += step) {
            if (!familyOfSets.contains(number)) {
                return false;
            }
        }

        return true;
    }

    /**
     * Get indexes of sets in familyOfSets that cover toCover or
     * 0 if toCover cannot be covered by familyOfSets.
     *
     * @param toCover      - series to cover,
     * @param familyOfSets - sets to cover the series,
     * @return List with indexes of sets that cover toCover or
     * list with only 0 if toCover cannot be covered.
     */
    public ArrayList<Integer> cover(FiniteSeries toCover,
                                    FamilyOfSets familyOfSets) {
        FamilyOfSets coveringSets = this.findCoveringSets(toCover, familyOfSets);
        ArrayList<Integer> resultIndexes = new ArrayList<>();

        if (isCovered(toCover, coveringSets)) {
            resultIndexes = coveringSets.getSetsIndexes();
            Collections.sort(resultIndexes);
        }
        else {
            resultIndexes.add(0);
        }

        return resultIndexes;
    }

    /**
     * Get those sets from familyOfSets that cover toCover as much
     * as possible.
     *
     * @param toCover      - series to cover,
     * @param familyOfSets - sets to cover the series.
     * @return Family of sets that cover toCover as much as possible.
     */
    protected abstract FamilyOfSets findCoveringSets(FiniteSeries toCover,
                                                     FamilyOfSets familyOfSets);


}
