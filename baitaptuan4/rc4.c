#include <stdio.h>
#include <string.h>

int S[10];
int K[4] = {2, 4, 1, 7};
int N = 10;
int key_len = 4;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void ksa() {
    for (int i = 0; i < N; i++) {
        S[i] = i;
    }
    
    int j = 0;
    for (int i = 0; i < N; i++) {
        j = (j + S[i] + K[i % key_len]) % N;
        swap(&S[i], &S[j]);
    }
    
    printf("Mang S sau buoc KSA: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", S[i]);
    }
    printf("\n\n");
}

void encrypt(const char *plaintext) {
    int i = 0, j = 0;

    printf("%-5s%-4s%-4s%-6s%-6s%-12s%-10s%-10s%s\n", 
           "m(t)", "i", "j", "S[i]", "S[j]", "t=(Si+Sj)%N", "k=S[t]", "C(Dec)", "C(ASCII)");
    
    for(int idx = 0; idx < 75; idx++) printf("-");
    printf("\n");

    int len = strlen(plaintext);
    for (int idx = 0; idx < len; idx++) {
        char m = plaintext[idx];
        
        i = (i + 1) % N;
        j = (j + S[i]) % N;
        swap(&S[i], &S[j]);
        
        int t = (S[i] + S[j]) % N;
        int k = S[t]; 

        unsigned char m_val = (unsigned char)m;
        unsigned char c_val = m_val ^ k;

        printf("%-5c%-4d%-4d%-6d%-6d%-12d%-10d%-10d", 
               m, i, j, S[i], S[j], t, k, (int)c_val);
        
        if (c_val >= 32 && c_val <= 126) {
            printf("%c\n", (char)c_val);
        } else {
            printf("[non-print]\n");
        }
    }
}

int main() {
    ksa();
    encrypt("cybersecurity");
    return 0;
}