#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NAME_SIZE 100

struct Product {
    int productID;
    char productName[NAME_SIZE];
    float price;
    int quantity;
};

void addNewProduct(struct Product **products, int *count);
void viewAllProducts(struct Product *products, int count);
void updateQuantity(struct Product *products, int count);
void searchByID(struct Product *products, int count);
void searchByName(struct Product *products, int count);
void searchByPriceRange(struct Product *products, int count);
void deleteByID(struct Product **products, int *count);
void selectChoice(struct Product **myProducts, int *totalNumberOfProducts, int choice);

void selectChoice(struct Product **myProducts, int *totalNumberOfProducts, int choice) {
    switch (choice) {
        case 1: addNewProduct(myProducts, totalNumberOfProducts); break;
        case 2: viewAllProducts(*myProducts, *totalNumberOfProducts); break;
        case 3: updateQuantity(*myProducts, *totalNumberOfProducts); break;
        case 4: searchByID(*myProducts, *totalNumberOfProducts); break;
        case 5: searchByName(*myProducts, *totalNumberOfProducts); break;
        case 6: searchByPriceRange(*myProducts, *totalNumberOfProducts); break;
        case 7: deleteByID(myProducts, totalNumberOfProducts); break;
        case 8:
            free(*myProducts);
            printf("Memory freed. Exiting program.\n");
            exit(0);
        default:
            printf("Invalid choice! Try again.\n");
            while (getchar() != '\n'); 
    }
}

void addNewProduct(struct Product **products, int *count) {
    struct Product *temp = realloc(*products, (*count + 1) * sizeof(struct Product));
    if (!temp) {
        printf("Memory allocation failed!\n");
        return;
    }
    *products = temp;
    struct Product newProduct;

    while (1) {  
        printf("Enter Product Name (1-50 char): ");
        if(!fgets(newProduct.productName, NAME_SIZE, stdin)){
            printf("Input error. Try again.\n");
            continue;
        }
        newProduct.productName[strcspn(newProduct.productName, "\n")] = '\0';
        int length = strlen(newProduct.productName);
        int hasLetter = 0;
        for (int i = 0; newProduct.productName[i] != '\0'; i++) {
            if (isalpha((unsigned char)newProduct.productName[i])) {
                hasLetter = 1;
                break;
            }
        }    
        if (!hasLetter) {
            printf("Invalid name! Product name must contain at least one letter.\n");
            continue;;
        }
        if (length < 1 || length > 50) {
            printf("Invalid! Name length must be between 1 and 50 characters.\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Enter Product ID (1-10000): ");
        if (scanf("%d", &newProduct.productID) != 1) {
            printf("Invalid input! Please enter digits only.\n");
            while (getchar() != '\n'); 
            continue;
        }
        if (newProduct.productID < 1 || newProduct.productID > 10000) {
            printf("Product ID must be between 1 and 10000.\n");
            continue;
        }
        while (getchar() != '\n'); 
        break;
    }

    while (1) {
        printf("Enter Product Price (1-100000): ");
        if(scanf("%f", &newProduct.price) != 1) {
            printf("Invalid input! Please enter digits only.\n");
            while (getchar() != '\n');
            continue;
        }
        if (newProduct.price < 1 || newProduct.price > 100000) {
            printf("Product price must be between 1 and 100000.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); 
        break;
    }
    while (1) {
        printf("Enter Product Quantity (0-1000000): ");
        if (scanf("%d", &newProduct.quantity) != 1) {
            printf("Invalid input! Enter a whole number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (newProduct.quantity < 0 || newProduct.quantity > 1000000) {
            printf("Quantity must be between 0 and 1000000.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        break;
    }

    (*products)[*count] = newProduct;
    (*count)++;
    printf("Product added successfully!\n");
}

void viewAllProducts(struct Product *products, int count) {
    if (count == 0) {
        printf("No products available.\n");
        return;
    }
    printf("\n=== Product List ===\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               products[i].productID, products[i].productName,
               products[i].price, products[i].quantity);
    }
}

void updateQuantity(struct Product *products, int count) {
    if (count == 0) {
        printf("No products to update.\n");
        return;
    }
    int id, newQty;
    printf("Enter Product ID to update: ");
    scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (products[i].productID == id) {
            printf("Enter new quantity: ");
            scanf("%d", &newQty);
            if (newQty < 0) {
                printf("Invalid quantity!\n");
                return;
            }
            products[i].quantity = newQty;
            printf("Quantity updated!\n");
            return;
        }
    }
    printf("Product not found!\n");
}

void searchByID(struct Product *products, int count) {
    if (count == 0) {
        printf("No products available.\n");
        return;
    }
    int id;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (products[i].productID == id) {
            printf("Found: %s | Price: %.2f | Quantity: %d\n",
                   products[i].productName, products[i].price, products[i].quantity);
            return;
        }
    }
    printf("Product not found!\n");
}

void searchByName(struct Product *products, int count) {
    if (count == 0) {
        printf("No products available.\n");
        return;
    }
    char name[NAME_SIZE];
    printf("Enter product name (partial allowed): ");
    while (getchar() != '\n' && !feof(stdin));
    fgets(name, NAME_SIZE, stdin);
    name[strcspn(name, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(products[i].productName, name)) {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].productID, products[i].productName,
                   products[i].price, products[i].quantity);
            found = 1;
        }
    }
    if (!found) printf("No products match the name.\n");
}

void searchByPriceRange(struct Product *products, int count) {
    if (count == 0) {
        printf("No products available.\n");
        return;
    }
    float low, high;
    printf("Enter minimum and maximum price: ");
    scanf("%f %f", &low, &high);
    if (low > high) {
        printf("Invalid range.\n");
        return;
    }
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (products[i].price >= low && products[i].price <= high) {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].productID, products[i].productName,
                   products[i].price, products[i].quantity);
            found = 1;
        }
    }
    if (!found) printf("No products in that price range.\n");
}

