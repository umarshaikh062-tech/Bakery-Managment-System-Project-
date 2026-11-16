#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ITEMS 10
#define MAX_ORDERS 100

/* =========================================================
   SHARED BAKERY DATA
   All modules use these same product arrays.
   ========================================================= */

char *itemNames[ITEMS] = {
    "Plain Bread (Large Loaf)",
    "Milk Rusk (Pack)",
    "Custom Cake (Made to Order)",
    "Chicken Patty",
    "Cream Roll",
    "Butter Cookies (250 g)",
    "Cheese Pizza Slice",
    "Donut (Chocolate Glazed)",
    "Cheesecake (1 lb)",
    "Swiss Roll"};

float price[ITEMS] = {
    250.0, 300.0, 1000.0, 150.0, 120.0,
    450.0, 350.0, 180.0, 1500.0, 500.0};

float discount[ITEMS] = {0};

int initialStock[ITEMS] = {
    20, 15, 30, 30, 25, 12, 15, 20, 15, 10};

int currentStock[ITEMS] = {
    20, 15, 30, 30, 25, 12, 15, 20, 15, 10};

int quantitySold[ITEMS] = {0};
float totalSales[ITEMS] = {0};

const char *customCakeFlavors[3] = {"Red Velvet Cake", "Vanilla Cake", "Chocolate Cake"};
int customCakeStock[3] = {10, 10, 10};

/* Expiry dates: day, month and year */
int expiryDay[ITEMS] = {
    25, 28, 30, 26, 27, 31, 25, 29, 30, 28};

int expiryMonth[ITEMS] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

int expiryYear[ITEMS] = {
    2026, 2026, 2026, 2026, 2026,
    2026, 2026, 2026, 2026, 2026};

void customerMenu(void);
void viewDiscounts(void);
void purchaseItems(const char *itemNames[], float productData[][2], int currentStock[], int n);
void cancelCustomerOrder(void);

/* =========================================================
   FUNCTION PROTOTYPES
   ========================================================= */

/* Login and main menus */
int adminLogin(void);
void adminMenu(void);

/* Stock management */
void displayProducts(void);
void displayStock(void);
void addStock(void);
void sellItems(void);
void checkLowStock(void);

/* Expiry management */
void expiryMenu(void);
void displayExpiry(void);
void checkExpired(void);
void updateExpiry(void);

/* Sales and discounts */
void viewSalesReport(void);
void addDiscount(void);

/* Search and sorting */
void searchSortMenu(void);
void linearSearch(void);
void binarySearch(void);
void sortItems(int sortedIndex[]);

/* Order management */
void manageOrders(void);
void addOrder(void);
void viewOrders(void);
void searchOrder(void);
void updateOrder(void);
void cancelOrder(void);

/* Input helper */
void clearInputBuffer(void);
int findItemIndex(const char *name);
int findOrderProductIndex(const char *name);
int customCakeFlavorFromOrderName(const char *name);
int configureCustomCake(char details[], int detailsSize, float *unitPrice, int *flavorIndex);
void reduceItemStock(int itemIndex, const char *storedName, int quantity);
void restoreItemStock(int itemIndex, const char *storedName, int quantity);
int customCakeStockTotal(void);

/* =========================================================
   MAIN FUNCTION
   ========================================================= */

/* =========================================================
   INPUT HELPER
   ========================================================= */

int findItemIndex(const char *name)
{
    int i;
    for (i = 0; i < ITEMS; i++)
    {
        if (strcmp(itemNames[i], name) == 0)
            return i;
    }
    return -1;
}

int customCakeStockTotal(void)
{
    return customCakeStock[0] + customCakeStock[1] + customCakeStock[2];
}

int findOrderProductIndex(const char *name)
{
    int exact = findItemIndex(name);
    if (exact >= 0)
        return exact;
    if (strncmp(name, "Custom Cake [", 13) == 0)
        return 2;
    return -1;
}

int customCakeFlavorFromOrderName(const char *name)
{
    if (strstr(name, "Red Velvet Cake") != NULL)
        return 0;
    if (strstr(name, "Vanilla Cake") != NULL)
        return 1;
    if (strstr(name, "Chocolate Cake") != NULL)
        return 2;
    return -1;
}


int configureCustomCake(char details[], int detailsSize, float *unitPrice, int *flavorIndex)
{
    int flavorChoice, toppingChoice, sizeChoice;
    int result = 0;
    int ch;
    size_t messageCapacity = 10;
    size_t messageLength = 0;
    char topping[30], size[20];
    char *message;

    message = (char *)malloc(messageCapacity * sizeof(char));

    if (message == NULL)
        goto memory_error;

    goto allocation_success;

memory_error:
    printf("Memory allocation failed!\n");
    goto cleanup;

allocation_success:
    printf("\n========== CUSTOM CAKE MENU ==========\n");
    printf("Available flavor stock:\n");
    for (int i = 0; i < 3; i++)
        printf("%d. %-20s Stock: %d\n", i + 1, customCakeFlavors[i], customCakeStock[i]);

    printf("Select Cake Flavor (1-3): ");
    if (scanf("%d", &flavorChoice) != 1)
    {
        printf("Invalid flavor choice!\n");
        clearInputBuffer();
        goto cleanup;
    }

    if (flavorChoice < 1 || flavorChoice > 3)
    {
        printf("Invalid flavor choice!\n");
        goto cleanup;
    }

    *flavorIndex = flavorChoice - 1;

    if (customCakeStock[*flavorIndex] <= 0)
    {
        printf("Sorry, this flavor is out of stock.\n");
        goto cleanup;
    }

    printf("\nSelect Topping:\n1. Choco Chips\n2. Cherries\n3. Nuts\n4. No Topping\nEnter choice: ");
    if (scanf("%d", &toppingChoice) != 1)
    {
        printf("Invalid topping choice!\n");
        clearInputBuffer();
        goto cleanup;
    }

    switch (toppingChoice)
    {
    case 1:
        strcpy(topping, "Choco Chips");
        break;
    case 2:
        strcpy(topping, "Cherries");
        break;
    case 3:
        strcpy(topping, "Nuts");
        break;
    case 4:
        strcpy(topping, "No Topping");
        break;
    default:
        printf("Invalid topping choice!\n");
        goto cleanup;
    }

    printf("\nSelect Cake Size:\n1. Small - Rs. 1000\n2. Medium - Rs. 1500\n3. Large - Rs. 2000\nEnter choice: ");
    if (scanf("%d", &sizeChoice) != 1)
    {
        printf("Invalid size choice!\n");
        clearInputBuffer();
        goto cleanup;
    }

    switch (sizeChoice)
    {
    case 1:
        strcpy(size, "Small");
        *unitPrice = 1000.0f;
        break;
    case 2:
        strcpy(size, "Medium");
        *unitPrice = 1500.0f;
        break;
    case 3:
        strcpy(size, "Large");
        *unitPrice = 2000.0f;
        break;
    default:
        printf("Invalid size choice!\n");
        goto cleanup;
    }

    printf("Enter custom message for cake (maximum 50 characters): ");

    /* Skip leading whitespace, as the original scanf format did. */
    ch = getchar();
    while (ch != EOF && (ch == ' ' || ch == '\t' || ch == '\n' ||
                         ch == '\r' || ch == '\v' || ch == '\f'))
        ch = getchar();

    if (ch == EOF)
    {
        const char defaultMessage[] = "No message";
        char *resizedMessage = (char *)realloc(message, sizeof(defaultMessage));

        if (resizedMessage == NULL)
            goto memory_error;

        message = resizedMessage;
        memcpy(message, defaultMessage, sizeof(defaultMessage));
        messageLength = sizeof(defaultMessage) - 1;
        messageCapacity = sizeof(defaultMessage);
    }
    else
    {
        /* Expand the message allocation only when the entered text needs more room. */
        while (ch != '\n' && ch != EOF && messageLength < 50)
        {
            if (messageLength + 1 >= messageCapacity)
            {
                size_t newCapacity = messageCapacity * 2;
                char *resizedMessage;

                if (newCapacity > 51)
                    newCapacity = 51;

                resizedMessage = (char *)realloc(message, newCapacity * sizeof(char));
                if (resizedMessage == NULL)
                    goto memory_error;

                message = resizedMessage;
                messageCapacity = newCapacity;
            }

            message[messageLength++] = (char)ch;

            /* Do not consume characters beyond the existing 50-character input limit. */
            if (messageLength < 50)
                ch = getchar();
        }

        message[messageLength] = '\0';
    }

    /* Reduce unused capacity to the actual message size when possible. */
    if (messageLength + 1 < messageCapacity)
    {
        char *resizedMessage = (char *)realloc(message, (messageLength + 1) * sizeof(char));
        if (resizedMessage != NULL)
            message = resizedMessage;
    }

    for (int i = 0; message[i] != '\0'; i++)
        if (message[i] == '|')
            message[i] = '/';

    snprintf(details, detailsSize,
             "Custom Cake [%s; %s; %s; Message: %s]",
             customCakeFlavors[*flavorIndex], topping, size, message);

    printf("\n%s\n", details);

    result = 1;

cleanup:

    free(message);

    return result;
}


