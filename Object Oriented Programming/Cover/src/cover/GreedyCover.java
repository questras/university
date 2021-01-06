package cover;

import java.util.ArrayList;


public class GreedyCover extends CoverAlgorithm {

    /**
     * Count how many elements from toCover are in set
     * and are not in currentSets.
     *
     * @param set         - set to count elements in,
     * @param currentSets - sets with not counted elements,
     * @param toCover     - series to count elements from.
     * @return Count of how many elements from toCover are in set
     * and are not in currentSets.
     */
    private int countOccurrencesInSet(Set set,
                                      FamilyOfSets currentSets,
                                      FiniteSeries toCover) {
        int start = toCover.getFirstValue();
        int stop = toCover.getLastValue();
        int step = toCover.getStep();

        int count = 0;
        for (int number = start; number <= stop; number += step) {
            if (set.contains(number) && !currentSets.contains(number)) {
                count++;
            }
        }

        return count;
    }

    /**
     * Count how many elements from toCover are in
     * each set of familyOfSets and are not in currentSets.
     *
     * @param familyOfSets - sets to count elements in,
     * @param currentSets  - sets with not counted elements,
     * @param toCover      - series to count elements from.
     * @return Array with counts of how many elements from toCover
     * are in each set of familyOfSets and are not in currentSets.
     */
    private int[] countOccurrencesInFamilyOfSets(FamilyOfSets familyOfSets,
                                                 FamilyOfSets currentSets,
                                                 FiniteSeries toCover) {
        ArrayList<Set> sets = familyOfSets.getSets();
        int setsSize = sets.size();

        int[] occurrences = new int[setsSize];
        for (int i = 0; i < setsSize; i++) {
            occurrences[i] =
                    countOccurrencesInSet(sets.get(i), currentSets, toCover);
        }

        return occurrences;
    }

    /**
     * Get set from familyOfSets that has the largest amount of elements
     * from toCover that are not in currentSets.
     *
     * @param familyOfSets - sets to choose the best set from,
     * @param currentSets  - sets with not counted elements,
     * @param toCover      - series to count elements from.
     * @return Set from familyOfSets that has the largest amount of elements
     * from toCover that are not in currentSets.
     */
    private Set getSetWithBestCount(FamilyOfSets familyOfSets,
                                    FamilyOfSets currentSets,
                                    FiniteSeries toCover) {
        int[] occurrences =
                countOccurrencesInFamilyOfSets(familyOfSets, currentSets, toCover);
        int bestCount = 0;
        int bestSetIndex = -1;
        for (int i = 0; i < occurrences.length; i++) {
            if (occurrences[i] > bestCount) {
                bestSetIndex = i;
                bestCount = occurrences[i];
            }
        }

        if (bestSetIndex != -1) {
            return familyOfSets.getSet(bestSetIndex);
        }

        return null;
    }

    @Override
    protected FamilyOfSets findCoveringSets(FiniteSeries toCover,
                                            FamilyOfSets familyOfSets) {
        FamilyOfSets coveringSets = new FamilyOfSets();

        Set setToAdd = getSetWithBestCount(familyOfSets, coveringSets, toCover);
        while (setToAdd != null) {
            coveringSets.add(setToAdd);
            setToAdd = getSetWithBestCount(familyOfSets, coveringSets, toCover);
        }

        return coveringSets;
    }
}