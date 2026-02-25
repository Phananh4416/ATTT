#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

// Hàm tìm vị trí của ký tự trong bảng 5x5
void findPosition(char matrix[SIZE][SIZE], char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Hàm tạo bảng 5x5 từ từ khóa
void generateMatrix(char *key, char matrix[SIZE][SIZE]) {
    int dict[26] = {0};
    int x = 0, y = 0;
    
    // Đánh dấu ký tự trong key
    for (int i = 0; i < strlen(key); i++) {
        char c = toupper(key[i]);
        if (c == 'J') c = 'I';
        if (c >= 'A' && c <= 'Z' && !dict[c - 'A']) {
            matrix[x][y++] = c;
            dict[c - 'A'] = 1;
            if (y == SIZE) { x++; y = 0; }
        }
    }
    
    // Điền các chữ cái còn lại
    for (int i = 0; i < 26; i++) {
        if (i == ('J' - 'A')) continue; // Bỏ qua J
        if (!dict[i]) {
            matrix[x][y++] = (char)(i + 'A');
            dict[i] = 1;
            if (y == SIZE) { x++; y = 0; }
        }
    }
}

// Hàm mã hóa
void encrypt(char matrix[SIZE][SIZE], char *text) {
    int len = strlen(text);
    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        findPosition(matrix, text[i], &r1, &c1);
        findPosition(matrix, text[i+1], &r2, &c2);

        if (r1 == r2) { // Cùng hàng
            text[i] = matrix[r1][(c1 + 1) % SIZE];
            text[i+1] = matrix[r2][(c2 + 1) % SIZE];
        } else if (c1 == c2) { // Cùng cột
            text[i] = matrix[(r1 + 1) % SIZE][c1];
            text[i+1] = matrix[(r2 + 1) % SIZE][c2];
        } else { // Hình chữ nhật
            text[i] = matrix[r1][c2];
            text[i+1] = matrix[r2][c1];
        }
    }
}
// Hàm giải mã (Decrypt)
void decrypt(char matrix[SIZE][SIZE], char *text) {
    int len = strlen(text);
    for (int i = 0; i < len; i += 2) {
        int r1, c1, r2, c2;
        findPosition(matrix, text[i], &r1, &c1);
        findPosition(matrix, text[i+1], &r2, &c2);

        if (r1 == r2) { // Cùng hàng: lùi sang trái
            // Cộng thêm SIZE trước khi modulo để tránh số âm trong C
            text[i] = matrix[r1][(c1 - 1 + SIZE) % SIZE];
            text[i+1] = matrix[r2][(c2 - 1 + SIZE) % SIZE];
        } else if (c1 == c2) { // Cùng cột: lùi lên trên
            text[i] = matrix[(r1 - 1 + SIZE) % SIZE][c1];
            text[i+1] = matrix[(r2 - 1 + SIZE) % SIZE][c2];
        } else { // Hình chữ nhật: quy tắc giữ nguyên
            text[i] = matrix[r1][c2];
            text[i+1] = matrix[r2][c1];
        }
    }
}
int main() {
    char key[] = "MONARCHY"; // Ví dụ từ khóa
    char matrix[SIZE][SIZE];
    char input[] = "DO YOU LIKE TO STUDY A CRYPTOGRAPHY COURSE";
    char prepared[200] = "";
    int j = 0;

    generateMatrix(key, matrix);

    // Tiền xử lý: Viết hoa, bỏ dấu cách, xử lý cặp trùng và độ dài lẻ
    for (int i = 0; i < strlen(input); i++) {
        if (isalpha(input[i])) {
            char c = toupper(input[i]);
            prepared[j++] = (c == 'J') ? 'I' : c;
            
            // Nếu chữ tiếp theo giống hệt (đang ở vị trí lẻ của cặp)
            // Ví dụ: "LL" -> "LX L"
        }
    }
    
    // Xử lý chèn X nếu cần và đảm bảo độ dài chẵn
    char finalInput[200] = "";
    int k = 0;
    for(int i=0; i<j; i++) {
        finalInput[k++] = prepared[i];
        if(i+1 < j && prepared[i] == prepared[i+1] && k % 2 != 0) {
            finalInput[k++] = 'X';
        }
    }
    if (strlen(finalInput) % 2 != 0) finalInput[strlen(finalInput)] = 'X';

    printf("Key: %s\n", key);
    printf("Plaintext: %s\n", finalInput);
    
    encrypt(matrix, finalInput);
    printf("Ciphertext: %s\n", finalInput);

    // Gọi hàm giải mã
    decrypt(matrix, finalInput);
    printf("Decrypted text: %s\n", finalInput);
    
    return 0;
}