void reduceItemStock(int itemIndex, const char *storedName, int quantity)
{
    if (itemIndex == 2)
    {
        int flavor = customCakeFlavorFromOrderName(storedName);
        if (flavor >= 0 && quantity <= customCakeStock[flavor])
            customCakeStock[flavor] -= quantity;
        currentStock[2] = customCakeStockTotal();
    }
    else if (itemIndex >= 0)
        currentStock[itemIndex] -= quantity;
}

void restoreItemStock(int itemIndex, const char *storedName, int quantity)
{
    if (itemIndex == 2)
    {
        int flavor = customCakeFlavorFromOrderName(storedName);
        if (flavor >= 0)
            customCakeStock[flavor] += quantity;
        currentStock[2] = customCakeStockTotal();
    }
    else if (itemIndex >= 0)
        currentStock[itemIndex] += quantity;
}

void clearInputBuffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* Clear remaining input */
    }
}

/* =========================================================
   1. ADMIN LOGIN
   ========================================================= */

int adminLogin(void)
{
    char admin[] = "bakeryadmin";
    char pass1[] = "1122";

    char user[50];
    char pass2[50];

    int attempts = 0;

    while (attempts < 3)
    {
        printf("\nEnter Username: ");

        if (scanf("%49s", user) != 1)
            return 0;

        printf("Enter login PIN: ");

        if (scanf("%49s", pass2) != 1)
            return 0;

        if (strcmp(admin, user) == 0 && strcmp(pass1, pass2) == 0)
        {
            printf("\nLogin Successful!\n");
            return 1;
        }

        attempts++;

        printf("\nLogin Unsuccessful!\n");
        printf("Attempts remaining: %d\n", 3 - attempts);
    }

    printf("\nLogin Access Locked! Too many incorrect attempts.\n");

    return 0;
}

/* =========================================================
   MAIN ADMIN MENU
   ========================================================= */

void adminMenu(void)
{
    int choice;

    do
    {
        printf("\n\n============================================\n");
        printf("              ADMIN MAIN MENU               \n");
        printf("============================================\n");
        printf("1. Stock Management\n");
        printf("2. Expiry Management\n");
        printf("3. Sales Report\n");
        printf("4. Discount Management\n");
        printf("5. Search and Sort Products\n");
        printf("6. Order Management\n");
        printf("7. View Product Menu\n");
        printf("8. Logout and Exit\n");
        printf("--------------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Enter a number.\n");
            clearInputBuffer();
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            int stockChoice;

            do
            {
                printf("\n========== STOCK MANAGEMENT ==========\n");
                printf("1. Display Current Stock\n");
                printf("2. Add Stock\n");
                printf("3. Sell Items\n");
                printf("4. Check Low Stock\n");
                printf("5. Back to Admin Menu\n");
                printf("Enter choice: ");

                if (scanf("%d", &stockChoice) != 1)
                {
                    printf("Invalid input!\n");
                    clearInputBuffer();
                    continue;
                }

                switch (stockChoice)
                {
                case 1:
                    displayStock();
                    break;
                case 2:
                    addStock();
                    break;
                case 3:
                    sellItems();
                    break;
                case 4:
                    checkLowStock();
                    break;
                case 5:
                    printf("Returning to Admin Menu...\n");
                    break;
                default:
                    printf("Invalid choice! Select 1-5.\n");
                }

            } while (stockChoice != 5);

            break;
        }

        case 2:
            expiryMenu();
            break;

        case 3:
            viewSalesReport();
            break;

        case 4:
            addDiscount();
            break;

        case 5:
            searchSortMenu();
            break;

        case 6:
            manageOrders();
            break;

        case 7:
            displayProducts();
            break;

        case 8:
            printf("\nLogging out...\n");
            break;

        default:
            printf("Invalid choice! Select 1-8.\n");
        }

    } while (choice != 8);
}

/* =========================================================
   2. STOCK MANAGEMENT
   ========================================================= */

void displayProducts(void)
{
    int i;

    printf("\n====================================================================\n");
    printf("                       BAKERY PRODUCTS MENU                         \n");
    printf("====================================================================\n");
    printf("%-5s %-38s %16s\n", "NO.", "ITEM NAME", "PRICE (Rs.)");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < ITEMS; i++)
    {
        printf("%-5d %-38s %16.2f\n",
               i + 1, itemNames[i], price[i]);
    }

    printf("====================================================================\n");
}

