
# Bakery Management System

A menu-driven **Bakery Management System developed using the C programming language**. The program provides separate Customer and Admin consoles for purchasing bakery products, managing stock, applying discounts, tracking expiry dates, generating bills, and handling customer orders.

---

## Features

### Customer Console

- View currently available product discounts.
- View the product list, prices, and available stock during checkout.
- Purchase one or more products in a single checkout.
- Generate detailed bills showing:
  - Customer name
  - Product names and quantities
  - Price per item after discount
  - Line totals
  - Original total, total discount, and final amount payable
- Order custom cakes by selecting:
  - **Flavor:** Red Velvet, Vanilla, or Chocolate
  - **Topping:** Choco Chips, Cherries, Nuts, or No Topping
  - **Size:** Small, Medium, or Large
  - A custom cake message
- Request cancellation of pending customer orders.

### Admin Console

Admin access is protected by a username and PIN, with a maximum of three login attempts.

#### Admin Portal Features

| Section | Functions |
|---|---|
| **Stock Management** | Display current stock, add stock, record a sale, and check low-stock items. |
| **Expiry Management** | View expiry dates, check expired products, and update expiry dates. |
| **Sales Report** | View initial stock, remaining stock, quantity sold, revenue, and grand total sales. |
| **Discount Management** | Set a discount percentage for a product. |
| **Search and Sort** | Search products using linear search or binary search. |
| **Order Management** | Add orders, view all orders, search by customer name, update order status, and cancel pending orders. |
| **Product Menu** | Display all products and their prices. |

---

## Product List

The system contains ten bakery products:

1. Plain Bread (Large Loaf)
2. Milk Rusk (Pack)
3. Custom Cake (Made to Order)
4. Chicken Patty
5. Cream Roll
6. Butter Cookies (250 g)
7. Cheese Pizza Slice
8. Donut (Chocolate Glazed)
9. Cheesecake (1 lb)
10. Swiss Roll

---

## Technologies and Concepts Used

The project is developed using the C programming language and incorporates the following programming concepts:

- Functions and modular programming
- Arrays and pointers
- Structures
- Dynamic memory allocation
- File handling
- Conditional statements
- Loops and switch statements
- Linear search and binary search
- Input validation and error handling

---

## Requirements

- A C compiler supporting **C99 or later**
- GCC or MinGW
- Dev-C++ or another C IDE (optional)

The program uses standard C libraries:

```c
#include <stdio.h>
#include <string.h>
```

---

## How to Compile and Run

### Using GCC (Windows, Linux, or macOS)

Save the source code as `bakery.c` and open a terminal in the folder containing the file.

**Compile:**

```bash
gcc -std=c99 bakery.c -o bakery
```

**Run on Windows:**

```bash
bakery.exe
```

**Run on Linux or macOS:**

```bash
./bakery
```

### Using Dev-C++

1. Open Dev-C++.
2. Select **File → New → Source File**.
3. Paste the C program into the editor.
4. Save the file as `bakery.c`.
5. Select **Execute → Compile & Run** (or press `F11`, depending on the Dev-C++ version).
6. Use the on-screen menu to select Customer Console, Admin Console, or Exit.

---

## Data Storage

The program uses text files to manage order records.

### Orders File

A text file named `orders.txt` is used to store order information. Each order record follows this pipe-separated format:

```text
orderID|customerName|itemName|quantity|itemPrice|totalBill|status
```

**Example:**

```text
1|Ali Khan|Chicken Patty|2|150.00|300.00|Pending
```

**Order statuses include:**

- `Pending`
- `Completed`
- `Cancelled`

### Temporary File

The program also uses `temp.txt` temporarily when updating or cancelling an order. Both files are used in the program's working directory.

---

## Project Structure

The project follows a simple single-file structure:

```text
Bakery-Management-System/
│
├── bakery.c
├── README.md
├── orders.txt
└── temp.txt
```

- **bakery.c:** Main C source code containing the system functionality.
- **README.md:** Project documentation.
- **orders.txt:** Stores customer order records.
- **temp.txt:** Temporary file used during order updates.

The order-related files are created or used by the program when required.

---

## How to Use

1. Compile and run the program.
2. Select `1` for Customer Console or `2` for Admin Console.
3. In the Customer Console, view discounts, browse products, purchase items, generate bills, or request order cancellation.
4. To access the Admin Console, enter the required login credentials.
5. Select the desired administrative feature from the menu.
6. Follow the on-screen instructions to complete the selected operation.
7. Select the Exit option when finished.

---

## Project Objectives

- Apply fundamental and intermediate C programming concepts in a practical project.
- Develop a menu-driven application using functions and switch statements.
- Understand file handling and record management.
- Implement searching algorithms.
- Practice dynamic memory allocation and data validation.
- Understand the basic operations involved in managing a bakery.

---

## Conclusion

The **Bakery Management System** provides an organized way to handle common bakery operations through separate customer and administrative interfaces. It combines product purchasing, billing, inventory management, discount handling, expiry tracking, and order management in one application.

This project was developed as part of an academic programming assignment to gain practical experience in C programming, problem-solving, and collaborative software development.

---
