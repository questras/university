void bubbleSort(List<int> array, int n) {
  for (int i = 1; i < n; i++) {
    for (int k = n-1; k >= i; k--) {
      if (array[k] < array[k-1]) {
        var temp = array[k];

        array[k] = array[k-1];
        array[k-1] = temp;
      }
    }
  }
}

void main(List<String> args) {
  int n = 50000;
  final array = new List<int>(n);

  for (int i = 0; i < n; i++) {
    array[i] = n-i;
  }
  bubbleSort(array, n);
}