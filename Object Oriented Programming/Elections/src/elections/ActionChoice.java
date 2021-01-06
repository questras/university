package elections;


public class ActionChoice {
    private final ElectoralDistrict district;
    private final Action action;

    public ActionChoice(ElectoralDistrict district, Action action) {
        this.district = district;
        this.action = action;
    }

    public ElectoralDistrict getDistrict() {
        return this.district;
    }

    public Action getAction() {
        return this.action;
    }


    /**
     * Get cost of ActionChoice i.e cost of action multiplied by
     * number of voters in district.
     *
     * @return Cost of ActionChoice.
     */
    public int getCost() {
        return this.district.getNumberOfAllVoters() * this.action.getCost();
    }
}
