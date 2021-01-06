package elections.voters;

import elections.Candidate;

import java.util.ArrayList;
import java.util.Map;


public abstract class FeatureVoter extends Voter {
    protected int desiredFeature;

    public FeatureVoter(String name, String surname,
                        int electoralDistrictNumber,
                        int desiredFeature) {

        super(name, surname, electoralDistrictNumber);
        this.desiredFeature = desiredFeature;
    }


    /**
     * Choose best feature from features based on voter's strategy
     * to choose features.
     *
     * @param features - features to choose from.
     * @return Best feature based on voter's strategy.
     */
    protected abstract int chooseBestFeature(ArrayList<Integer> features);


    /**
     * Choose best Candidate from candidates based on voter's desired
     * feature.
     *
     * @param candidates - Candidates to choose from.
     * @return best Candidate based on voter's desired feature.
     */
    protected Candidate chooseCandidateBasedOnFeature(
            ArrayList<Candidate> candidates
    ) {

        // Get desired feature of all candidates.
        ArrayList<Integer> desiredFeatureOfCandidates = new ArrayList<>();
        for (Candidate candidate : candidates) {
            desiredFeatureOfCandidates.add(
                    candidate.getFeatureWithNumber(this.desiredFeature)
            );
        }

        // Get feature with best value.
        int bestFeature = this.chooseBestFeature(desiredFeatureOfCandidates);

        // Get all candidates with feature with best value.
        ArrayList<Candidate> bestCandidates = new ArrayList<>();
        for (Candidate candidate : candidates) {
            int feature = candidate.getFeatureWithNumber(this.desiredFeature);
            if (feature == bestFeature) {
                bestCandidates.add(candidate);
            }
        }

        return this.getRandomCandidate(bestCandidates);
    }


    /**
     * Get best candidate from candidatesByParty based on
     * feature strategy i.e choose candidate with best value
     * of voter's desired feature.
     *
     * @param candidatesByParty - candidates grouped by parties.
     * @return Candidate with best value of voter's desired feature.
     */
    @Override
    protected Candidate chooseCandidate(
            Map<String, ArrayList<Candidate>> candidatesByParty
    ) {

        // Get best candidate from each party.
        ArrayList<Candidate> bestCandidates = new ArrayList<>();
        for (ArrayList<Candidate> candidates : candidatesByParty.values()) {
            bestCandidates.add(this.chooseCandidateBasedOnFeature(candidates));
        }

        // Return best candidate from each party's best candidate.
        return this.chooseCandidateBasedOnFeature(bestCandidates);
    }
}
