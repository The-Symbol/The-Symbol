#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 10//limits
#define MAX_PURCHASES 100

typedef struct {
    int id;
    char name[50];
    float price;
    int stock;
} Product;

typedef struct {
    int productId;
    char productName[50];
    float price;
    int quantity;
} Purchase;

typedef struct {
    char name[50];
    char email[50];
    char address[100];
    char phoneNumber[10];
} Customer;

// Function prototypes
void displayProducts(Product products[], int size);
void purchaseProduct(Product products[], int size, Purchase purchases[], int *purchaseIndex, Customer *customer);
void displayCategoryProducts(int category, Purchase purchases[], int *purchaseIndex, Customer *customer);
void displayPurchaseHistory(Purchase purchases[], int purchaseIndex);
void getCustomerDetails(Customer *customer);
void savePurchaseHistoryToFile(Purchase purchases[], int purchaseIndex, Customer *customer);

void displayProducts(Product products[], int size) {
    int i;
    printf("\t Available Products:\n");
    for (i = 0; i < size; i++) {
        printf("ID: %d, : %s, Price: %.2f, Stock: %d\n", products[i].id, products[i].name, products[i].price, products[i].stock);
    }
}

void purchaseProduct(Product products[], int size, Purchase purchases[], int *purchaseIndex, Customer *customer) {
    int productId, quantity;
    printf("\t\nEnter Product ID to purchase: ");
    scanf("%d", &productId);

    if (productId < 1 || productId > size) {
        printf("Invalid Product ID!\n");
        return;
    }

    printf("\tEnter quantity to purchase: ");
    scanf("%d", &quantity);

    if (quantity <= 0) {
        printf("Quantity must be greater than 0!\n");
        return;
    }

    Product *product = &products[productId - 1];// This line retrieves the product from the array based on the entered product ID .

    if (quantity > product->stock) {
        printf("Insufficient stock! Only %d available.\n", product->stock);
    } else {
        product->stock -= quantity;
        printf("\t You purchased %d of %s for %.2f each. Total: %.2f\n", quantity, product->name, product->price, product->price * quantity);

        // Add's purchase to purchase history
        purchases[*purchaseIndex].productId = product->id;//*purchaseIndex - determine the current index in the purchases array where the purchase details will be stored
        strcpy(purchases[*purchaseIndex].productName, product->name);//Strcpy -copy string from the source
        purchases[*purchaseIndex].price = product->price;
        purchases[*purchaseIndex].quantity = quantity;
        (*purchaseIndex)++;

        // Get's customer details
        getCustomerDetails(customer);

        // Display's purchase summary
        printf("\nPurchase Summary:\n");
        printf("Customer Name: %s\n", customer->name);
        printf("Email ID: %s\n", customer->email);
        printf("Address: %s\n", customer->address);
        printf("Phone Number: %s\n", customer->phoneNumber);
        printf("Product Name: %s\n", purchases[*purchaseIndex - 1].productName);
        printf("Quantity: %d\n", purchases[*purchaseIndex - 1].quantity);
        printf("Total: %.2f\n", purchases[*purchaseIndex - 1].price * purchases[*purchaseIndex - 1].quantity);

        // Save purchase history to file
        savePurchaseHistoryToFile(purchases, *purchaseIndex, customer);
    }
}

void displayCategoryProducts(int category, Purchase purchases[], int *purchaseIndex, Customer *customer) {
    Product laptops[] = {
        {1, "ASUS TUF A15", 82990, 10},
        {2, "Lenovo Legion 5", 67000, 8},
        {3, "DELL Alienware M15", 142990, 5},
        {4, "Apple MacBook Air", 99999, 10},
    };

    Product smartphones[] = {
        {1, "Samsung Galaxy S24", 98199, 20},
        {2, "vivo X100s Pro 5G", 89999, 15},
        {3, "Redmi Note 13 Pro", 20499, 12},
        {4, "Apple iPhone 16", 56990, 10}
    };

        Product headphones[] = {
        {1, "Skullcandy Hesh ANC(Black)", 6999, 15},
        {2, "BoAt Nirvana 751ANC", 6117, 20},
        {3, "JBL Tune 770NC Wireless(White)", 5999, 18},
        {4, "Sony WH-CH520", 4488, 10}
    };

    Product smartwatches[] = {
        {1, "Apple Watch SE 2nd Gen", 24000, 5},
        {2, "Huawei Watch GT 5", 16999, 10},
        {3, "Samsung Galaxy Watch 7", 34799, 8},
        {4, "boAt Lunar Discovery", 1399, 40}
    };

    Product tablets[] = {
        {1, "Lenovo Fold", 6499, 8},
        {2, "Honor Pad X8", 8999, 6},
        {3, "Samsung Galaxy Tab S9 FE", 34999, 8},
        {4, "OnePlus Pad Go", 23499, 40}
    };

    switch (category) {
        case 1:
            displayProducts(laptops, sizeof(laptops) / sizeof(laptops[0]));
            purchaseProduct(laptops, sizeof(laptops) / sizeof(laptops[0]), purchases, purchaseIndex, customer);
            break;
        case 2:
            displayProducts(smartphones, sizeof(smartphones) / sizeof(smartphones[0]));
            purchaseProduct(smartphones, sizeof(smartphones) / sizeof(smartphones[0]), purchases, purchaseIndex, customer);
            break;
        case 3:
            displayProducts(headphones, sizeof(headphones) / sizeof(headphones[0]));
            purchaseProduct(headphones, sizeof(headphones) / sizeof(headphones[0]), purchases, purchaseIndex, customer);
            break;
        case 4:
            displayProducts(smartwatches, sizeof(smartwatches) / sizeof(smartwatches[0]));
            purchaseProduct(smartwatches, sizeof(smartwatches) / sizeof(smartwatches[0]), purchases, purchaseIndex, customer);
            break;
        case 5:
            displayProducts(tablets, sizeof(tablets) / sizeof(tablets[0]));
            purchaseProduct(tablets, sizeof(tablets) / sizeof(tablets[0]), purchases, purchaseIndex, customer);
            break;
        default:
            printf("Invalid category!\n");
    }
}

