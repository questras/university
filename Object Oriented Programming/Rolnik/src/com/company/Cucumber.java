package com.company;

public class Cucumber extends Vegetable {
    public Cucumber() {
        super("Cucumber", 2);
    }

    @Override
    protected void grow() throws InterruptedException {
        // Cucumber is growing.
        Thread.sleep(3000);

        // Cucumber is fresh for 5 seconds.
        this.value = 7;
        Thread.sleep(5000);

        // Cucumber is almost fresh for 5 seconds.
        this.value = 5;
        Thread.sleep(5000);

        // Cucumber is not fresh.
        this.value = 0;
    }
}