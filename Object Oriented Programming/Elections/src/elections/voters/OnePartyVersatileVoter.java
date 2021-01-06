package elections.voters;

import elections.Candidate;

import java.util.ArrayList;
import java.util.Map;


public class OnePartyVersatileVoter extends VersatileVoter {
    private final String partyToVote;

    public OnePartyVersatileVoter(String name, String surname,
                                  int electoralDistrictNumber,
                                  ArrayList<Integer> weights,
                                  String partyToVote) {

        super(name, surname, electoralDistrictNumber, weights);
        this.partyToVote = partyToVote;
    }


    /**
     * Get best candidate from candidatesByParty from voter's party to vote
     * based on versatile strategy.
     *
     * @param candidatesByParty - candidates grouped by parties.
     * @return Best candidate based on versatile strategy from voter's
     * party to vote.
     */
    @Override
    protected Candidate chooseCandidate(
            Map<String, ArrayList<Candidate>> candidatesByParty
    ) {
        ArrayList<Candidate> partyCandidates =
                candidatesByParty.get(this.partyToVote);
        return this.chooseCandidateBasedOnWeightedSum(partyCandidates);
    }
}
