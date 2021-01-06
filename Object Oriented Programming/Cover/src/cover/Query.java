package cover;

import java.util.ArrayList;

public class Query {
    private final int algorithm;
    private final FiniteSeries toCover;
    private final int lastSetIndexBeforeQuery;

    public Query(int algorithm, FiniteSeries toCover,
                 int lastSetIndexBeforeQuery) {
        this.algorithm = algorithm;
        this.toCover = toCover;
        this.lastSetIndexBeforeQuery = lastSetIndexBeforeQuery;
    }

    public int getLastSetIndexBeforeQuery() {
        return this.lastSetIndexBeforeQuery;
    }

    /**
     * Run query's cover algorithm on given sets.
     *
     * @param familyOfSets - sets to run cover algorithm with.
     * @return List of indexes of sets from familyOfSets that
     * cover query's toCover series.
     */
    public ArrayList<Integer> run(FamilyOfSets familyOfSets) {
        switch (this.algorithm) {
            case 1:
                return new AccurateCover().cover(toCover, familyOfSets);
            case 2:
                return new GreedyCover().cover(toCover, familyOfSets);
            case 3:
                return new NaiveCover().cover(toCover, familyOfSets);
            default:
                return new ArrayList<>();
        }
    }

    /**
     * Run query's cover algorithm on given sets and print result.
     *
     * @param familyOfSets - sets to run cover algorithm with.
     */
    public void runAndPrintResult(FamilyOfSets familyOfSets) {
        ArrayList<Integer> result = this.run(familyOfSets);
        int resultSize = result.size();

        System.out.print(result.get(0));
        for (int i = 1; i < resultSize; i++) {
            System.out.print(" " + result.get(i));
        }
        System.out.println();
    }
}
