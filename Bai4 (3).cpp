#include <stdio.h>
#include <string.h>

#define MAX_ZONES 50
#define MAX_NAME_LEN 50
#define MAX_PER_TRANSACTION 4

struct TicketZone {
    int zone_id;
    char zone_name[MAX_NAME_LEN];
    double base_price;
    int total_capacity;
    int sold_count;
    int early_bird;
};

int find_zone_index(const struct TicketZone zones[], int count, int target_id);
void display_zones(const struct TicketZone zones[], int count);
void add_zone(struct TicketZone zones[], int *count);
void update_zone_price(struct TicketZone zones[], int count);
void delete_zone(struct TicketZone zones[], int *count);
void process_booking(struct TicketZone zones[], int count);
void show_menu(void);
void clear_input_buffer(void);

int main(void) {
    struct TicketZone zones[MAX_ZONES];
    int zone_count = 0;

    zones[0] = (struct TicketZone){101, "Zone SVIP", 2500000.0, 100, 15, 1};
    zones[1] = (struct TicketZone){102, "Zone VIP", 1800000.0, 250, 40, 1};
    zones[2] = (struct TicketZone){103, "Zone GA", 1000000.0, 500, 120, 0};
    zones[3] = (struct TicketZone){104, "Zone Standard", 600000.0, 800, 300, 0};
    zone_count = 4;

    int choice;
    do {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            printf("\n[Loi] Vui long nhap mot so hop le!\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1:
                display_zones(zones, zone_count);
                break;
            case 2:
                add_zone(zones, &zone_count);
                break;
            case 3:
                update_zone_price(zones, zone_count);
                break;
            case 4:
                delete_zone(zones, &zone_count);
                break;
            case 5:
                process_booking(zones, zone_count);
                break;
            case 0:
                printf("\n--- Cam on ban da su dung he thong TicketBox. Hen gap lai! ---\n");
                break;
            default:
                printf("\n[Loi] Lua chon khong hop le. Vui long chon tu 0 den 5!\n");
        }
    } while (choice != 0);

    return 0;
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int find_zone_index(const struct TicketZone zones[], int count, int target_id) {
    for (int i = 0; i < count; i++) {
        if (zones[i].zone_id == target_id) {
            return i;
        }
    }
    return -1;
}

void show_menu(void) {
    printf("\n==================================================\n");
    printf("     TICKETBOX POS - VPOP STAR ARENA MANAGEMENT   \n");
    printf("==================================================\n");
    printf("1. Xem danh sach cac khu vuc ve\n");
    printf("2. Them khu vuc ve moi\n");
    printf("3. Cap nhat gia ve goc\n");
    printf("4. Xoa khu vuc ve (Chua ban ve)\n");
    printf("5. Xu ly giao dich dat ve\n");
    printf("0. Thoat chuong trinh\n");
    printf("==================================================\n");
    printf("Nhap lua chon cua ban: ");
}

void display_zones(const struct TicketZone zones[], int count) {
    printf("\n=========================================================================================\n");
    printf("                               DANH SACH KHU VUC VE - VPOP STAR ARENA                    \n");
    printf("=========================================================================================\n");
    printf("%-8s | %-16s | %-15s | %-10s | %-10s | %-12s\n", 
           "Ma ID", "Ten Zone", "Gia Goc (VND)", "Phat Hanh", "Da Ban", "Early Bird");
    printf("-----------------------------------------------------------------------------------------\n");
    
    if (count == 0) {
        printf("Chua co khu vuc ve nao trong he thong.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("%-8d | %-16s | %-15.0f | %-10d | %-10d | %-12s\n",
                   zones[i].zone_id,
                   zones[i].zone_name,
                   zones[i].base_price,
                   zones[i].total_capacity,
                   zones[i].sold_count,
                   zones[i].early_bird == 1 ? "Co (15%)" : "Khong");
        }
    }
    printf("=========================================================================================\n");
}

