package cover;

import java.util.ArrayList;
import java.util.Collections;


public class AccurateCover extends CoverAlgorithm {

    /**
     * Get this set which index list is lexicographically first
     * according to numbers. Assuming both sets have the same length.
     *
     * @param set1 - set to compare,
     * @param set2 - set to compare.
     * @return Set that is lexicographically first.
     */
    private FamilyOfSets getLexicographicallyFirst(FamilyOfSets set1,
                                                   FamilyOfSets set2) {
        int n = set1.getSize();
        ArrayList<Integer> set1Indexes = set1.getSetsIndexes();
        ArrayList<Integer> set2Indexes = set2.getSetsIndexes();

        Collections.sort(set1Indexes);
        Collections.sort(set2Indexes);

        for (int i = 0; i < n; i++) {
            if (set1Indexes.get(i) < set2Indexes.get(i)) {
                return set1;
            }
            else if (set1Indexes.get(i) > set2Indexes.get(i)) {
                return set2;
            }
        }

        // Sets are equal lexicographically.
        return set1;
    }

    @Override
    protected FamilyOfSets findCoveringSets(FiniteSeries toCover,
                                            FamilyOfSets familyOfSets) {
        ArrayList<Set> sets = familyOfSets.getSets();
        int setsSize = sets.size();

        FamilyOfSets bestSets = new FamilyOfSets();
        FamilyOfSets currentSets;

        // Find all subsets of [familyOfSets] using binary representation
        // of numbers.
        // https://www.geeksforgeeks.org/finding-all-subsets-of-a-given-set-in-java/
        for (long i = 0; i < (1L << setsSize); i++) {
            currentSets = new FamilyOfSets();

            for (int bit = 0; bit < setsSize; bit++) {
                if ((i & (1L << bit)) != 0) {
                    currentSets.add(sets.get(bit));
                }
            }

            // Check if [currentSets] are more optimal than [bestSets].
            if (this.isCovered(toCover, currentSets)) {
                int currentSize = currentSets.getSize();
                int bestSize = bestSets.getSize();

                if (currentSize < bestSize || bestSize == 0) {
                    bestSets = currentSets;
                }
                else if (currentSize == bestSize) {
                    bestSets =
                            getLexicographicallyFirst(bestSets, currentSets);
                }
            }
        }

        return bestSets;
    }
}
