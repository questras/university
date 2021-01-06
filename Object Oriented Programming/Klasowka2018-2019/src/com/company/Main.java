package com.company;

class Karta {
    private int wartosc;
    private int kolor;

    public Karta(int wartosc, int kolor) {
        this.wartosc = wartosc;
        this.kolor = kolor;
    }

    public int podaj_wartosc() {
        return this.wartosc;
    }

    public int podaj_kolor() {
        return this.kolor;
    }

    // Zwroc true jesli podana karta jest młodsza, false w.p.p.
    public boolean porownaj(Karta k) {
        if (this.podaj_wartosc() == k.podaj_wartosc()) {
            return this.podaj_kolor() > k.podaj_kolor();
        }
        else if (this.podaj_wartosc() == 1) {
            return true;
        }
        else if (k.podaj_wartosc() == 1) {
            return false;
        }
        else {
            return this.podaj_wartosc() > k.podaj_wartosc();
        }
    }

    public static Karta zwroc_starsza_karte(Karta k1, Karta k2) {
        if (k1.podaj_wartosc() == k2.podaj_wartosc()) {
            if (k1.podaj_kolor() > k2.podaj_kolor()) {
                return k1;
            }
            else {
                return k2;
            }
        }
        else if (k1.podaj_wartosc() == 1) {
            return k1;
        }
        else if (k2.podaj_wartosc() == 1) {
            return k2;
        }
        else {
            if (k1.podaj_wartosc() > k2.podaj_wartosc()) {
                return k1;
            }
            else {
                return k2;
            }
        }
    }

    private String kolorToString() {
        switch (this.kolor) {
            case 1: return "Trefl";
            case 2: return "Karo";
            case 3: return "Kier";
            case 4: return "Pik";
            default: return "Nieznany kolor";
        }
    }

    private String wartoscToString() {
        switch (this.wartosc) {
            case 1: return "As";
            case 11: return "Walet";
            case 12: return "Dama";
            case 13: return "Król";
            default: return Integer.toString(this.wartosc);
        }
    }

    public String toString() {
        return this.wartoscToString() + " " + this.kolorToString();
    }

}

class Blyskawica {
    private int numer_rozdania;
    private int ustalona_kwota;
    private int indeks_nastepnej_karty;
    private Karta[] karty;
    private Gracz[] gracze;

    public Blyskawica(Gracz[] gracze, int ustalona_kwota) {
        this.indeks_nastepnej_karty = -1;
        this.numer_rozdania = 0;
        this.ustalona_kwota = ustalona_kwota;
        this.gracze = gracze;
        this.karty = new Karta[52];
    }

    void stworz_nowa_talie() {
        for (int i = 1; i <= 13; i++) {
            for (int j = 1; j <= 4; j++) {
                int indeks = (i-1)*4 + (j-1);
                karty[indeks] = new Karta(i, j);
            }
        }

        this.indeks_nastepnej_karty = 0;
    }

    void potasuj_talie() {
        Karta[] nowa_talia = new Karta[52];

    }

    void rozdaj_karte(Gracz g) {
        g.przyjmij_karte(this.karty[this.indeks_nastepnej_karty]);
        this.indeks_nastepnej_karty++;
    }

    void rozdaj_karty() {
        for (int i = 0; i < 5; i++) {
            for (Gracz gracz : this.gracze) {
                this.rozdaj_karte(gracz);
            }
        }
    }

    void raport_gry(Gracz wygrany_gracz) {
        System.out.println("Rozdanie: " + this.numer_rozdania);
        System.out.println("Karty: ");
        for (int i = 0; i < this.gracze.length; i++) {
            System.out.println("Gracz " + i);
            System.out.println(this.gracze[i].kartyToString());
        }
    }

    public void graj() {
        while (true) {
            // Zbierz pieniadze do puli i zresetuj karty.
            for (Gracz gracz : this.gracze) {
                gracz.wplac_do_puli(this.ustalona_kwota);
                if (gracz.czy_rezygnacja()) {
                    return;
                }
                gracz.zresetuj_karty();
            }

            this.numer_rozdania++;
            this.stworz_nowa_talie();
            this.potasuj_talie();
            this.rozdaj_karty();

            Gracz naj_gracz = this.gracze[0];
            Karta naj_karta = new Karta(0, 0);

            for (Gracz gracz : this.gracze) {
                if (!naj_karta.porownaj(gracz.zwroc_najstarsza_karte())) {
                    naj_karta = gracz.zwroc_najstarsza_karte();
                    naj_gracz = gracz;
                }
            }

            int wygrana = this.gracze.length * this.ustalona_kwota;
            naj_gracz.przyjmij_wygrana(wygrana);

            this.raport_gry(naj_gracz);
        }
    }

}

class Gracz {
    private int pieniadze;
    private int indeks_ostatniej_karty;
    private Karta[] karty;

    public Gracz(int pieniadze) {
        this.pieniadze = pieniadze;
        this.indeks_ostatniej_karty = -1;
        this.karty = new Karta[5];
    }

    public void zresetuj_karty() {
        this.indeks_ostatniej_karty = -1;
    }

    public void przyjmij_karte(Karta k) {
        if (this.indeks_ostatniej_karty == 4) {
            return;
        }

        this.indeks_ostatniej_karty++;
        this.karty[this.indeks_ostatniej_karty] = k;
    }


    Karta zwroc_najstarsza_karte() {
        Karta maks_karta = new Karta(0, 0);
        if (this.indeks_ostatniej_karty == -1) {
            return maks_karta;
        }

        for(int i = 0; i <= this.indeks_ostatniej_karty; i++) {
            if (!maks_karta.porownaj(this.karty[i])) {
                maks_karta = this.karty[i];
            }
        }

        return maks_karta;
    }

    public void wplac_do_puli(int stawka) {
        this.pieniadze -= stawka;
    }

    public void przyjmij_wygrana(int suma) {
        this.pieniadze += suma;
    }

    boolean czy_rezygnacja() {
        return this.pieniadze < 0;
    }

    public String kartyToString() {
        String karty = "";
        for (Karta karta : this.karty) {
            karty = karty + karta.toString() + "\n";
        }

        return karty;
    }
}

public class Main {

    public static void main(String[] args) {
	// write your code here
    }
}
