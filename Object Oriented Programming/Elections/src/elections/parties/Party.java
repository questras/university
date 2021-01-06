package elections.parties;

import elections.ElectoralDistrict;
import elections.Action;
import elections.ActionChoice;
import elections.voters.Voter;

import java.util.ArrayList;


public abstract class Party {

    protected String name;
    private int budget;

    public Party(String name, int budget) {
        this.name = name;
        this.budget = budget;
    }

    /**
     * Get best ActionChoice choosing from actions and districts
     * based on party's strategy.
     *
     * @param actions   - actions to choose from,
     * @param districts - districts to choose from.
     * @return Best ActionChoice based on party's strategy.
     */
    protected abstract ActionChoice getBestActionChoice(
            ArrayList<Action> actions,
            ArrayList<ElectoralDistrict> districts
    );


    /**
     * Check if party can afford given ActionChoice.
     *
     * @param choice - ActionChoice to check.
     * @return True if party can afford given ActionChoice, False otherwise.
     */
    protected boolean canAfford(ActionChoice choice) {
        return this.budget >= choice.getCost();
    }


    /**
     * Get all ActionChoices that party can afford.
     *
     * @param actions   - actions to choose from,
     * @param districts - districts to choose from.
     * @return All ActionChoices that party can afford.
     */
    protected ArrayList<ActionChoice> getAffordableActionChoices(
            ArrayList<Action> actions,
            ArrayList<ElectoralDistrict> districts
    ) {

        ArrayList<ActionChoice> affordableActionChoices = new ArrayList<>();

        for (ElectoralDistrict district : districts) {
            for (Action action : actions) {
                ActionChoice choice = new ActionChoice(district, action);
                if (this.canAfford(choice)) {
                    affordableActionChoices.add(choice);
                }
            }
        }

        return affordableActionChoices;
    }


    /**
     * Influence voters in given district with given action.
     *
     * @param action   - action to influence with,
     * @param district - district with voters to influence.
     */
    protected void influenceElectoralDistrict(Action action,
                                              ElectoralDistrict district) {

        for (Voter voter : district.getAllVoters()) {
            voter.getInfluenced(action);
        }
    }

    /**
     * Run campaign i.e choose best affordable ActionChoice based on party's
     * strategy and influence chosen district with chosen action or do nothing
     * if no ActionChoice is affordable.
     *
     * @param actions   - actions to choose from,
     * @param districts - districts to choose from.
     * @return True if campaign was successfully run and district was
     * influenced, False if there was no affordable ActionChoice.
     */
    public boolean runCampaign(ArrayList<Action> actions,
                               ArrayList<ElectoralDistrict> districts) {

        ActionChoice bestActionChoice =
                this.getBestActionChoice(actions, districts);

        // No affordable ActionChoice available.
        if (bestActionChoice == null) {
            return false;
        }

        this.influenceElectoralDistrict(
                bestActionChoice.getAction(),
                bestActionChoice.getDistrict()
        );
        this.budget -= bestActionChoice.getCost();

        return true;
    }

    public String getName() {
        return name;
    }
}
