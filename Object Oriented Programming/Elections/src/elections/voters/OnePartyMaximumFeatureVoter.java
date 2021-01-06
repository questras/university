package elections.voters;

import elections.Candidate;
import elections.voters.MaximumFeatureVoter;

import java.util.ArrayList;
import java.util.Map;


public class OnePartyMaximumFeatureVoter extends MaximumFeatureVoter {
    private final String partyToVote;

    public OnePartyMaximumFeatureVoter(String name, String surname,
                                       int electoralDistrictNumber,
                                       int desiredFeature,
                                       String partyToVote) {

        super(name, surname, electoralDistrictNumber, desiredFeature);
        this.partyToVote = partyToVote;
    }


    /**
     * Choose candidate from candidatesByParty with greatest voter's
     * desired feature that is in voter's party to vote.
     *
     * @param candidatesByParty - candidates grouped by parties.
     * @return Candidate from voter's party to vote with greatest
     * voter's desired feature.
     */
    @Override
    protected Candidate chooseCandidate(
            Map<String, ArrayList<Candidate>> candidatesByParty
    ) {

        ArrayList<Candidate> partyCandidates =
                candidatesByParty.get(this.partyToVote);
        return this.chooseCandidateBasedOnFeature(partyCandidates);
    }
}