void add_zone(struct TicketZone zones[], int *count) {
    if (*count >= MAX_ZONES) {
        printf("\n[Loi] He thong da dat gioi han toi da %d zone ve!\n", MAX_ZONES);
        return;
    }

    struct TicketZone new_zone;
    printf("\n--- THEM KHU VUC VE MOI ---\n");
    
    printf("Nhap ma zone (so nguyen duy nhat): ");
    if (scanf("%d", &new_zone.zone_id) != 1) {
        printf("[Loi] Du lieu khong hop le!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (find_zone_index(zones, *count, new_zone.zone_id) != -1) {
        printf("[Loi] Ma zone %d da ton tai trong he thong!\n", new_zone.zone_id);
        return;
    }

    printf("Nhap ten zone (toi da 49 ky tu): ");
    if (fgets(new_zone.zone_name, sizeof(new_zone.zone_name), stdin) != NULL) {
        size_t len = strlen(new_zone.zone_name);
        if (len > 0 && new_zone.zone_name[len - 1] == '\n') {
            new_zone.zone_name[len - 1] = '\0';
        }
    }

    printf("Nhap gia ve goc (VND): ");
    if (scanf("%lf", &new_zone.base_price) != 1 || new_zone.base_price < 0) {
        printf("[Loi] Gia ve khong hop le!\n");
        clear_input_buffer();
        return;
    }

    printf("Nhap tong so luong ve phat hanh: ");
    if (scanf("%d", &new_zone.total_capacity) != 1 || new_zone.total_capacity <= 0) {
        printf("[Loi] So luong ve khong hop le!\n");
        clear_input_buffer();
        return;
    }

    new_zone.sold_count = 0;

    printf("Ap dung Early Bird (1: Co, 0: Khong): ");
    if (scanf("%d", &new_zone.early_bird) != 1 || (new_zone.early_bird != 0 && new_zone.early_bird != 1)) {
        printf("[Loi] Trang thai Early Bird khong hop le! Mac dinh dat la 0.\n");
        new_zone.early_bird = 0;
        clear_input_buffer();
    }
    clear_input_buffer();

    zones[*count] = new_zone;
    (*count)++;
    printf("\n[Thanh cong] Da them khu vuc ve moi thanh cong!\n");
}

void update_zone_price(struct TicketZone zones[], int count) {
    if (count == 0) {
        printf("\n[Thong bao] Chua co zone nao de cap nhat.\n");
        return;
    }

    int target_id;
    printf("\n--- CAP NHAT GIA VE GOC ---\n");
    printf("Nhap ma zone can cap nhat gia: ");
    if (scanf("%d", &target_id) != 1) {
        printf("[Loi] Nhap sai dinh dang!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int idx = find_zone_index(zones, count, target_id);
    if (idx == -1) {
        printf("[Loi] Khong tim thay khu vuc ve co ma ID: %d\n", target_id);
        return;
    }

    printf("Zone hien tai: %s | Gia cu: %.0f VND\n", zones[idx].zone_name, zones[idx].base_price);
    printf("Nhap gia ve goc moi (VND): ");
    double new_price;
    if (scanf("%lf", &new_price) != 1 || new_price < 0) {
        printf("[Loi] Gia ve moi khong hop le!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    zones[idx].base_price = new_price;
    printf("\n[Thanh cong] Da cap nhat gia moi cho zone '%s' thanh cong!\n", zones[idx].zone_name);
}

void delete_zone(struct TicketZone zones[], int *count) {
    if (*count == 0) {
        printf("\n[Thong bao] Danh sach zone dang trong.\n");
        return;
    }

    int target_id;
    printf("\n--- XOA KHU VUC VE ---\n");
    printf("Nhap ma zone can xoa: ");
    if (scanf("%d", &target_id) != 1) {
        printf("[Loi] Nhap sai dinh dang!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int idx = find_zone_index(zones, *count, target_id);
    if (idx == -1) {
        printf("[Loi] Khong tim thay khu vuc ve co ma ID: %d\n", target_id);
        return;
    }

    if (zones[idx].sold_count > 0) {
        printf("[Tu choi] Khong the xoa zone '%s' vi da co %d ve duoc ban ra!\n", 
               zones[idx].zone_name, zones[idx].sold_count);
        return;
    }

    for (int i = idx; i < *count - 1; i++) {
        zones[i] = zones[i + 1];
    }
    (*count)--;
    printf("\n[Thanh cong] Da xoa khu vuc ve ma ID %d khoi he thong.\n", target_id);
}

void process_booking(struct TicketZone zones[], int count) {
    if (count == 0) {
        printf("\n[Thong bao] Hien tai chua co khu vuc ve nao mo ban.\n");
        return;
    }

    display_zones(zones, count);

    int target_id, quantity;
    printf("\n--- XU LY GIAO DICH DAT VE ---\n");
    printf("Nhap ma zone ban muon mua: ");
    if (scanf("%d", &target_id) != 1) {
        printf("[Loi] Nhap sai dinh dang!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int idx = find_zone_index(zones, count, target_id);
    if (idx == -1) {
        printf("[Loi] Ma zone khong ton tai!\n");
        return;
    }

    printf("Nhap so luong ve muon mua (Toi da %d ve/giao dich): ", MAX_PER_TRANSACTION);
    if (scanf("%d", &quantity) != 1) {
        printf("[Loi] Nhap sai dinh dang!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (quantity <= 0 || quantity > MAX_PER_TRANSACTION) {
        printf("[Loi Giao Dich] So luong mua khong hop le! Quy tac chi cho phep mua toi da %d ve/giao dich.\n", MAX_PER_TRANSACTION);
        return;
    }

    int remaining_tickets = zones[idx].total_capacity - zones[idx].sold_count;
    if (quantity > remaining_tickets) {
        printf("[Loi Giao Dich] Khong du ve! Khu vuc '%s' chi con lai %d ve trong kho.\n", 
               zones[idx].zone_name, remaining_tickets);
        return;
    }

    double raw_total = zones[idx].base_price * quantity;
    double discount_amount = 0.0;
    double final_total = raw_total;

    if (zones[idx].early_bird == 1) {
        discount_amount = raw_total * 0.15;
        final_total = raw_total - discount_amount;
    }

    zones[idx].sold_count += quantity;

    printf("\n==================================================\n");
    printf("          HOA DON GIAO DICH - TICKETBOX           \n");
    printf("            Su kien: VPOP Star Arena              \n");
    printf("==================================================\n");
    printf(" - Khu vuc chon:       %s (ID: %d)\n", zones[idx].zone_name, zones[idx].zone_id);
    printf(" - Don gia goc:        %.0f VND\n", zones[idx].base_price);
    printf(" - So luong mua:       %d ve\n", quantity);
    printf(" - Tam tinh:           %.0f VND\n", raw_total);
    if (zones[idx].early_bird == 1) {
        printf(" - Uu dai Early Bird: -15%% (-%.0f VND)\n", discount_amount);
    } else {
        printf(" - Uu dai Early Bird: Khong ap dung\n");
    }
    printf("--------------------------------------------------\n");
    printf(" => TONG THANH TOAN:   %.0f VND\n", final_total);
    printf("==================================================\n");
    printf("[Thanh Cong] Giao dich dat ve da duoc hoan tat!\n");
}
