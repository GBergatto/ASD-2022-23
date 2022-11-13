#include <stdio.h>

int gcd(int a, int b);

int main(int argc, char const *argv[]) {
    int div = gcd(12, 22);
    printf("%d\n", div);

    return 0;
}

int gcd(int a, int b) {
    if (b > a) {  // scambio
        int temp = a;
        a = b;
        b = temp;
    }

    // condizione di terminazione
    if (b == 0 || a == b) {
        return a;
    }

    if (b % 2 == 0) {
        if (a % 2 == 0)
            return 2 * gcd(a / 2, b / 2);
        else
            return gcd(a, b / 2);
    } else if (a % 2 == 0) {
        // aggiunta per simmetria
        return gcd(a / 2, b);
    } else {
        return gcd((a - b) / 2, b);
    }
}