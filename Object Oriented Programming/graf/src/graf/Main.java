package graf;

import java.util.*;

public class Main {

    public static void main(String[] args) {
        Graph<String> graph = new Graph<>();
        graph.addVertice("V1");
        graph.addVertice("V2");
        graph.addVertice("V3");
        graph.addVertice("V4");
        graph.addVertice("V5");
        graph.addVertice("V6");
        graph.addVertice("V7");
        graph.addVertice("V8");

        graph.addEdge("V1", "V2");
        graph.addEdge("V1", "V3");
        graph.addEdge("V1", "V4");
        graph.addEdge("V3", "V5");
        graph.addEdge("V4", "V6");
        graph.addEdge("V4", "V7");
        graph.addEdge("V5", "V8");

//                    V1
//                 /   |   \
//                V2   V3    V4
//                    /      / \
//                   V5     V6  V7
//                  /
//                 V8

        System.out.println("DFS");
        for (Iterator<String> it = graph.DFSiter("V1"); it.hasNext(); ) {
            String v = it.next();
            System.out.println(v);
        }

        System.out.println("-----------------------");

        System.out.println("BFS");
        for (Iterator<String> it = graph.BFSiter("V1"); it.hasNext(); ) {
            String v = it.next();
            System.out.println(v);
        }

    }
}
