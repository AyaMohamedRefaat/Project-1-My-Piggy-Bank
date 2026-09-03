#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define COIN_KINDS   5U
#define TOY_COUNT    4U
#define NAME_LEN    16U

static const uint16_t COIN_VALUE[COIN_KINDS] = { 25U, 50U, 100U, 200U, 500U };
static uint16_t coinCount[COIN_KINDS];

typedef struct {
    char     name[NAME_LEN];
    uint32_t price; /* in piastres */
} Toy_t;

static Toy_t shop[TOY_COUNT];

/* Function Prototypes */
static void     seedBank(void);
static void     addCoins(void);
static void     takeCoins(void);
static uint32_t bankTotal(void);
static uint32_t sumCoins(const uint16_t *counts, uint8_t n);
static uint8_t  biggestPile(void);
static void     showBank(void);
static void     drawBar(uint16_t value, uint16_t full, uint8_t width);
static void     buyToy(void);
static void     bankReport(void);

int main(void) {
    seedBank();
    int choice = 0;
    int c = 0;

    do {
        printf("\n====================================\n");
        printf("         MY PIGGY BANK MENU         \n");
        printf("====================================\n");
        printf("1. Add Coins\n");
        printf("2. Take Coins\n");
        printf("3. View Bank \n");
        printf("4. Buy Toy Check\n");
        printf("5. Bank Full Summary Report\n");
        printf("6. Reset / Seed Bank\n");
        printf("0. Exit\n");
        printf("------------------------------------\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\n[!] Invalid input! Please enter a number.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        switch (choice) {
            case 1: addCoins(); break;
            case 2: takeCoins(); break;
            case 3: showBank(); break;
            case 4: buyToy(); break;
            case 5: bankReport(); break;
            case 6: seedBank(); printf("\n[*] Bank re-seeded successfully.\n"); break;
            case 0: printf("\nGoodbye!\n"); break;
            default: printf("\n[!] Option does not exist.\n"); break;
        }
    } while (choice != 0);

    return 0;
}

static void seedBank(void) {
    coinCount[0] = 4U;  /* 25 piastres x 4 */
    coinCount[1] = 6U;  /* 50 piastres x 6 */
    coinCount[2] = 10U; /* 100 piastres x 10 */
    coinCount[3] = 3U;  /* 200 piastres x 3 */
    coinCount[4] = 2U;  /* 500 piastres x 2 */

    strncpy(shop[0].name, "Yo-Yo", NAME_LEN - 1U);
    shop[0].name[NAME_LEN - 1U] = '\0';
    shop[0].price = 300U;

    strncpy(shop[1].name, "Puzzle Box", NAME_LEN - 1U);
    shop[1].name[NAME_LEN - 1U] = '\0';
    shop[1].price = 750U;

    strncpy(shop[2].name, "Action Figure", NAME_LEN - 1U);
    shop[2].name[NAME_LEN - 1U] = '\0';
    shop[2].price = 1500U;

    strncpy(shop[3].name, "RC Car", NAME_LEN - 1U);
    shop[3].name[NAME_LEN - 1U] = '\0';
    shop[3].price = 3500U;
}

