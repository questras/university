package elections.parties;

import elections.*;
import elections.voters.Voter;

import java.util.ArrayList;
import java.util.Collections;


public class GreedyParty extends Party {

    public GreedyParty(String name, int budget) {
        super(name, budget);
    }

    /**
     * Get best ActionChoice choosing from actions and districts
     * based on greedy strategy i.e choosing ActionChoice that
     * maximizes sum of weighted sums of party's candidates in
     * chosen district.
     *
     * @param actions   - actions to choose from,
     * @param districts - districts to choose from.
     * @return Best ActionChoice based on greedy strategy.
     */
    @Override
    protected ActionChoice getBestActionChoice(
            ArrayList<Action> actions,
            ArrayList<ElectoralDistrict> districts
    ) {

        ArrayList<ActionChoice> affordableActionChoices =
                this.getAffordableActionChoices(actions, districts);

        ActionChoice bestActionChoice = null;
        int bestSumOfWeightedSumRaise = 0;

        // Get best ActionChoice from affordableActionChoices.
        for (ActionChoice choice : affordableActionChoices) {
            // Count initial sum of weighted sums of party's candidates.
            ArrayList<ArrayList<Integer>> votersWeightsList =
                    this.getWeightsListOfVoters(
                            choice.getDistrict().getAllVoters()
                    );

            ArrayList<ArrayList<Integer>> partyCandidatesFeaturesList =
                    this.getFeaturesListsOfCandidates(
                            choice.getDistrict().getCandidatesFromParty(
                                    this.name
                            )
                    );

            int initialSumOfWeightedSums =
                    this.calculateSumOfWeightedSums(
                            votersWeightsList,
                            partyCandidatesFeaturesList
                    );

            // Calculate influenced weights of each voter.
            ArrayList<ArrayList<Integer>> votersInfluencedWeightsList =
                    new ArrayList<>();

            for (ArrayList<Integer> votersWeights : votersWeightsList) {
                votersInfluencedWeightsList.add(
                        this.calculateInfluencedWeights(
                                votersWeights,
                                choice.getAction().getDescription()
                        )
                );
            }

            int influencedSumOfWeightedSums =
                    this.calculateSumOfWeightedSums(
                            votersInfluencedWeightsList,
                            partyCandidatesFeaturesList
                    );

            // Calculate raise of sum after influence.
            int sumOfWeightedSumsRaise =
                    influencedSumOfWeightedSums - initialSumOfWeightedSums;

            if (bestActionChoice == null ||
                    sumOfWeightedSumsRaise > bestSumOfWeightedSumRaise) {

                bestActionChoice = choice;
                bestSumOfWeightedSumRaise = sumOfWeightedSumsRaise;
            }
        }

        return bestActionChoice;
    }


    /**
     * Get weights list of each voter in voters, who has weights list.
     *
     * @param voters - list of voters to take weights from.
     * @return List of weights of each voter.
     */
    private ArrayList<ArrayList<Integer>> getWeightsListOfVoters(
            ArrayList<Voter> voters
    ) {

        ArrayList<ArrayList<Integer>> votersWeightsList =
                new ArrayList<>();

        for (Voter voter : voters) {
            votersWeightsList.add(voter.getWeights());
        }
        // Remove all null objects corresponding to voters without weights.
        votersWeightsList.removeAll(Collections.singleton(null));

        return votersWeightsList;
    }


    /**
     * Get features list of each candidate in candidates.
     *
     * @param candidates - list of candidates to take features from.
     * @return List of features of each candidate.
     */
    private ArrayList<ArrayList<Integer>> getFeaturesListsOfCandidates(
            ArrayList<Candidate> candidates
    ) {

        ArrayList<ArrayList<Integer>> candidatesFeaturesList =
                new ArrayList<>();
        for (Candidate candidate : candidates) {
            candidatesFeaturesList.add(candidate.getFeatures());
        }

        return candidatesFeaturesList;
    }

    /**
     * For numbers(n1, n2, ...) and  weights(w1, w2, ...) calculate
     * weighted sum i.e w1*n1 + w2*n2 + ...
     *
     * @param numbers - numbers for weighted sum,
     * @param weights - weights for weighted sum.
     * @return Weighted sum of numbers and weights.
     */
    private int calculateWeightedSum(ArrayList<Integer> numbers,
                                     ArrayList<Integer> weights) {

        int sum = 0;

        for (int i = 0; i < numbers.size(); i++) {
            sum += weights.get(i) * numbers.get(i);
        }

        return sum;
    }


    /**
     * Calculate sum of weighted sums of all combinations of
     * lists of numbers in numbersList and lists of weights in weightsList.
     *
     * @param weightsList - list with lists of weights,
     * @param numbersList - list with lists of numbers,
     * @return Sum of weighted sums of all combinations of lists of numbers
     * in numbersList and lists of weights in weightsList.
     */
    private int calculateSumOfWeightedSums(
            ArrayList<ArrayList<Integer>> weightsList,
            ArrayList<ArrayList<Integer>> numbersList
    ) {

        int sumOfWeightedSums = 0;

        for (ArrayList<Integer> weights : weightsList) {
            for (ArrayList<Integer> numbers : numbersList) {
                sumOfWeightedSums +=
                        this.calculateWeightedSum(numbers, weights);
            }
        }

        return sumOfWeightedSums;
    }


    /**
     * Influence numbers in weights with numbers in influenceNumbers i.e
     * add number from influenceNumbers to corresponding weight in weights
     * without exceeding minimum and maximum values of weights.
     *
     * @param weights          - weights to influence,
     * @param influenceNumbers - numbers to influence with.
     * @return List of influenced weights.
     */
    private ArrayList<Integer> calculateInfluencedWeights(
            ArrayList<Integer> weights,
            ArrayList<Integer> influenceNumbers
    ) {

        int minWeight = Collections.min(weights);
        int maxWeight = Collections.max(weights);

        ArrayList<Integer> newWeights = new ArrayList<>();

        for (int i = 0; i < weights.size(); i++) {
            int newWeight = weights.get(i) + influenceNumbers.get(i);

            if (newWeight < minWeight) {
                newWeight = minWeight;
            }
            else if (newWeight > maxWeight) {
                newWeight = maxWeight;
            }

            newWeights.add(newWeight);
        }

        return newWeights;
    }
}
