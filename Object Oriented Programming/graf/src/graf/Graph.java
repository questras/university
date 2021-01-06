package graf;

import java.util.*;


public class Graph<T> {
    Map<T, Set<T>> verticesAndNeighbours;

    public Graph() {
        this.verticesAndNeighbours = new HashMap<>();
    }

    public boolean hasVertice(T element) {
        return this.verticesAndNeighbours.containsKey(element);
    }

    public void addVertice(T element) {
        if (!this.hasVertice(element)) {
            this.verticesAndNeighbours.put(
                    element,
                    new HashSet<>()
            );
        }
    }

    public void addEdge(T element1, T element2) {
        if (this.hasVertice(element1) && this.hasVertice(element2)) {

            this.verticesAndNeighbours.get(element1).add(element2);
            this.verticesAndNeighbours.get(element2).add(element1);
        }
    }

    private Iterator<T> iter(T firstVertice, SearchStructure<T> structure) {
        if (this.hasVertice(firstVertice)) {
            ArrayList<T> saved = new ArrayList<>();
            Set<T> seen = new HashSet<>();

            structure.push(firstVertice);
            while (!structure.isEmpty()) {
                T current = structure.pop();
                saved.add(current);
                seen.add(current);

                for (T v : this.verticesAndNeighbours.get(current)) {
                    if (!seen.contains(v)) {
                        structure.push(v);
                    }
                }
            }

            return saved.iterator();
        }

        return null;
    }

    public Iterator<T> DFSiter(T firstVertice) {
        return this.iter(firstVertice, new SearchStack<>());
    }

    public Iterator<T> BFSiter(T firstVertice) {
        return this.iter(firstVertice, new SearchQueue<>());
    }
}
