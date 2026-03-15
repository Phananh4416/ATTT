#include <stdio.h>
#include <stdint.h>

// Hàm lấy bậc của đa thức (vị trí bit 1 cao nhất)
int get_degree(uint32_t poly) {
    if (poly == 0) return -1;
    int deg = 0;
    while (poly > 1) {
        poly >>= 1;
        deg++;
    }
    return deg;
}

// Hàm chia đa thức trong GF(2)
// Trả về thương q và số dư r
void poly_div(uint32_t num, uint32_t den, uint32_t *q, uint32_t *r) {
    *q = 0;
    *r = num;
    int deg_den = get_degree(den);
    
    if (deg_den < 0) return; // Không thể chia cho 0

    // Trong khi bậc của số dư còn lớn hơn hoặc bằng bậc của số chia
    while (get_degree(*r) >= deg_den) {
        int shift = get_degree(*r) - deg_den;
        *q ^= (1 << shift);          // Cộng (XOR) vào thương
        *r ^= (den << shift);        // Trừ (XOR) đi phần tương ứng từ số dư
    }
}

// Hàm nhân đa thức thông thường trong GF(2) (không modulo)
uint32_t poly_mul(uint32_t a, uint32_t b) {
    uint32_t res = 0;
    while (b > 0) {
        if (b & 1) res ^= a; // Nếu bit cuối của b là 1, cộng a vào kết quả
        a <<= 1;             // Dịch a lên 1 bậc (nhân x)
        b >>= 1;             // Dịch b xuống để xét bit tiếp theo
    }
    return res;
}

// Hàm Euclidean mở rộng tìm nghịch đảo nhân
uint32_t find_inverse(uint32_t a) {
    // Đa thức tối giản m(x) = x^10 + x^3 + 1
    // Biểu diễn nhị phân: 10000001001 -> Hệ thập phân: 1033
    uint32_t m = 1033;
    
    // Khởi tạo các giá trị
    uint32_t r1 = m, r2 = a;
    uint32_t t1 = 0, t2 = 1;
    uint32_t q, r, t;
    int step = 1;

    printf("\n=== Tim nghich dao nhan cho a = %u ===\n", a);
    printf("%-5s | %-5s | %-5s | %-5s | %-5s | %-5s | %-5s | %-5s\n", 
           "Step", "r1", "r2", "q", "r", "t1", "t2", "t");
    printf("------------------------------------------------------------------------\n");

    while (r2 != 0) {
        // 1. Chia đa thức để lấy q và r
        poly_div(r1, r2, &q, &r);
        
        // 2. Tính t = t1 - (q * t2). Trừ trong GF(2) là XOR
        t = t1 ^ poly_mul(q, t2);

        // In ra các giá trị trung gian
        printf("%-5d | %-5u | %-5u | %-5u | %-5u | %-5u | %-5u | %-5u\n", 
               step, r1, r2, q, r, t1, t2, t);

        // 3. Cập nhật các biến cho bước lặp tiếp theo
        r1 = r2;
        r2 = r;
        t1 = t2;
        t2 = t;
        step++;
    }

    if (r1 == 1) {
        printf("------------------------------------------------------------------------\n");
        printf("=> Nghich dao cua %u la: %u\n", a, t1);
        return t1;
    } else {
        printf("------------------------------------------------------------------------\n");
        printf("=> Khong ton tai nghich dao (gcd != 1)\n");
        return 0;
    }
}

int main() {
    uint32_t a = 523;
    uint32_t b = 1015;

    // Test vector 1
    find_inverse(a);

    // Test vector 2
    find_inverse(b);

    return 0;
}