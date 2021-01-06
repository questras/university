package com.company;

class Bilbo {
    public Bilbo() {}

    public void rozdziel_skarby(Krasnolud[] krasnoludy, Skarb[] skarby) {
        int liczba_krasnoludow = krasnoludy.length;
        int indeks_krasnoluda = 0;

        for (Skarb skarb : skarby) {
            krasnoludy[indeks_krasnoluda].wezSkarb(skarb);

            if (indeks_krasnoluda + 1 == liczba_krasnoludow) {
                indeks_krasnoluda = 0;
            }
            else {
                indeks_krasnoluda += 1;
            }
        }
    }
}


class Krasnolud {
    private int rozmiar_tablicy_skarbow;
    private int index_ostatniego_skarbu;
    private Skarb[] skarby;
    private String imie;

    public Krasnolud(String imie) {
        this.imie = imie;
        this.rozmiar_tablicy_skarbow = 100;
        this.skarby = new Skarb[this.rozmiar_tablicy_skarbow];
        this.index_ostatniego_skarbu = -1;
    }

    public void wezSkarb(Skarb s) {
        if(this.index_ostatniego_skarbu < this.rozmiar_tablicy_skarbow-1) {
            this.index_ostatniego_skarbu += 1;
            this.skarby[this.index_ostatniego_skarbu] = s;
        }
    }

    public void wypiszSkarby() {
        System.out.println("Krasnolud " + this.imie +  " posiada: ");
        for (int i = 0; i <= index_ostatniego_skarbu; i++) {
            System.out.println("\t" + this.skarby[i].toString());
        }
    }
}


class Skarb {
    private int wartosc;

    public Skarb(int w) {
        this.wartosc = w;
    }

    public String toString() {
        return "Skarb o wartości " + this.wartosc;
    }
}


public class Main {
    public static Skarb[] stworz_skarby(int liczba_skarbow) {
        Skarb[] skarby = new Skarb[liczba_skarbow];

        for (int i = 0; i < liczba_skarbow; i++) {
            skarby[i] = new Skarb(i*5 + 9);
        }

        return skarby;
    }

    public static Krasnolud[] stworz_krasnoludy(int liczba_krasnoludow) {
        Krasnolud[] krasnoludy = new Krasnolud[liczba_krasnoludow];

        for (int i = 0; i < liczba_krasnoludow; i++) {
            krasnoludy[i] = new Krasnolud("Krasnolud" + i);
        }

        return krasnoludy;
    }

    public static void main(String[] args) {
        Bilbo bilbo = new Bilbo();

        Krasnolud[] krasnoludy = stworz_krasnoludy(7);
        Skarb[] skarby = stworz_skarby(10);

        bilbo.rozdziel_skarby(krasnoludy, skarby);

        for (Krasnolud krasnolud: krasnoludy) {
            krasnolud.wypiszSkarby();
        }

    }
}
