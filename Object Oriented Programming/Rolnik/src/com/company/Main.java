package com.company;

public class Main {

    public static void printSimulationInfo(int number, String strategy,
                                           int gardenSize, int time) {
        System.out.println("**********************");
        System.out.println("Simulation " + number);
        System.out.println("Strategy: " + strategy);
        System.out.println("Garden size: " + gardenSize);
        System.out.println("Season time: " + time);
        System.out.println("**********************");
    }

    public static void main(String[] args) throws InterruptedException {
        printSimulationInfo(1, "PracownikPGR", 3, 12);
        Garden garden1 = new Garden(3);
        PracownikPGR farmer1 = new PracownikPGR();
        farmer1.simulate(garden1, 12);

        printSimulationInfo(2, "Gospodarz", 3, 12);
        Garden garden2 = new Garden(3);
        Gospodarz farmer2 = new Gospodarz();
        farmer2.simulate(garden2, 12);


        printSimulationInfo(3, "PracownikPGR", 8, 30);
        Garden garden3 = new Garden(8);
        PracownikPGR farmer3 = new PracownikPGR();
        farmer3.simulate(garden3, 30);


        printSimulationInfo(4, "Gospodarz", 8, 30);
        Garden garden4 = new Garden(8);
        Gospodarz farmer4 = new Gospodarz();
        farmer4.simulate(garden4, 30);
    }
}
