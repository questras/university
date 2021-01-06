package elections;

import java.util.ArrayList;


public class Candidate {
    private final String name;
    private final String surname;
    private int electoralDistrictNumber;
    private final String partyName;
    private int numberOnList;
    private final ArrayList<Integer> features;


    public Candidate(String name, String surname, int electoralDistrictNumber,
                     String partyName, int numberOnList,
                     ArrayList<Integer> features) {

        this.name = name;
        this.surname = surname;
        this.electoralDistrictNumber = electoralDistrictNumber;
        this.partyName = partyName;
        this.numberOnList = numberOnList;
        this.features = features;
    }

    public String getPartyName() {
        return partyName;
    }

    public int getNumberOnList() {
        return numberOnList;
    }

    public int getElectoralDistrictNumber() {
        return electoralDistrictNumber;
    }

    public ArrayList<Integer> getFeatures() {
        return new ArrayList<>(this.features);
    }

    public String getName() {
        return name;
    }

    public String getSurname() {
        return surname;
    }


    /**
     * Get feature of candidate with given number.
     *
     * @param numberOfFeature - number of feature to get.
     * @return Feature of candidate with given number.
     */
    public int getFeatureWithNumber(int numberOfFeature) {
        return this.features.get(numberOfFeature - 1);
    }


    /**
     * Change ElectoralDistrict of candidate i.e change candidate's
     * number of district and update number on list.
     *
     * @param newDistrict - ElectoralDistrict to change to.
     */
    public void changeElectoralDistrict(
            ElectoralDistrict newDistrict
    ) {
        this.electoralDistrictNumber = newDistrict.getNumber();
        this.numberOnList +=
                newDistrict.getNumberOfCandidatesInParty(this.partyName);
    }
}
