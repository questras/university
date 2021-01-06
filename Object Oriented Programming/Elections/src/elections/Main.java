package elections;

import java.io.FileNotFoundException;


public class Main {

    public static void main(String[] args) throws FileNotFoundException {
        String filename = args[0];

        InputReader reader = new InputReader(filename);
        Elections elections = reader.getAndParseData();

        elections.startCampaign();
        elections.runElections();
    }
}
