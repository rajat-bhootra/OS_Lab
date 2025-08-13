unsigned char * uart = (unsigned char *)0x10000000; 

void putchar(char c) {
    *uart = c;
    return;
}

void print(const char * str) {
    while (*str != '\0') {
        putchar(*str);
        str++;
    }
    return;
}

// Function to print an integer
void print_int(int num) {
    char buffer[12];  // Enough to hold "-2147483648" and '\0'
    char *ptr = buffer + sizeof(buffer) - 1;
    int is_negative = 0;

    *ptr = '\0';  // Null-terminate the string

    if (num == 0) {
        putchar('0');
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0) {
        *(--ptr) = '0' + (num % 10);  // Convert last digit to char
        num /= 10;                    // Remove last digit
    }

    if (is_negative) {
        *(--ptr) = '-';  // Add negative sign
    }

    print(ptr);  // Print the resulting string
}

// void kmain(void) {
//     print("Hello world!\r\n");

//     int counter = 0;
//     int check; 

//     int example_number = 0;
//     while (1) {
//         // Read input from the UART
//         for (int i = 0; i < 310000000; i++) {
//             // Busy wait
//         }

//         print("Hello world!\r\n");

//         check = *uart;
//         if (counter == 0) {
//             putchar(*uart);
//             check = *uart;
//             counter = 1;
//         }

//         if (check != *uart) {
//             counter = 0;
//         }

//         // Example usage of print_int
//         print("Printing a number: ");
//         print_int(example_number++);
//         print("\r\n");
//     }

//     return;
// }

void print_ascii_art() {
    print("								                   ,,,, \r\n");
    print("								             ,;) .';;;;',\r\n");
    print("								 ;;,,_,-.-.,;;'_,|I\\;;;/),,_\r\n");
    print("								  `';;/:|:);{ ;;;|| \\;/ /;;;\\__\r\n");
    print("								      L;/-';/ \\;;\\',/;\\/;;;.') \\\r\n");
    print("								      .:`''` - \\;;'.__/;;;/  . _'-._ \r\n");
    print("								    .'/   \\     \\;;;;;;/.'_7:.  '). \\_\r\n");
    print("								  .''/     | '._ );}{;//.'    '-:  '.,L\r\n");
    print("								.'. /       \\  ( |;;;/_/         \\._./;\\   _,\r\n");
    print("								 . /        |\\ ( /;;/_/             ';;;\\,;;_,\r\n");
    print("								. /         )__(/;;/_/                (;;'''''\r\n");
    print("								 /        _;:':;;;;:';-._             );\r\n");
    print("								/        /   \\  `'`   --.'-._         \\/\r\n");
    print("								       .'     '.  ,'         '-,\r\n");
    print("								      /    /   r--,..__       '.\\\r\n");
    print("								    .'    '  .'        '--._     ]\r\n");
    print("								    (     :.(;>        _ .' '- ;/\r\n");
    print("								    |      /:;(    ,_.';(   __.'\r\n");
    print("								     '- -'\"|;:/    (;;;;-'--'\r\n");
    print("								           |;/      ;;(\r\n");
    print("								           ''      /;;|\r\n");
    print("								                   \\;;|\r\n");
    print("								                    \\/\r\n");
}


void kmain(void) {
    // Print initial message
    print_ascii_art();
    char received_char;
    char previous_char = 0; // Track the last printed character

    while (1) {
        received_char = *uart;
        if (received_char != previous_char) {
            putchar(received_char);
            previous_char = received_char;
        }
        // for (int i = 0; i < 310000000; i++) {
        // }
    }
}