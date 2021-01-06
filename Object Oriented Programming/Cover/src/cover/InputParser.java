package cover;

import java.util.ArrayList;


public class InputParser {
    private final ArrayList<Integer> input;
    private ArrayList<Set> parsedSets;
    private ArrayList<Query> parsedQueries;

    public InputParser(ArrayList<Integer> input) {
        this.input = input;
        this.parsedSets = new ArrayList<>();
        this.parsedQueries = new ArrayList<>();
    }

    public ArrayList<Query> getParsedQueries() {
        return this.parsedQueries;
    }

    public ArrayList<Set> getParsedSets() {
        return this.parsedSets;
    }

    private boolean matchFiniteSeries(int a, int b, int c) {
        return (a > 0) && (b < 0) && (c < 0);
    }

    private boolean matchInfiniteSeries(int a, int b) {
        return (a > 0) && (b < 0);
    }

    /**
     * Create a set with setIndex from numbers in input
     * from start to stop.
     *
     * @param setIndex - index for new set,
     * @param start    - index of first element of set in input,
     * @param stop     - index of last element of set in input,
     * @return Set created from numbers on given positions in input.
     */
    private Set parseInputToSet(int setIndex, int start, int stop) {
        Set newSet = new Set(setIndex);

        while (start <= stop) {
            if (stop - start + 1 >= 3) {
                int firstValue = this.input.get(start);
                int step = this.input.get(start + 1);
                int lastValue = this.input.get(start + 2);

                if (this.matchFiniteSeries(firstValue, lastValue, step)) {
                    newSet.add(
                            new FiniteSeries(firstValue, -lastValue, -step)
                    );
                    start += 3;
                    continue;
                }
            }

            if (stop - start + 1 >= 2) {
                int firstValue = this.input.get(start);
                int step = this.input.get(start + 1);

                if (this.matchInfiniteSeries(firstValue, step)) {
                    newSet.add(new InfiniteSeries(firstValue, -step));
                    start += 2;
                    continue;
                }
            }

            newSet.add(new Number(this.input.get(start)));
            start++;
        }

        return newSet;
    }

    /**
     * Create a query after set with @p lastSetIndex.
     *
     * @param lastSetIndex - index of last set before created query,
     * @param start        - index of the beginning of query in input.
     * @return Query created from input from @p start index.
     */
    private Query parseInputToQuery(int lastSetIndex, int start) {
        int toCoverRange = -(this.input.get(start));
        // toCover is {1,2,3,...,toCoverRange} Finite series.
        FiniteSeries toCover = new FiniteSeries(1, toCoverRange, 1);
        int algorithm = input.get(start + 1);

        return new Query(algorithm, toCover, lastSetIndex);
    }

    /**
     * Parse @p this.input to @p this.parsedSets and @p this.parsedQueries.
     */
    public void parseInput() {
        this.parsedSets = new ArrayList<>();
        this.parsedQueries = new ArrayList<>();

        int currentSetIndex = 1;
        int start = 0;
        int stop = this.input.size();

        while (start < stop) {
            if (this.input.get(start) < 0) {
                this.parsedQueries.add(
                        this.parseInputToQuery(currentSetIndex - 1, start)
                );
                start += 2;
            }
            else {
                int setEnd = start;
                while (input.get(setEnd) != 0) {
                    setEnd++;
                }

                this.parsedSets.add(
                        this.parseInputToSet(currentSetIndex, start, setEnd - 1)
                );
                currentSetIndex++;
                start = setEnd + 1;
            }
        }
    }
}
