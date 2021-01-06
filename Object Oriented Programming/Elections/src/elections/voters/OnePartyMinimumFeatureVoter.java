package elections.voters;

import elections.Candidate;
import elections.voters.MinimumFeatureVoter;

import java.util.ArrayList;
import java.util.Map;


public class OnePartyMinimumFeatureVoter extends MinimumFeatureVoter {
    private String partyToVote;

    public OnePartyMinimumFeatureVoter(String name, String surname,
                                       int electoralDistrictNumber,
                                       int desiredFeature,
                                       String partyToVote) {

        super(name, surname, electoralDistrictNumber, desiredFeature);
        this.partyToVote = partyToVote;
    }


    /**
     * Choose candidate from candidatesByParty with smallest voter's
     * desired feature that is in voter's party to vote.
     *
     * @param candidatesByParty - candidates grouped by parties.
     * @return Candidate from voter's party to vote with smallest
     * voter's desired feature.
     */
    @Override
    protected Candidate chooseCandidate(
            Map<String, ArrayList<Candidate>> candidatesByParty) {

        ArrayList<Candidate> partyCandidates =
                candidatesByParty.get(this.partyToVote);
        return this.chooseCandidateBasedOnFeature(partyCandidates);
    }
}
