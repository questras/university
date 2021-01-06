package elections.voters;

import elections.Candidate;
import elections.Action;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Map;


public class VersatileVoter extends Voter {
    private ArrayList<Integer> weights;

    public VersatileVoter(String name, String surname,
                          int electoralDistrictNumber,
                          ArrayList<Integer> weights) {

        super(name, surname, electoralDistrictNumber);
        this.weights = weights;
    }


    /**
     * Get weights of voter.
     *
     * @return Weights of the voter.
     */
    @Override
    public ArrayList<Integer> getWeights() {
        return new ArrayList<>(this.weights);
    }


    /**
     * Calculate weighted sum of candidate's features
     * with voter's weights.
     *
     * @param candidate - candidate to take features from.
     * @return Weighted sum of candidate's features with voter's weights.
     */
    public int calculateWeightedSumOfCandidate(Candidate candidate) {
        ArrayList<Integer> features = candidate.getFeatures();
        int sum = 0;

        for (int i = 0; i < features.size(); i++) {
            sum += this.weights.get(i) * features.get(i);
        }

        return sum;
    }


    /**
     * Get candidate from candidates with greatest value of
     * weighted sum of candidate's features with voter's weights.
     *
     * @param candidates - candidates to choose from.
     * @return Candidate with greatest value of weighted sum
     * of candidate's features with voter's weights.
     */
    protected Candidate chooseCandidateBasedOnWeightedSum(
            ArrayList<Candidate> candidates
    ) {

        ArrayList<Integer> candidatesWeightedSums = new ArrayList<>();

        // Calculate weighted sum of all candidates.
        for (Candidate candidate : candidates) {
            candidatesWeightedSums.add(this.calculateWeightedSumOfCandidate(candidate));
        }

        // Take weighted sum with greatest value.
        int bestWeightedSum = Collections.max(candidatesWeightedSums);


        ArrayList<Candidate> candidatesWithBestWeightedSum =
                new ArrayList<>();
        // Get all candidates with best weighted sum.
        for (int i = 0; i < candidates.size(); i++) {
            if (candidatesWeightedSums.get(i) == bestWeightedSum) {
                candidatesWithBestWeightedSum.add(candidates.get(i));
            }
        }

        return this.getRandomCandidate(candidatesWithBestWeightedSum);
    }


    /**
     * Get best candidate from candidatesByParty based on
     * versatile strategy i.e choose candidate from candidates
     * with greatest value of weighted sum of candidate's
     * features with voter's weights.
     *
     * @param candidatesByParty - candidates grouped by parties.
     * @return Best candidate based on versatile strategy.
     */
    @Override
    protected Candidate chooseCandidate(
            Map<String, ArrayList<Candidate>> candidatesByParty
    ) {

        // Get best candidate from each party.
        ArrayList<Candidate> bestCandidates = new ArrayList<>();
        for (ArrayList<Candidate> candidates : candidatesByParty.values()) {
            bestCandidates.add(
                    this.chooseCandidateBasedOnWeightedSum(candidates)
            );
        }

        // Return best candidate chosen from each party's best candidate.
        return this.chooseCandidateBasedOnWeightedSum(bestCandidates);
    }


    /**
     * Change voter's weights based on action.
     *
     * @param action - Action to be influenced with.
     */
    @Override
    public void getInfluenced(Action action) {
        int minWeight = Collections.min(this.weights);
        int maxWeight = Collections.max(this.weights);
        ArrayList<Integer> influenceNumbers = action.getDescription();

        for (int i = 0; i < weights.size(); i++) {
            int newWeight = weights.get(i) + influenceNumbers.get(i);

            if (newWeight < minWeight) {
                newWeight = minWeight;
            }
            else if (newWeight > maxWeight) {
                newWeight = maxWeight;
            }

            this.weights.set(i, newWeight);
        }
    }
}