static void addCoins(void) {
    int coinIdx = 0;
    int amount = 0;
    int c = 0;

    printf("\nAvailable Coins:\n");
    for (uint8_t i = 0U; i < COIN_KINDS; ++i) {
        printf("  [%u] %u piastres\n", i, COIN_VALUE[i]);
    }

    printf("Select coin index (0-4): ");
    if (scanf("%d", &coinIdx) != 1 || coinIdx < 0 || coinIdx >= (int)COIN_KINDS) {
        printf("\n[!] Invalid coin selection! Nothing added.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    printf("Enter quantity to add: ");
    if (scanf("%d", &amount) != 1 || amount <= 0) {
        printf("\n[!] Invalid amount! Nothing added.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    coinCount[coinIdx] += (uint16_t)amount;
    printf("\n[+] Added %d coin(s) of %u piastres.\n", amount, COIN_VALUE[coinIdx]);
}

static void takeCoins(void) {
    int coinIdx = 0;
    int amount = 0;
    int c = 0;

    printf("\nAvailable Coins:\n");
    for (uint8_t i = 0U; i < COIN_KINDS; ++i) {
        printf("  [%u] %u piastres (Available: %u)\n", i, COIN_VALUE[i], coinCount[i]);
    }

    printf("Select coin index (0-4): ");
    if (scanf("%d", &coinIdx) != 1 || coinIdx < 0 || coinIdx >= (int)COIN_KINDS) {
        printf("\n[!] Invalid coin selection! Nothing taken.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    printf("Enter quantity to take: ");
    if (scanf("%d", &amount) != 1 || amount <= 0) {
        printf("\n[!] Invalid amount! Nothing taken.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    if ((uint16_t)amount > coinCount[coinIdx]) {
        printf("\n[!] Refused: Insufficient coins available (%u in bank).\n", coinCount[coinIdx]);
        return;
    }

    coinCount[coinIdx] -= (uint16_t)amount;
    printf("\n[-] Successfully withdrew %d coin(s) of %u piastres.\n", amount, COIN_VALUE[coinIdx]);
}

static uint32_t bankTotal(void) {
    uint32_t total = 0U;
    for (uint8_t i = 0U; i < COIN_KINDS; ++i) {
        total += (uint32_t)coinCount[i] * COIN_VALUE[i];
    }
    return total;
}

static uint32_t sumCoins(const uint16_t *counts, uint8_t n) {
    if (n == 0U) {
        return 0U;
    }
    return (uint32_t)counts[n - 1U] + sumCoins(counts, n - 1U);
}

static uint8_t biggestPile(void) {
    uint8_t maxIdx = 0U;
    for (uint8_t i = 1U; i < COIN_KINDS; ++i) {
        if (coinCount[i] > coinCount[maxIdx]) {
            maxIdx = i;
        }
    }
    return maxIdx;
}

static void drawBar(uint16_t value, uint16_t full, uint8_t width) {
    if (full == 0U) {
        full = 1U;
    }

    uint16_t clampedVal = (value > full) ? full : value;
    uint8_t hashes = (uint8_t)(((uint32_t)clampedVal * width) / full);

    for (uint8_t i = 0U; i < width; ++i) {
        if (i < hashes) {
            putchar('#');
        } else {
            putchar('-');
        }
    }
}

static void showBank(void) {
    uint16_t maxCount = coinCount[biggestPile()];
    if (maxCount == 0U) {
        maxCount = 1U;
    }

    printf("\n--- Piggy Bank Inventory ---\n");
    for (uint8_t i = 0U; i < COIN_KINDS; ++i) {
        printf("%3u piastres | Count: %3u | [", COIN_VALUE[i], coinCount[i]);
        drawBar(coinCount[i], maxCount, 20U);
        printf("]\n");
    }
}

static void buyToy(void) {
    int selection = 0;
    int c = 0;
    uint32_t currentMoney = bankTotal();

    printf("\n--- Toy Shop ---\n");
    for (uint8_t i = 0U; i < TOY_COUNT; ++i) {
        printf("[%u] %-15s - Price: %u piastres\n", i, shop[i].name, shop[i].price);
    }
    printf("Current Piggy Bank Balance: %u piastres\n", currentMoney);
    printf("Select a toy index (0-3): ");

    if (scanf("%d", &selection) != 1 || selection < 0 || selection >= (int)TOY_COUNT) {
        printf("\n[!] Invalid toy selection!\n");
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    Toy_t chosen = shop[(uint8_t)selection];
    if (currentMoney >= chosen.price) {
        printf("\n[+] You CAN buy the %s!", chosen.name);
        printf("\n    Remaining balance would be: %u piastres\n", currentMoney - chosen.price);
    } else {
        printf("\n[-] You cannot afford the %s yet.", chosen.name);
        printf("\n    You need %u more piastres.\n", chosen.price - currentMoney);
    }
}

static void bankReport(void) {
    uint32_t totalVal = bankTotal();
    uint32_t totalCoins = sumCoins(coinCount, COIN_KINDS);
    uint8_t biggestIdx = biggestPile();
    uint8_t affordableToys = 0U;

    for (uint8_t i = 0U; i < TOY_COUNT; ++i) {
        if (totalVal >= shop[i].price) {
            affordableToys++;
        }
    }

    printf("\n====================================\n");
    printf("        PIGGY BANK SUMMARY REPORT   \n");
    printf("====================================\n");
    printf("Total Balance        : %u piastres\n", totalVal);
    printf("Total Coin Count     : %u coins\n", totalCoins);
    printf("Tallest Coin Pile    : %u piastres (%u coins)\n", COIN_VALUE[biggestIdx], coinCount[biggestIdx]);
    printf("Affordable Toys Today: %u of %u\n", affordableToys, TOY_COUNT);
    printf("====================================\n");
}