void deleteByID(struct Product **products, int *count) {
    if (*count == 0) {
        printf("No products to delete.\n");
        return;
    }
    int id;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);
    int index = -1;
    for (int i = 0; i < *count; i++) {
        if ((*products)[i].productID == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Product not found!\n");
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        (*products)[i] = (*products)[i + 1];
    }

    struct Product *temp = realloc(*products, (*count - 1) * sizeof(struct Product));
    if (temp != NULL || (*count - 1) == 0)
        *products = temp;
    else
    printf("Warning: Memory reallocation failed, but product was deleted.\n");

    (*count)--;
    printf("Product deleted successfully!\n");
}

int main() {
    int totalNumberOfProducts;
    char input[50];

    while (1) {        
        printf("Enter total number of products you want to add initially: ");
        if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error! Try again.\n");
        continue;
    }
    input[strcspn(input, "\n")] = '\0';
    if (sscanf(input, "%d", &totalNumberOfProducts) != 1) {
        printf("Invalid input! Please enter a positive number.\n");
        continue;
    }
    if (totalNumberOfProducts < 1 || totalNumberOfProducts > 100) {
        printf("Number must be between 1 and 100.\n");
        continue;
    }
    break; 
    }

    struct Product *myProducts = calloc(totalNumberOfProducts, sizeof(struct Product));
    if (myProducts == NULL) {
    printf("Memory allocation failed! Exiting program.\n");
    return 1;
    }
    int count = 0;
        for (int i = 0; i < totalNumberOfProducts; i++) {
            printf("\nEnter details for product %d ::\n", i + 1);
            addNewProduct(&myProducts, &count); 
        }
        int choice;
        while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Add New Product\n2. View All Products\n3. Update Quantity\n");
        printf("4. Search Product by ID\n5. Search Products by Name\n");
        printf("6. Search Products by Price Range\n7. Delete Product by ID\n8. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Enter a number between 1–8.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        selectChoice(&myProducts, &count, choice);
    }
}
