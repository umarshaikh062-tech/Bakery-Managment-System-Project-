# Bakery Management System

A menu-driven **Bakery Management System written in C**. The program
provides separate Customer and Admin consoles for purchasing bakery
products, managing stock, applying discounts, tracking expiry dates, and
handling customer orders.

## Features

### Customer Console

-   View currently available product discounts.
-   View the product list, prices, and available stock during checkout.
-   Purchase one or more products in a single checkout.
-   Generate a bill showing:
    -   Customer name
    -   Product names and quantities
    -   Price per item after discount
    -   Line totals
    -   Original total, total discount, and final amount payable
-   Order custom cakes by selecting:
    -   Flavor: Red Velvet, Vanilla, or Chocolate
    -   Topping: Choco Chips, Cherries, Nuts, or No Topping
    -   Size: Small, Medium, or Large
    -   A custom cake message
-   Request cancellation of a pending customer order.

### Admin Console

Admin access is protected by a username and PIN. The program allows up
to three login attempts.

  -----------------------------------------------------------------------
  Section                             Functions
  ----------------------------------- -----------------------------------
  Stock Management                    Display current stock, add stock,
                                      record a sale, and check low-stock
                                      items

  Expiry Management                   View expiry dates, check expired
                                      products, and update expiry dates

  Sales Report                        View initial stock, remaining
                                      stock, quantity sold, revenue, and
                                      grand total sales

  Discount Management                 Set a discount percentage for a
                                      product

  Search and Sort                     Search products using linear search
                                      or binary search

  Order Management                    Add orders, view all orders, search
                                      by customer name, update order
                                      status, and cancel pending orders

  Product Menu                        Display all products and their
                                      prices
  -----------------------------------------------------------------------

### Product List

The system contains ten products:

1.  Plain Bread (Large Loaf)
2.  Milk Rusk (Pack)
3.  Custom Cake (Made to Order)
4.  Chicken Patty
5.  Cream Roll
6.  Butter Cookies (250 g)
7.  Cheese Pizza Slice
8.  Donut (Chocolate Glazed)
9.  Cheesecake (1 lb)
10. Swiss Roll

## Requirements

-   A C compiler that supports **C99 or later**
-   Examples: GCC or MinGW
-   Dev-C++ or another C IDE (optional)

The program uses standard C libraries:

``` c
#include <stdio.h>
#include <string.h>
```

## How to Compile and Run

### Using GCC (Windows, Linux, or macOS)

Save the source code as `bakery.c`, then open a terminal in the folder
containing the file.

Compile:

``` bash
gcc -std=c99 bakery.c -o bakery
```

Run on Windows:

``` bash
bakery.exe
```

Run on Linux or macOS:

``` bash
./bakery
```

### Using Dev-C++

1.  Open Dev-C++.
2.  Select **File → New → Source File**.
3.  Paste the C program.
4.  Save it as `bakery.c`.
5.  Select **Execute → Compile & Run** (or press `F11`, depending on the
    Dev-C++ version).
6.  Use the on-screen menu to select Customer Console, Admin Console, or
    Exit.

## Admin Login

The credentials currently defined in the source code are:

  Field      Value
  ---------- ---------------
  Username   `bakeryadmin`
  PIN        `1122`

**Note:** These are demonstration credentials stored directly in the
source code. Change them before using the program in any real
environment.

## Data Storage

The program uses a text file named `orders.txt` to store order records.
Each order line follows this pipe-separated format:

``` text
orderID|customerName|itemName|quantity|itemPrice|totalBill|status
```

Example:

``` text
1|Ali Khan|Chicken Patty|2|150.00|300.00|Pending
```

Order statuses used by the program include:

-   `Pending`
-   `Completed`
-   `Cancelled`

The program also uses `temp.txt` temporarily when updating or cancelling
an order. Keep `orders.txt` in the program's working directory so the
program can read and update saved orders.

## Important Notes and Current Limitations

-   **Orders are saved to `orders.txt`**, so order records can remain
    available after the program closes.
-   Product stock, discounts, sales totals, and custom-cake flavor stock
    are currently held in memory. They are initialized again when the
    program starts; changes to them are not automatically saved between
    runs.
-   Customer cancellation uses the customer-order records held in
    memory. Therefore, the customer cancellation option is intended for
    orders created during the current program session.
-   The customer checkout code currently starts its order ID counter at
    `1` rather than finding the highest existing ID in `orders.txt`.
    Repeated customer checkouts across sessions may therefore create
    duplicate order IDs. This should be corrected before relying on
    order IDs as unique identifiers.
-   Expiry checking compares the dates entered by the admin with the
    dates stored in the program. It does not automatically read the
    computer's current date.
-   This is an academic/demo project. It does not include a database,
    multi-user access control, or secure password storage.

## Project Structure

A simple single-file setup can be used:

``` text
Bakery-Management-System/
├── bakery.c
├── README.md
├── orders.txt       # Created when an order is saved
└── temp.txt         # Temporary file used during order updates
```

`orders.txt` and `temp.txt` are created or used in the program's current
working directory. You do not need to create `temp.txt` manually.

## How to Use

1.  Run the program.
2.  Select `1` for Customer Console or `2` for Admin Console.
3.  In the Customer Console, view discounts, purchase products, or
    request cancellation of a pending order.
4.  For the Admin Console, enter the admin username and PIN.
5.  Select the required admin section and follow the prompts.
6.  Select the exit option when finished.




