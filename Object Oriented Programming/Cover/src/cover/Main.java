package cover;

import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {
        // Read and parse data from stdin.
        ArrayList<Integer> input = InputReader.readData();
        InputParser inputParser = new InputParser(input);
        inputParser.parseInput();

        // Get parsed sets and queries.
        ArrayList<Set> parsedSets = inputParser.getParsedSets();
        ArrayList<Query> parsedQueries = inputParser.getParsedQueries();

        FamilyOfSets currentSets = new FamilyOfSets();
        int currentIndex = 0;

        // Run queries on loaded sets.
        for (Query query : parsedQueries) {
            int lastIndex = query.getLastSetIndexBeforeQuery();
            Set currentSet = parsedSets.get(currentIndex);

            while (currentSet.getIndex() < lastIndex) {
                currentSets.add(currentSet);
                currentIndex++;
                currentSet = parsedSets.get(currentIndex);
            }
            // Add set with index equal to [lastIndex].
            currentSets.add(currentSet);

            query.runAndPrintResult(currentSets);
        }
    }
}
