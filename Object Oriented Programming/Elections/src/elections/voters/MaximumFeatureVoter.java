package elections.voters;

import elections.voters.FeatureVoter;

import java.util.ArrayList;
import java.util.Collections;


public class MaximumFeatureVoter extends FeatureVoter {

    public MaximumFeatureVoter(String name, String surname,
                               int electoralDistrictNumber,
                               int desiredFeature) {

        super(name, surname, electoralDistrictNumber, desiredFeature);
    }


    /**
     * Choose feature with greatest value from features.
     *
     * @param features - features to choose from.
     * @return Feature with greatest value.
     */
    @Override
    protected int chooseBestFeature(ArrayList<Integer> features) {
        return Collections.max(features);
    }
}