void displayStock(void)
{
    int i;

    printf("\n==========================================================================\n");
    printf("                         CURRENT BAKERY STOCK                             \n");
    printf("==========================================================================\n");
    printf("%-5s %-38s %12s %10s\n",
           "NO.", "ITEM NAME", "PRICE/FROM", "STOCK");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < ITEMS; i++)
    {
        printf("%-5d %-38s %12.2f %10d\n",
               i + 1, itemNames[i], price[i], currentStock[i]);
    }

    printf("==========================================================================\n");
    printf("Custom Cake flavor stock: Red Velvet=%d, Vanilla=%d, Chocolate=%d\n", customCakeStock[0], customCakeStock[1], customCakeStock[2]);
}

void addStock(void)
{
    int item, quantity, flavor;
    displayProducts();
    printf("\nEnter item number to add stock (1-%d): ", ITEMS);
    if (scanf("%d", &item) != 1)
    {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    if (item < 1 || item > ITEMS)
    {
        printf("Invalid item number!\n");
        return;
    }
    if (item == 3)
    {
        printf("\nSelect flavor to restock:\n1. Red Velvet\n2. Vanilla\n3. Chocolate\nChoice: ");
        if (scanf("%d", &flavor) != 1)
        {
            clearInputBuffer();
            printf("Invalid choice!\n");
            return;
        }
        if (flavor < 1 || flavor > 3)
        {
            printf("Invalid flavor!\n");
            return;
        }
        printf("Current %s stock: %d\nEnter quantity to add: ", customCakeFlavors[flavor - 1], customCakeStock[flavor - 1]);
    }
    else
        printf("\nSelected Item: %s\nCurrent Stock: %d\nEnter quantity to add: ", itemNames[item - 1], currentStock[item - 1]);
    if (scanf("%d", &quantity) != 1)
    {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    if (quantity <= 0)
    {
        printf("Quantity must be greater than zero!\n");
        return;
    }
    if (item == 3)
    {
        customCakeStock[flavor - 1] += quantity;
        currentStock[2] = customCakeStockTotal();
        printf("Stock added. %s stock: %d | Total Custom Cake stock: %d\n", customCakeFlavors[flavor - 1], customCakeStock[flavor - 1], currentStock[2]);
    }
    else
    {
        currentStock[item - 1] += quantity;
        printf("Stock added successfully! New stock: %d\n", currentStock[item - 1]);
    }
}

void sellItems(void)
{
    int item, quantity, flavor = -1;
    float finalPrice;
    char customDetails[200] = "";
    displayProducts();
    printf("\nEnter item number to sell (1-%d): ", ITEMS);
    if (scanf("%d", &item) != 1)
    {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    if (item < 1 || item > ITEMS)
    {
        printf("Invalid item number!\n");
        return;
    }
    if (item == 3)
    {
        if (!configureCustomCake(customDetails, sizeof(customDetails), &finalPrice, &flavor))
            return;
        finalPrice -= finalPrice * discount[2] / 100.0f;
        printf("Available %s stock: %d\n", customCakeFlavors[flavor], customCakeStock[flavor]);
    }
    else
    {
        finalPrice = price[item - 1] - (price[item - 1] * discount[item - 1] / 100.0f);
        printf("Selected Item: %s\nOriginal Price: Rs. %.2f\nDiscount: %.2f%%\nAvailable Stock: %d\n", itemNames[item - 1], price[item - 1], discount[item - 1], currentStock[item - 1]);
    }
    printf("Enter quantity sold: ");
    if (scanf("%d", &quantity) != 1)
    {
        printf("Invalid quantity!\n");
        clearInputBuffer();
        return;
    }
    if (quantity <= 0)
    {
        printf("Quantity must be greater than zero!\n");
        return;
    }
    if ((item == 3 && quantity > customCakeStock[flavor]) || (item != 3 && quantity > currentStock[item - 1]))
    {
        printf("Not enough stock! Sale rejected.\n");
        return;
    }
    if (item == 3)
        reduceItemStock(2, customDetails, quantity);
    else
        reduceItemStock(item - 1, itemNames[item - 1], quantity);
    quantitySold[item - 1] += quantity;
    totalSales[item - 1] += finalPrice * quantity;
    printf("\nSale recorded successfully!\nItem: %s\nQuantity Sold: %d\nPrice per Item: Rs. %.2f\nTotal Amount: Rs. %.2f\nRemaining Stock: %d\n", item == 3 ? customDetails : itemNames[item - 1], quantity, finalPrice, finalPrice * quantity, currentStock[item - 1]);
}

void checkLowStock(void)
{
    int i, found = 0;

    printf("\n============================================================\n");
    printf("                    LOW STOCK ALERT                         \n");
    printf("============================================================\n");
    printf("%-5s %-38s %10s\n", "NO.", "ITEM NAME", "REMAINING");
    printf("------------------------------------------------------------\n");

    for (i = 0; i < ITEMS; i++)
    {
        if (currentStock[i] < 5)
        {
            printf("%-5d %-38s %10d  ** LOW **\n",
                   i + 1, itemNames[i], currentStock[i]);
            found = 1;
        }
    }

    for (i = 0; i < 3; i++)
    {
        if (customCakeStock[i] < 5)
        {
            printf("Custom Cake - %-20s %10d  ** LOW **\n", customCakeFlavors[i], customCakeStock[i]);
            found = 1;
        }
    }

    if (found == 0)
        printf("All items have sufficient stock.\n");

    printf("============================================================\n");
}

/* =========================================================
   3. EXPIRY MANAGEMENT
   ========================================================= */

void expiryMenu(void)
{
    int choice;

    do
    {
        printf("\n========== EXPIRY MANAGEMENT ==========\n");
        printf("1. View Expiry Dates\n");
        printf("2. Check Expired Items\n");
        printf("3. Update Expiry Date\n");
        printf("4. Return to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            clearInputBuffer();
            continue;
        }

        switch (choice)
        {
        case 1:
            displayExpiry();
            break;

        case 2:
            checkExpired();
            break;

        case 3:
            updateExpiry();
            break;

        case 4:
            printf("Returning to Main Menu...\n");
            break;

        default:
            printf("Invalid choice!\n");
        }

    } while (choice != 4);
}

void displayExpiry(void)
{
    int i;

    printf("\n==================== BAKERY EXPIRY DATES ====================\n");

    for (i = 0; i < ITEMS; i++)
    {
        printf("%2d. %-38s - %02d/%02d/%04d\n",
               i + 1, itemNames[i],
               expiryDay[i], expiryMonth[i], expiryYear[i]);
    }

    printf("=============================================================\n");
}

void checkExpired(void)
{
    int today, currentMonth, currentYear;
    int i, found = 0;

    printf("\nEnter today's date (DD MM YYYY): ");

    if (scanf("%d %d %d", &today, &currentMonth, &currentYear) != 3)
    {
        printf("Invalid date input!\n");
        clearInputBuffer();
        return;
    }

    if (today < 1 || today > 31 ||
        currentMonth < 1 || currentMonth > 12 ||
        currentYear < 1)
    {
        printf("Invalid date!\n");
        return;
    }

    printf("\n========== EXPIRED ITEMS ==========\n");

    for (i = 0; i < ITEMS; i++)
    {
        if (expiryYear[i] < currentYear ||
            (expiryYear[i] == currentYear &&
             expiryMonth[i] < currentMonth) ||
            (expiryYear[i] == currentYear &&
             expiryMonth[i] == currentMonth &&
             expiryDay[i] < today))
        {
            printf("%s - EXPIRED\n", itemNames[i]);
            found = 1;
        }
    }

    if (found == 0)
        printf("No expired items found.\n");

    printf("===================================\n");
}

void updateExpiry(void)
{
    int item, d, m, y;

    printf("\nEnter item number (1-%d): ", ITEMS);

    if (scanf("%d", &item) != 1)
    {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }

    if (item < 1 || item > ITEMS)
    {
        printf("Invalid item number!\n");
        return;
    }

    printf("Selected item: %s\n", itemNames[item - 1]);
    printf("Enter new expiry date (DD MM YYYY): ");

    if (scanf("%d %d %d", &d, &m, &y) != 3)
    {
        printf("Invalid date input!\n");
        clearInputBuffer();
        return;
    }

    if (d >= 1 && d <= 31 && m >= 1 && m <= 12 && y > 0)
    {
        expiryDay[item - 1] = d;
        expiryMonth[item - 1] = m;
        expiryYear[item - 1] = y;

        printf("Expiry date updated successfully!\n");
    }
    else
    {
        printf("Invalid date!\n");
    }
}

/* =========================================================
   4. SALES REPORT
   ========================================================= */

void viewSalesReport(void)
{
    int i;
    float grandTotal = 0;
    printf("\n============================== SALES REPORT ==============================\n\n");
    printf("%-38s %10s %10s %8s %15s\n", "Item", "Initial", "Remaining", "Sold", "Revenue");
    printf("-------------------------------------------------------------------------------\n");
    for (i = 0; i < ITEMS; i++)
    {
        printf("%-38s %10d %10d %8d Rs. %10.2f\n", itemNames[i], initialStock[i], currentStock[i], quantitySold[i], totalSales[i]);
        grandTotal += totalSales[i];
    }
    printf("-------------------------------------------------------------------------------\n%-65s Rs. %10.2f\n", "Grand Total Sales:", grandTotal);
    printf("===============================================================================\n");
}

/* =========================================================
   5. DISCOUNT MANAGEMENT
   ========================================================= */

void addDiscount(void)
{
    char searchName[100];
    int i, found = 0;
    float percentage;

    displayProducts();

    printf("\nEnter product name exactly as shown: ");

    if (scanf(" %99[^\n]", searchName) != 1)
    {
        printf("Invalid product name input!\n");
        return;
    }

    for (i = 0; i < ITEMS; i++)
    {
        if (strcmp(searchName, itemNames[i]) == 0)
        {
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        printf("\nProduct not found!\n");
        return;
    }

    printf("\nProduct found : %s\n", itemNames[i]);
    printf("Original Price: Rs. %.2f\n", price[i]);
    printf("Current Discount: %.2f%%\n", discount[i]);

    printf("Enter discount percentage (1-100): ");

    if (scanf("%f", &percentage) != 1)
    {
        printf("Invalid discount input!\n");
        clearInputBuffer();
        return;
    }

    if (percentage < 1 || percentage > 100)
    {
        printf("Invalid discount percentage!\n");
        return;
    }

    discount[i] = percentage;

    printf("\nDiscount updated successfully!\n");
    printf("Product         : %s\n", itemNames[i]);
    printf("Discount        : %.2f%%\n", discount[i]);
    printf("Discount Amount : Rs. %.2f\n",
           price[i] * discount[i] / 100.0f);
    printf("Final Price     : Rs. %.2f\n",
           price[i] - (price[i] * discount[i] / 100.0f));
}

/* =========================================================
   6. SEARCH AND SORT
   ========================================================= */

/* Sort product indexes alphabetically.
   The original product arrays remain in their original order. */
void sortItems(int sortedIndex[])
{
    int i, j, temp;

    for (i = 0; i < ITEMS; i++)
        sortedIndex[i] = i;

    for (i = 0; i < ITEMS - 1; i++)
    {
        for (j = 0; j < ITEMS - i - 1; j++)
        {
            if (strcmp(itemNames[sortedIndex[j]],
                       itemNames[sortedIndex[j + 1]]) > 0)
            {
                temp = sortedIndex[j];
                sortedIndex[j] = sortedIndex[j + 1];
                sortedIndex[j + 1] = temp;
            }
        }
    }
}

void linearSearch(void)
{
    char search[100];
    int i, found = 0;

    printf("\nEnter item name exactly as shown: ");

    if (scanf(" %99[^\n]", search) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    for (i = 0; i < ITEMS; i++)
    {
        if (strcmp(itemNames[i], search) == 0)
        {
            printf("\nItem Found: %s\n", itemNames[i]);
            printf("Price: Rs. %.2f\n", price[i]);
            printf("Stock: %d\n", currentStock[i]);
            found = 1;
            break;
        }
    }

    if (found == 0)
        printf("\nItem Not Found!\n");
}

void binarySearch(void)
{
    char search[100];
    int sortedIndex[ITEMS];
    int low = 0, high = ITEMS - 1;
    int mid, found = 0;
    int actualIndex;

    sortItems(sortedIndex);

    printf("\nEnter item name exactly as shown: ");

    if (scanf(" %99[^\n]", search) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    while (low <= high)
    {
        mid = (low + high) / 2;
        actualIndex = sortedIndex[mid];

        if (strcmp(itemNames[actualIndex], search) == 0)
        {
            printf("\nItem Found: %s\n", itemNames[actualIndex]);
            printf("Price: Rs. %.2f\n", price[actualIndex]);
            printf("Stock: %d\n", currentStock[actualIndex]);
            found = 1;
            break;
        }
        else if (strcmp(search, itemNames[actualIndex]) < 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }

    if (found == 0)
        printf("\nItem Not Found!\n");
}

void searchSortMenu(void)
{
    int choice;

    do
    {
        printf("\n========== SEARCH AND SORT ==========\n");
        printf("1. Linear Search\n");
        printf("2. Binary Search\n");
        printf("3. Back to Admin Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            clearInputBuffer();
            continue;
        }

        switch (choice)
        {
        case 1:
            linearSearch();
            break;

        case 2:
            binarySearch();
            break;

        case 3:
            printf("\nReturning to Admin Menu...\n");
            break;

        default:
            printf("\nInvalid Choice!\n");
        }

    } while (choice != 3);
}

/* =========================================================
   7. ORDER MANAGEMENT
   Orders are stored permanently in orders.txt.
   ========================================================= */

struct Order
{
    int orderID;
    char customerName[30];
    char itemName[200];
    int quantity;
    float itemPrice;
    float totalBill;
    char status[20];
};

void addOrder(void)
{
    struct Order o[ITEMS], tempOrder;
    FILE *fp, *readfp;
    int itemCount, i, item, orderID = 0, j, alreadyOrdered;
    float grandTotal = 0;
    printf("\nEnter Customer Name: ");
    if (scanf(" %29[^\n]", o[0].customerName) != 1)
    {
        printf("Invalid customer name!\n");
        return;
    }
    printf("\nHow many different items do you want to order? ");
    if (scanf("%d", &itemCount) != 1)
    {
        printf("Invalid number of items!\n");
        clearInputBuffer();
        return;
    }
    if (itemCount < 1 || itemCount > ITEMS)
    {
        printf("You can order between 1 and %d different items.\n", ITEMS);
        return;
    }
    readfp = fopen("orders.txt", "r");
    if (readfp != NULL)
    {
        while (fscanf(readfp, "%d|%29[^|]|%199[^|]|%d|%f|%f|%19[^\n]", &tempOrder.orderID, tempOrder.customerName, tempOrder.itemName, &tempOrder.quantity, &tempOrder.itemPrice, &tempOrder.totalBill, tempOrder.status) == 7)
            if (tempOrder.orderID > orderID)
                orderID = tempOrder.orderID;
        fclose(readfp);
    }
    orderID++;
    displayProducts();
    for (i = 0; i < itemCount; i++)
    {
        int flavor = -1;
        if (i > 0)
            strcpy(o[i].customerName, o[0].customerName);
        o[i].orderID = orderID;
        strcpy(o[i].status, "Pending");
        printf("\n========== PRODUCT %d OF %d ==========\nEnter product number (1-%d): ", i + 1, itemCount, ITEMS);
        if (scanf("%d", &item) != 1)
        {
            printf("Invalid product number!\n");
            clearInputBuffer();
            return;
        }
        if (item < 1 || item > ITEMS)
        {
            printf("Invalid product number!\n");
            return;
        }
        if (item == 3)
        {
            if (!configureCustomCake(o[i].itemName, sizeof(o[i].itemName), &o[i].itemPrice, &flavor))
                return;
            o[i].itemPrice -= o[i].itemPrice * discount[2] / 100.0f;
        }
        else
        {
            strcpy(o[i].itemName, itemNames[item - 1]);
            o[i].itemPrice = price[item - 1] - price[item - 1] * discount[item - 1] / 100.0f;
        }
        printf("Price per Item after discount: Rs. %.2f\n", o[i].itemPrice);
        printf("Available Stock: %d\n", item == 3 ? customCakeStock[flavor] : currentStock[item - 1]);
        printf("Enter Quantity: ");
        if (scanf("%d", &o[i].quantity) != 1)
        {
            printf("Invalid quantity!\n");
            clearInputBuffer();
            return;
        }
        if (o[i].quantity <= 0)
        {
            printf("Quantity must be greater than zero!\n");
            return;
        }
        alreadyOrdered = 0;
        for (j = 0; j < i; j++)
        {
            if (item == 3 && customCakeFlavorFromOrderName(o[j].itemName) == flavor)
                alreadyOrdered += o[j].quantity;
            else if (item != 3 && strcmp(o[j].itemName, o[i].itemName) == 0)
                alreadyOrdered += o[j].quantity;
        }
        if (item == 3 && o[i].quantity + alreadyOrdered > customCakeStock[flavor])
        {
            printf("Not enough stock for this flavor. Available before this order: %d\n", customCakeStock[flavor] - alreadyOrdered);
            return;
        }
        if (item != 3 && o[i].quantity + alreadyOrdered > currentStock[item - 1])
        {
            printf("Not enough stock for %s. Available before this order: %d\n", o[i].itemName, currentStock[item - 1] - alreadyOrdered);
            return;
        }
        o[i].totalBill = o[i].itemPrice * o[i].quantity;
        grandTotal += o[i].totalBill;
    }
    fp = fopen("orders.txt", "a");
    if (fp == NULL)
    {
        printf("\nCould not open orders.txt. Order was not placed.\n");
        return;
    }
    for (i = 0; i < itemCount; i++)
    {
        if (fprintf(fp, "%d|%s|%s|%d|%.2f|%.2f|%s\n", o[i].orderID, o[i].customerName, o[i].itemName, o[i].quantity, o[i].itemPrice, o[i].totalBill, o[i].status) < 0)
        {
            fclose(fp);
            printf("Could not save the complete order. Check orders.txt.\n");
            return;
        }
    }
    if (fclose(fp) != 0)
    {
        printf("Could not close orders.txt cleanly; verify the saved order.\n");
        return;
    }
    for (i = 0; i < itemCount; i++)
    {
        int index = findOrderProductIndex(o[i].itemName);
        reduceItemStock(index, o[i].itemName, o[i].quantity);
        if (index >= 0)
        {
            quantitySold[index] += o[i].quantity;
            totalSales[index] += o[i].totalBill;
        }
    }
    printf("\n========== ORDER SUMMARY ==========\nOrder ID: %d\nCustomer Name: %s\n", orderID, o[0].customerName);
    for (i = 0; i < itemCount; i++)
        printf("%-70s x %d = Rs. %.2f\n", o[i].itemName, o[i].quantity, o[i].totalBill);
    printf("-----------------------------------\nGrand Total: Rs. %.2f\nOrder Saved Successfully! Stock and sales report updated.\n", grandTotal);
}

void viewOrders(void)
{
    struct Order o;
    FILE *fp;
    int currentOrderID = -1;
    float grandTotal = 0;

    fp = fopen("orders.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo Orders Found!\n");
        return;
    }

    printf("\n==================== ALL ORDERS ====================\n");

    while (fscanf(fp,
                  "%d|%29[^|]|%199[^|]|%d|%f|%f|%19[^\n]",
                  &o.orderID,
                  o.customerName,
                  o.itemName,
                  &o.quantity,
                  &o.itemPrice,
                  &o.totalBill,
                  o.status) == 7)
    {
        if (o.orderID != currentOrderID)
        {
            if (currentOrderID != -1)
                printf("Order Total: Rs. %.2f\n\n", grandTotal);

            currentOrderID = o.orderID;
            grandTotal = 0;

            printf("\nOrder ID: %d\n", o.orderID);
            printf("Customer Name: %s\n", o.customerName);
            printf("Status: %s\n", o.status);
            printf("---------------------------------------------\n");
        }

        printf("%-38s x %d | Rs. %.2f each | Subtotal: Rs. %.2f\n",
               o.itemName, o.quantity, o.itemPrice, o.totalBill);

        grandTotal += o.totalBill;
    }

    if (currentOrderID != -1)
        printf("Order Total: Rs. %.2f\n", grandTotal);

    printf("====================================================\n");

    fclose(fp);
}

void searchOrder(void)
{
    struct Order o;
    char name[30];
    int found = 0, currentOrderID = -1;
    float grandTotal = 0;

    FILE *fp = fopen("orders.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo Orders Found!\n");
        return;
    }

    printf("\nEnter Customer Name: ");

    if (scanf(" %29[^\n]", name) != 1)
    {
        printf("Invalid name!\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp,
                  "%d|%29[^|]|%199[^|]|%d|%f|%f|%19[^\n]",
                  &o.orderID,
                  o.customerName,
                  o.itemName,
                  &o.quantity,
                  &o.itemPrice,
                  &o.totalBill,
                  o.status) == 7)
    {
        if (strcmp(o.customerName, name) == 0)
        {
            if (o.orderID != currentOrderID)
            {
                if (currentOrderID != -1)
                    printf("Order Total: Rs. %.2f\n\n", grandTotal);

                currentOrderID = o.orderID;
                grandTotal = 0;
                found = 1;

                printf("\nOrder Found!\n");
                printf("Order ID: %d\n", o.orderID);
                printf("Customer Name: %s\n", o.customerName);
                printf("Status: %s\n", o.status);
                printf("---------------------------------------------\n");
            }

            printf("%-38s x %d | Rs. %.2f each | Subtotal: Rs. %.2f\n",
                   o.itemName, o.quantity, o.itemPrice, o.totalBill);

            grandTotal += o.totalBill;
        }
    }

    if (currentOrderID != -1)
        printf("Order Total: Rs. %.2f\n", grandTotal);

    if (found == 0)
        printf("\nOrder Not Found!\n");

    fclose(fp);
}

void updateOrder(void)
{
    struct Order o;
    int orderID, found = 0;

    FILE *fp = fopen("orders.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL)
    {
        printf("\nFile cannot be opened!\n");

        if (fp != NULL)
            fclose(fp);

        if (temp != NULL)
            fclose(temp);

        return;
    }

    printf("\nEnter Order ID to update: ");

    if (scanf("%d", &orderID) != 1)
    {
        printf("Invalid Order ID!\n");
        clearInputBuffer();
        fclose(fp);
        fclose(temp);
        remove("temp.txt");
        return;
    }

    while (fscanf(fp,
                  "%d|%29[^|]|%199[^|]|%d|%f|%f|%19[^\n]",
                  &o.orderID,
                  o.customerName,
                  o.itemName,
                  &o.quantity,
                  &o.itemPrice,
                  &o.totalBill,
                  o.status) == 7)
    {
        if (o.orderID == orderID &&
            strcmp(o.status, "Pending") == 0)
        {
            strcpy(o.status, "Completed");
            found = 1;
        }

        fprintf(temp, "%d|%s|%s|%d|%.2f|%.2f|%s\n",
                o.orderID,
                o.customerName,
                o.itemName,
                o.quantity,
                o.itemPrice,
                o.totalBill,
                o.status);
    }

    fclose(fp);
    fclose(temp);

    remove("orders.txt");
    rename("temp.txt", "orders.txt");

    if (found == 1)
        printf("\nOrder Status Updated!\n");
    else
        printf("\nPending Order Not Found!\n");
}

void cancelOrder(void)
{
    struct Order o;
    int orderID, found = 0;
    FILE *fp = fopen("orders.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL)
    {
        printf("\nFile cannot be opened!\n");
        if (fp != NULL)
            fclose(fp);
        if (temp != NULL)
            fclose(temp);
        return;
    }

    printf("\nEnter Order ID to cancel: ");
    if (scanf("%d", &orderID) != 1)
    {
        printf("Invalid Order ID!\n");
        clearInputBuffer();
        fclose(fp);
        fclose(temp);
        remove("temp.txt");
        return;
    }

    while (fscanf(fp, "%d|%29[^|]|%199[^|]|%d|%f|%f|%19[^\n]",
                  &o.orderID, o.customerName, o.itemName, &o.quantity,
                  &o.itemPrice, &o.totalBill, o.status) == 7)
    {
        if (o.orderID == orderID && strcmp(o.status, "Pending") == 0)
        {
            int index = findOrderProductIndex(o.itemName);
            strcpy(o.status, "Cancelled");
            found = 1;
            if (index >= 0)
            {
                restoreItemStock(index, o.itemName, o.quantity);
                quantitySold[index] -= o.quantity;
                totalSales[index] -= o.totalBill;
                if (quantitySold[index] < 0)
                    quantitySold[index] = 0;
                if (totalSales[index] < 0.0f)
                    totalSales[index] = 0.0f;
            }
        }
        fprintf(temp, "%d|%s|%s|%d|%.2f|%.2f|%s\n",
                o.orderID, o.customerName, o.itemName, o.quantity,
                o.itemPrice, o.totalBill, o.status);
    }

    fclose(fp);
    fclose(temp);

    if (remove("orders.txt") != 0 || rename("temp.txt", "orders.txt") != 0)
    {
        printf("\nCould not update the order file. Inventory changes may need checking.\n");
        return;
    }

    if (found)
        printf("\nOrder Cancelled. Stock restored and sales report reversed.\n");
    else
        printf("\nPending Order Not Found!\n");
}

void manageOrders(void)
{
    int choice;

    do
    {
        printf("\n\n========== MANAGE ORDERS ==========\n");
        printf("1. Add Order\n");
        printf("2. View All Orders\n");
        printf("3. Search Order\n");
        printf("4. Update Order Status (by Order ID)\n");
        printf("5. Cancel Order (by Order ID)\n");
        printf("6. Back to Admin Menu\n");
        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input!\n");
            clearInputBuffer();
            continue;
        }

        switch (choice)
        {
        case 1:
            addOrder();
            break;

        case 2:
            viewOrders();
            break;

        case 3:
            searchOrder();
            break;

        case 4:
            updateOrder();
            break;

        case 5:
            cancelOrder();
            break;

        case 6:
            printf("\nReturning to Admin Menu...\n");
            break;

        default:
            printf("\nInvalid Choice!\n");
        }

    } while (choice != 6);
}

/* CUSTOMER CONSOLE DATA AND FUNCTIONS */
struct CustomerOrder
{
    int orderID;
    char customerName[30];
    char itemName[200];
    int quantity;
    float itemPrice;
    float totalBill;
    char status[20];
};

struct CustomerOrder customerOrders[MAX_ORDERS];
int totalCustomerOrders = 0;

void viewDiscounts(void)
{

    printf("\n========== AVAILABLE DISCOUNTS ==========\n");

    int found = 0;

    for (int i = 0; i < ITEMS; i++)
    {
        if (discount[i] > 0)
        {
            printf("%-32s %.2f%%\n", itemNames[i], discount[i]);
            found = 1;
        }
    }

    if (found == 0)
        printf("No discounts are currently available.\n");

    printf("=========================================\n");
}

void purchaseItems(const char *names[], float productData[][2], int stock[], int n)
{
    int totalItems, itemID, quantity, successfulItems = 0;
    char customerName[30];
    char orderNames[ITEMS][200];
    int orderQty[ITEMS];
    float orderUnitPrice[ITEMS];
    float originalTotal = 0;
    float totalDiscount = 0;
    float grandTotal = 0;
    printf("\n========== PURCHASE ITEMS ==========\nEnter customer name: ");
    if (scanf(" %29[^\n]", customerName) != 1)
    {
        printf("Invalid customer name!\n");
        return;
    }
    printf("How many items do you want to purchase? ");
    if (scanf("%d", &totalItems) != 1)
    {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    if (totalItems < 1 || totalItems > n)
    {
        printf("Invalid number of items!\n");
        return;
    }
    printf("\n========== AVAILABLE ITEMS ==========\n");
    for (int j = 0; j < n; j++)
    {
        if (j == 2)
            printf("%-3d %-35s From Rs. %.2f | Total Stock: %d\n", j + 1, names[j], productData[j][0], stock[j]);
        else
            printf("%-3d %-35s Rs. %.2f | Stock: %d\n", j + 1, names[j], productData[j][0], stock[j]);
    }
    printf("Custom Cake flavor stock: Red Velvet %d, Vanilla %d, Chocolate %d\n", customCakeStock[0], customCakeStock[1], customCakeStock[2]);

    for (int i = 0; i < totalItems; i++)
    {
        char selectedName[200];
        float originalPrice;
        int flavor = -1;
        printf("\nEnter item id: ");
        if (scanf("%d", &itemID) != 1)
        {
            printf("Invalid item ID!\n");
            clearInputBuffer();
            continue;
        }
        if (itemID < 1 || itemID > n)
        {
            printf("Invalid item ID!\n");
            continue;
        }
        if (itemID == 3)
        {
            if (!configureCustomCake(selectedName, sizeof(selectedName), &originalPrice, &flavor))
                continue;
        }
        else
        {
            strcpy(selectedName, names[itemID - 1]);
            originalPrice = productData[itemID - 1][0];
        }
        printf("Enter item quantity: ");
        if (scanf("%d", &quantity) != 1)
        {
            printf("Invalid quantity!\n");
            clearInputBuffer();
            continue;
        }
        if (quantity <= 0)
        {
            printf("Invalid quantity!\n");
            continue;
        }
        if ((itemID == 3 && quantity > customCakeStock[flavor]) || (itemID != 3 && quantity > stock[itemID - 1]))
        {
            printf("Not enough stock for selected item!\n");
            if (itemID == 3)
                printf("Available flavor stock: %d\n", customCakeStock[flavor]);
            else
                printf("Available stock: %d\n", stock[itemID - 1]);
            continue;
        }
        float discountRate = productData[itemID - 1][1];
        float discountPerItem = originalPrice * discountRate / 100.0f;
        float discountedPrice = originalPrice - discountPerItem;
        float itemSubtotal = discountedPrice * quantity;
        originalTotal += originalPrice * quantity;
        totalDiscount += discountPerItem * quantity;
        int foundIndex = -1;
        if (itemID != 3)
            for (int j = 0; j < successfulItems; j++)
                if (strcmp(orderNames[j], selectedName) == 0)
                {
                    foundIndex = j;
                    break;
                }
        if (foundIndex >= 0)
            orderQty[foundIndex] += quantity;
        else if (successfulItems < ITEMS)
        {
            strcpy(orderNames[successfulItems], selectedName);
            orderQty[successfulItems] = quantity;
            orderUnitPrice[successfulItems] = discountedPrice;
            successfulItems++;
        }
        else
        {
            printf("Order has reached its item limit.\n");
            continue;
        }
        if (itemID == 3)
            reduceItemStock(2, selectedName, quantity);
        else
            reduceItemStock(itemID - 1, selectedName, quantity);
        quantitySold[itemID - 1] += quantity;
        totalSales[itemID - 1] += itemSubtotal;
        grandTotal += itemSubtotal;
        printf("\nProduct: %s\nQuantity: %d\nOriginal Price: Rs. %.2f\nDiscount: %.2f%%\nDiscounted Price: Rs. %.2f\nSubtotal: Rs. %.2f\n", selectedName, quantity, originalPrice, discountRate, discountedPrice, itemSubtotal);
        if (itemID == 3)
            printf("Remaining %s stock: %d\n", customCakeFlavors[flavor], customCakeStock[flavor]);
        else
            printf("Remaining Stock: %d\n", stock[itemID - 1]);
    }
    if (successfulItems == 0)
    {
        printf("\nNo items were successfully purchased. No bill was generated.\n");
        return;
    }

    /* Save customer order in orders.txt */

    int customerOrderID = 0;
    int savedQuantity;
    float savedPrice, savedTotal;
    char savedCustomerName[30];
    char savedItemName[200];
    char savedStatus[20];

    FILE *readfp;
    FILE *writefp;

    readfp = fopen("orders.txt", "r");

    if (readfp != NULL)
    {
        while (fscanf(readfp, "%d|%29[^|]|%199[^|]|%d|%f|%f|%19[^\n]",
                      &customerOrderID,
                      savedCustomerName,
                      savedItemName,
                      &savedQuantity,
                      &savedPrice,
                      &savedTotal,
                      savedStatus) == 7)
        {
            /* Find the highest existing order ID */
        }

        fclose(readfp);
    }

    customerOrderID++;

    writefp = fopen("orders.txt", "a");

    if (writefp == NULL)
    {
        printf("\nERROR: Customer order could not be saved!\n");
    }
    else
    {
        int saveSuccessful = 1;

        for (int i = 0; i < successfulItems; i++)
        {
            float itemSubtotal = orderUnitPrice[i] * orderQty[i];

            if (fprintf(writefp, "%d|%s|%s|%d|%.2f|%.2f|Pending\n",
                        customerOrderID,
                        customerName,
                        orderNames[i],
                        orderQty[i],
                        orderUnitPrice[i],
                        itemSubtotal) < 0)
            {
                saveSuccessful = 0;
                break;
            }
        }

        if (fclose(writefp) != 0)
        {
            saveSuccessful = 0;
        }

        if (saveSuccessful)
        {
            printf("\nCustomer order saved successfully!\n");
            printf("Customer Order ID: %d\n", customerOrderID);
        }
        else
        {
            printf("\nERROR: There was a problem saving the customer order!\n");
        }
    }

    printf("\n==================== CUSTOMER BILL ====================\n");
    printf("Customer: %s\n", customerName);
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < successfulItems; i++)
    {
        float itemSubtotal = orderUnitPrice[i] * orderQty[i];
        printf("%-55s\n", orderNames[i]);
        printf("  Quantity: %d  |  Price per item (after discount): Rs. %.2f\n", orderQty[i], orderUnitPrice[i]);
        printf("  Line total: Rs. %.2f\n", itemSubtotal);
        printf("-------------------------------------------------------\n");
        if (totalCustomerOrders < MAX_ORDERS)
        {
            customerOrders[totalCustomerOrders].orderID = customerOrderID;
            strcpy(customerOrders[totalCustomerOrders].customerName, customerName);
            strcpy(customerOrders[totalCustomerOrders].itemName, orderNames[i]);
            customerOrders[totalCustomerOrders].quantity = orderQty[i];
            customerOrders[totalCustomerOrders].itemPrice = orderUnitPrice[i];
            customerOrders[totalCustomerOrders].totalBill = itemSubtotal;
            strcpy(customerOrders[totalCustomerOrders].status, "Pending");
            totalCustomerOrders++;
        }
        else
            printf("Warning: cancellation record limit reached for this line.\n");
    }
    printf("Original total:       Rs. %.2f\n", originalTotal);
    printf("Total discount:      -Rs. %.2f\n", totalDiscount);
    printf("=======================================================\n");
    printf("AMOUNT TO PAY:        Rs. %.2f\n", grandTotal);
    printf("=======================================================\n");
    printf("Thank you for shopping!\n");
}

void cancelCustomerOrder(void)
{
    char customer[30];
    char item[200];
    char confirm;

    int found = 0;
    int i;

    printf("\n========== ORDER CANCELLATION ==========\n");

    printf("\nEnter customer name: ");
    scanf(" %29[^\n]", customer);

    printf("Enter item name: ");
    scanf(" %199[^\n]", item);

    for (i = 0; i < totalCustomerOrders; i++)
    {
        if (strcmp(customerOrders[i].customerName, customer) == 0 &&
            strcmp(customerOrders[i].itemName, item) == 0 &&
            strcmp(customerOrders[i].status, "Pending") == 0)
        {
            found = 1;

            printf("\nOrder Found!");
            printf("\nCustomer Name: %s", customerOrders[i].customerName);
            printf("\nItem Name: %s", customerOrders[i].itemName);
            printf("\nQuantity: %d", customerOrders[i].quantity);
            printf("\nTotal Bill: Rs. %.2f", customerOrders[i].totalBill);

            printf("\n\nDo you want to cancel this order? (Y/N): ");
            scanf(" %c", &confirm);

            if (confirm == 'Y' || confirm == 'y')
            {

                FILE *fp = fopen("orders.txt", "r");
                FILE *temp = fopen("temp.txt", "w");

                struct Order o;
                int fileFound = 0;

                if (fp == NULL || temp == NULL)
                {
                    printf("\nUnable to open order file!");

                    if (fp != NULL)
                        fclose(fp);

                    if (temp != NULL)
                        fclose(temp);

                    return;
                }

                while (fscanf(fp, "%d|%29[^|]|%199[^|]|%d|%f|%f|%19[^\n]",
                              &o.orderID, o.customerName, o.itemName,
                              &o.quantity, &o.itemPrice, &o.totalBill,
                              o.status) == 7)
                {
                    if (o.orderID == customerOrders[i].orderID &&
                        strcmp(o.itemName, customerOrders[i].itemName) == 0 &&
                        strcmp(o.status, "Pending") == 0)
                    {
                        strcpy(o.status, "Cancelled");
                        fileFound = 1;
                    }

                    fprintf(temp, "%d|%s|%s|%d|%.2f|%.2f|%s\n",
                            o.orderID, o.customerName, o.itemName,
                            o.quantity, o.itemPrice, o.totalBill, o.status);
                }

                fclose(fp);
                fclose(temp);

                if (!fileFound)
                {
                    remove("temp.txt");
                    printf("\nOrder not found in saved records!");
                    return;
                }

                if (remove("orders.txt") != 0 ||
                    rename("temp.txt", "orders.txt") != 0)
                {
                    printf("\nCould not update order file!");
                    return;
                }

                int itemIndex = findOrderProductIndex(customerOrders[i].itemName);
                if (itemIndex >= 0)
                {
                    restoreItemStock(itemIndex, customerOrders[i].itemName, customerOrders[i].quantity);
                    quantitySold[itemIndex] -= customerOrders[i].quantity;
                    totalSales[itemIndex] -= customerOrders[i].totalBill;
                    if (quantitySold[itemIndex] < 0)
                        quantitySold[itemIndex] = 0;
                    if (totalSales[itemIndex] < 0.0f)
                        totalSales[itemIndex] = 0.0f;
                }
                strcpy(customerOrders[i].status, "Cancelled");
                printf("\nOrder cancelled successfully!");
            }
            else
            {
                printf("\nOrder cancellation rejected.");
            }

            break;
        }
    }

    if (found == 0)
    {
        printf("\nNo pending order found!");
    }
}

void customerMenu(void)
{
    int choice;
    float productData[ITEMS][2];

    do
    {
        printf("\n========== CUSTOMER CONSOLE ==========\n");
        printf("1. View Available Discounts\n");
        printf("2. Purchase Items (Checkout & Generate Bill)\n");
        printf("3. Cancel Order\n");
        printf("4. Exit Customer Console\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            clearInputBuffer();
            continue;
        }

        switch (choice)
        {
        case 1:
            viewDiscounts();
            break;

        case 2:
            for (int i = 0; i < ITEMS; i++)
            {
                productData[i][0] = price[i];
                productData[i][1] = discount[i];
            }
            purchaseItems((const char **)itemNames, productData, currentStock, ITEMS);
            break;

        case 3:
            cancelCustomerOrder();
            break;

        case 4:
            printf("Returning to main selection...\n");
            break;

        default:
            printf("Invalid choice! Select 1-4.\n");
        }

    } while (choice != 4);
}

int main(void)
{
    int role;
    currentStock[2] = customCakeStockTotal();

    do
    {
        printf("\n============================================\n");
        printf("          BAKERY MANAGEMENT SYSTEM          \n");
        printf("============================================\n");
        printf("1. Customer Console\n");
        printf("2. Admin Console\n");
        printf("3. Exit\n");
        printf("Are you a customer or admin? ");

        if (scanf("%d", &role) != 1)
        {
            printf("Invalid input! Enter a number.\n");
            clearInputBuffer();
            continue;
        }

        switch (role)
        {
        case 1:
            customerMenu();
            break;

        case 2:
            if (adminLogin() == 1)
            {
                printf("\nWelcome to Bakery Admin Console!\n");
                adminMenu();
            }
            else
            {
                printf("\nAccess denied.\n");
            }
            break;

        case 3:
            printf("Thank you for using Bakery Management System.\n");
            break;

        default:
            printf("Invalid choice! Select 1-3.\n");
        }

    } while (role != 3);

    return 0;
}
