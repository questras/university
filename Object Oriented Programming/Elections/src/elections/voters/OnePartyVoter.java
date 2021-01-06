package elections.voters;

import elections.Candidate;
import elections.voters.Voter;

import java.util.ArrayList;
import java.util.Map;


public class OnePartyVoter extends Voter {
    private final String partyToVote;

    public OnePartyVoter(String name, String surname,
                         int electoralDistrictNumber, String partyToVote) {
        super(name, surname, electoralDistrictNumber);
        this.partyToVote = partyToVote;
    }


    /**
     * Choose random candidate from voter's party to vote.
     *
     * @param candidatesByParty - candidates grouped by parties.
     * @return Random candidate from voter's party to vote.
     */
    @Override
    protected Candidate chooseCandidate(
            Map<String, ArrayList<Candidate>> candidatesByParty
    ) {
        return this.getRandomCandidate(
                candidatesByParty.get(this.partyToVote)
        );
    }
}
