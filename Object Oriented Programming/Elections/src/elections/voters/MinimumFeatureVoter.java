package elections.voters;

import elections.voters.FeatureVoter;

import java.util.ArrayList;
import java.util.Collections;


public class MinimumFeatureVoter extends FeatureVoter {

    public MinimumFeatureVoter(String name, String surname,
                               int electoralDistrictNumber,
                               int desiredFeature) {

        super(name, surname, electoralDistrictNumber, desiredFeature);
    }


    /**
     * Choose feature with smallest value from features.
     *
     * @param features - features to choose from.
     * @return Feature with smallest value.
     */
    @Override
    protected int chooseBestFeature(ArrayList<Integer> features) {
        return Collections.min(features);
    }
}
