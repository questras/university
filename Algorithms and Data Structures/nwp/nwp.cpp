// Michał Wiśniewski 418486
// Najdłuższy wspólny podciąg prawie rosnący
// Złożoność czasowa: O(n * m * w)
// Złożoność pamięciowa: O(n * m * w)
// gdzie 'w' to najdłuższy możliwy wynik (w przypadku tego zadania w=20).

#include <iostream>
#include <climits>
using namespace std;

const int MAX_RESULT_LEN = 20;
const int MAX_N = 1000;
const int MAX_M = 1000;

int n, m, c;
int A[MAX_N + 42];
int B[MAX_M + 42];

// Tablica zliczająca, którą czyta się w następujący sposób:
// counting[i][j][k] znaczy:
// Jeśli ciąg A[1...i] i ciąg B[1...j] mają wspólny podciąg prawie
// rosnący o długości k, to wartością tablicy jest największa liczba, 
// jaka w tym podciągu występuje, w przeciwnym wypadku ustawione jest INT_MAX.
int counting[MAX_N + 42][MAX_M + 42][MAX_RESULT_LEN + 42];

int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n >> m >> c;
    int result_len = min(MAX_RESULT_LEN, min(n, m));

    for (int i = 1; i < n + 1; i++)
    {
        cin >> A[i];
    }
    for (int i = 1; i < m + 1; i++)
    {
        cin >> B[i];
    }

    // Ustawienie domyślnych wartości na INT_MAX.
    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < m + 1; j++)
        {
            for (int w = 0; w < result_len + 1; w++)
            {
                counting[i][j][w] = INT_MAX;
            }
        }
    }

    // Iterujemy się po wszystkich końcach ciągów A i B.
    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 1; j < m + 1; j++)
        {
            if (A[i] == B[j])
            {
                // Sytuacja, że końcówki ciągów są równe.

                // Dla k=1 ustawiamy wartość dla najmniejszej liczby spośród aktualnej A[i],
                // i wartości dla policzonych wcześniej ciągów A[1...i], B[1...j-1] oraz 
                // A[1...i-1], B[1...j]. Najmniejsza, bo chcemy aby największa liczba w danym
                // przedziale dla tego podciągu była jak najmniejsza.
                counting[i][j][1] = min(A[i], min(counting[i - 1][j][1], counting[i][j - 1][1]));

                int currentMaxNumber;
                for (int k = 2; k < result_len + 1; k++)
                {
                    // To będzie wynik dla aktualnego i,j,k.
                    currentMaxNumber = INT_MAX;

                    // Sprawdzamy czy dla ciągów A[1...i], B[1...j] i długości k jest
                    // podciąg w ciągu A[1...i-1], B[1...j-1] o długości k-1, taki, który
                    // możemy dołączyć, czyli którego największa liczba w podciągu jest 
                    // "prawie mniejsza" od aktualnego elementu.
                    if (counting[i - 1][j - 1][k - 1] <= A[i] + c)
                    {
                        // Jeśli możemy utworzyć taki podciąg, który ma maksymalną długość określoną
                        // w treści zadania, to kończymy wykonywanie programu.
                        if (k == result_len)
                        {
                            cout << result_len << endl;
                            return 0;
                        }

                        // "Łączymy" aktualną liczbę z podciągiem długości k-1 dla ciągów
                        // A[1...i-1], B[1...j-1] i ustawiamy aktualną największą liczbę dla
                        // danego podciągu.
                        currentMaxNumber = max(counting[i - 1][j - 1][k - 1], A[i]);
                    }

                    // Ustawiamy wartości dla danych końcówek w zależności od aktualnej największej liczby i
                    // poprzednich, policzonych już wyników. Bierzemy ten, który ma najmniejszą największą liczbę
                    // w podciągu.
                    counting[i][j][k] = min(currentMaxNumber, min(counting[i - 1][j][k], counting[i][j - 1][k]));
                }
            }
            else
            {
                // Sytuacja, że końcówki ciągów nie są równe, więc ustawiamy wartości dla
                // danych końcówek w zależności od poprzednich wyników (ciągów A[1...i] i B[1...j-1] oraz
                // A[1...i-1] i B[1...j]), ponieważ dla nich optymalna wartość jest już policzona.
                for (int k = 1; k < result_len + 1; k++)
                {
                    counting[i][j][k] = min(counting[i - 1][j][k], counting[i][j - 1][k]);
                }
            }
        }
    }

    // Szukamy największego k, dla którego mamy wspólny prawie rosnący podciąg.
    int result = 0;
    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 1; j < m + 1; j++)
        {
            for (int k = result_len - 1; k >= 1; k--)
            {
                if (counting[i][j][k] != INT_MAX)
                {
                    result = max(result, k);
                }
            }
        }
    }

    cout << result << endl;
}
