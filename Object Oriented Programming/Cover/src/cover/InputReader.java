package cover;

import java.util.ArrayList;
import java.util.Scanner;


public class InputReader {
    /**
     * Read integers from standard input.
     *
     * @return List of values read from standard input.
     */
    public static ArrayList<Integer> readData() {
        Scanner scanner = new Scanner(System.in);

        ArrayList<Integer> data = new ArrayList<>();
        while (scanner.hasNextInt()) {
            data.add(scanner.nextInt());
        }

        return data;
    }
}