void displayPurchaseHistory(Purchase purchases[], int purchaseIndex) {
    int i;
    printf("\t Purchase History:\n");
    for (i = 0; i < purchaseIndex; i++) {
        printf("Product ID: %d, Product Name: %s, Price: %.2f, Quantity: %d\n", purchases[i].productId, purchases[i].productName, purchases[i].price, purchases[i].quantity);
    }
}

void getCustomerDetails(Customer *customer) {
    printf("\t Enter your name: ");
    getchar(); // reads a single character from the standard input (stdin)
    fgets(customer->name, sizeof(customer->name), stdin);// fgets reads and stores text in "name" in fieldof "customer name"
    customer->name[strcspn(customer->name, "\n")] = 0;

    printf("\t Enter your email ID: ");
    fgets(customer->email, sizeof(customer->email), stdin);
    customer->email[strcspn(customer->email, "\n")] = 0;

    printf("\t Enter your address: ");
    fgets(customer->address, sizeof(customer->address), stdin);
    customer->address[strcspn(customer->address, "\n")] = 0;

    printf("\t Enter your phone number: ");
    fgets(customer->phoneNumber, sizeof(customer->phoneNumber), stdin);
    customer->phoneNumber[strcspn(customer->phoneNumber, "\n")] = 0;
}

void savePurchaseHistoryToFile(Purchase purchases[], int purchaseIndex, Customer *customer) {
    int i;
    FILE *file = fopen("purchase_history.txt", "a");//referenced to the opened file
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    fprintf(file, "Customer Name: %s\n", customer->name);//fprint - write the details to customer struct(in text)
    fprintf(file, "Email ID: %s\n", customer->email);
    fprintf(file, "Address: %s\n", customer->address);
    fprintf(file, "Phone Number: %s\n", customer->phoneNumber);
        fprintf(file, "Purchase History:\n");
    for ( i = 0; i < purchaseIndex; i++) {
        fprintf(file, "Product ID: %d, Product Name: %s, Price: %.2f, Quantity: %d\n",
                purchases[i].productId, purchases[i].productName, purchases[i].price, purchases[i].quantity);
    }
    fprintf(file, "----------------------------------------\n");
    fclose(file);
}

int main() {
    int choice;
    Purchase purchases[MAX_PURCHASES];
    int purchaseIndex = 0;
    Customer customer;

    while (1) {
        printf("\n**********************************************Welcome to E-Cart.com*****************************************************\n\n");
        printf("\n\t\t You can buy items specified here \n\n");
        printf("1. Laptops\n");
        printf("2. Smartphones\n");
        printf("3. Headphones\n");
        printf("4. Smartwatches\n");
        printf("5. Tablets\n");
        printf("6. View Purchase History\n");
        printf("7. Exit\n");
        printf("\t Enter your choice:");


        if (scanf("%d", &choice) != 1) {// returns 1 when integer
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Discard invalid input
            continue;
        }

        switch (choice) {
            case 1:
                displayCategoryProducts(1, purchases, &purchaseIndex, &customer); // Laptops
                break;
            case 2:
                displayCategoryProducts(2, purchases, &purchaseIndex, &customer); // Smartphones
                break;
            case 3:
                displayCategoryProducts(3, purchases, &purchaseIndex, &customer); // Headphones
                break;
            case 4:
                displayCategoryProducts(4, purchases, &purchaseIndex, &customer); // Smartwatches
                break;
            case 5:
                displayCategoryProducts(5, purchases, &purchaseIndex, &customer); // Tablets
                break;
            case 6:
                displayPurchaseHistory(purchases, purchaseIndex);
                break;
            case 7:
                printf("Exiting the program. Thank you for visiting!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
