public class BubbleSort {
    
	static void bubbleSort(int[] array, int n) {
		for (int i = 1; i < n; i++) {
            for (int k = n-1; k >= i; k--) {
                if (array[k] < array[k-1]) {
                    int temp = array[k];

                    array[k] = array[k-1];
                    array[k-1] = temp;
                }
            }
        }
	}

    public static void main(String[] args) {
        int n = 50000;
        int[] array = new int[n];

        for (int i = 0; i < n; i++) {
            array[i] = n-i;
        }

        bubbleSort(array, n);
    }

}