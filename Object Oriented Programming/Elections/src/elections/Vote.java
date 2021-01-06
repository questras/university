package elections;


public class Vote {
    private final int electoralDistrictNumber;
    private final String partyName;
    private final int numberOnList;
    private final String voterName;
    private final String voterSurname;

    public Vote(int electoralDistrictNumber, String partyName,
                int numberOnList, String voterName, String voterSurname) {
        this.electoralDistrictNumber = electoralDistrictNumber;
        this.partyName = partyName;
        this.numberOnList = numberOnList;
        this.voterName = voterName;
        this.voterSurname = voterSurname;
    }

    public int getElectoralDistrictNumber() {
        return electoralDistrictNumber;
    }

    public int getNumberOnList() {
        return numberOnList;
    }

    public String getPartyName() {
        return partyName;
    }

    public String getVoterName() {
        return voterName;
    }

    public String getVoterSurname() {
        return voterSurname;
    }
}
