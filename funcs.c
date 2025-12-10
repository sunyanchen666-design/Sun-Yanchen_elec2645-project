#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funcs.h"
#include <ctype.h>

#ifndef M_PI
#define M_PI 3.1415926
#endif

static int is_integer(const char *s)
{
    if (*s == '-' || *s == '+') 
    s++;
    if (*s == '\0') 
    return 0;
    while (*s) {
        if (!isdigit((unsigned char)*s)) 
        return 0;
        s++;
    }
    return 1;
}

static int compare_doubles(const void *a, const void *b)
{
    const double *da = (const double *)a;
    const double *db = (const double *)b;
    if (*da < *db) return -1;
    if (*da > *db) return 1;
    return 0;
}

static void sort_doubles(double *arr, int n)
{
    for (int i = 1; i < n; i++) {
        double key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

static double calculate_median(double *data, int n)
{
    if (n == 0) 
    return 0.0;
    if (n % 2 == 1) {
        return data[n / 2];
    } else {
        return (data[n / 2 - 1] + data[n / 2]) / 2.0;
    }
}

static double calculate_mean(double *data, int n)
{
    if (n == 0) 
    return 0.0;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;
}

static double calculate_variance(double *data, int n)
{
    if (n == 0) 
    return 0.0;
    double mean = calculate_mean(data, n);
    double sum_sq_diff = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_sq_diff += diff * diff;
    }
    return sum_sq_diff / n;
}

static double calculate_std_dev(double *data, int n)
{
    return sqrt(calculate_variance(data, n));
}

void menu_item_1(void) {
    char buf[128]; //Input buffer
    int choice = 0;
    char *endptr = NULL;  //End pointer for strtod
    double angle = 0.0;  //Angle input
    double rad = 0.0;  //Angle in radians
    char unit = 'r';   //Unit: 'd' for degrees, 'r' for radians
    int valid_input = 0;
    int value = 0;

    printf("\n Trigonometric Function Calculator\n");  //Program title

    // loop until a valid function choice is entered 
    for (;;) {
        printf("Choose function:\n");
        printf("  1) sin(x)\n");
        printf("  2) cos(x)\n");
        printf("  3) tan(x)\n");
        printf("Enter choice (1-3): ");
        //Read input
        if (!fgets(buf, sizeof(buf), stdin)) {
            printf("\nInput error.");
            return;  //Return to menu
        }
        buf[strcspn(buf, "\r\n")] = '\0';   //Remove line breaks
        
        if (!is_integer(buf)) {
            printf("Enter an integer!\n");
            valid_input = 0;
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= 3) {
                valid_input = 1;
            } else {
                printf("Invalid menu item!\n");
                valid_input = 0;
            }
        }
        //Exit the loop after achieving success
        if (valid_input) {  
        choice = value;  
        break;
    }
}

    // loop until a valid numeric angle is entered 
    for (;;) {
        printf("Enter angle value: ");   //Prompt for angle
        if (!fgets(buf, sizeof(buf), stdin)) { //Read input
            printf("\nInput error.");
            return;   //Return to menu
        }
        buf[strcspn(buf, "\r\n")] = '\0';   //Remove line breaks
        endptr = NULL;   //Reset endptr to NULL
        angle = strtod(buf, &endptr);  //String to double conversion
        if (endptr != buf) break;  //Valid number entered
        printf("Invalid number. Please enter a numeric value.\n\n");
    }

    // loop until a valid unit (d/r) is entered 
    for (;;) {
        printf("Is the angle in degrees or radians? (d/r): ");  //Prompt for unit
        if (!fgets(buf, sizeof(buf), stdin)) {  //Read input
            printf("\nInput error.");
            return;   //Return to menu
        }
        buf[strcspn(buf, "\r\n")] = '\0';   //Remove newline
        if (buf[0] == 'd' || buf[0] == 'D' || buf[0] == 'r' || buf[0] == 'R') {   //Check valid input
            unit = buf[0];   //Set unit to first character
            break;   //Valid unit entered
        }
        printf("Invalid unit. Please enter 'd' for degrees or 'r' for radians.\n\n");   
    }

    rad = angle;  //Default to radians
    if (unit == 'd' || unit == 'D') {  //Set to degrees
        rad = angle * M_PI / 180.0;  //Convert degrees to radians
    }
    // prepare a simple unit string using if/else (avoid ternary) 
    const char *unit_str;   //Unit string for output
    if (unit == 'd' || unit == 'D') {   
        unit_str = "deg";  
    } else {
        unit_str = "rad";
    }

    double result = 0.0;  //Result variable
    switch (choice) {
        case 1:
            result = sin(rad);
            printf("sin(%lf %s) = %.10lf\n", angle, unit_str, result);  //Sin output
            break;
        case 2:
            result = cos(rad);
            printf("cos(%lf %s) = %.10lf\n", angle, unit_str, result);  //Cos output
            break;
        case 3:
            if (cos(rad)< 1e-15) {
                printf("tan is undefined (cos(angle) is approximately zero).\n");  //Tan undefined output
            } else {
                result = tan(rad);
                printf("tan(%lf %s) = %.10lf\n", angle, unit_str, result);   //Tan output
            }
            break;  //End of switch
        default:
            break;  
    }
}

void menu_item_2(void) {
    double data[100];   //Array to hold input data
    double sorted_data[100];   //Array to hold sorted data
    int count = 0;   //Data count
    char buf[256];   //Input buffer

    printf("\n Statistical Analysis\n");
    printf("Enter numbers separated by spaces. Type 'd' to finish.\n\n");
    
    //Main input loop(Up to 100 numbers)
    while (count < 100) {
        printf("Enter data: ");   //Prompt for input
        if (!fgets(buf, sizeof(buf), stdin)) {   //Read input
            printf("Input error.");
            return;  //Return to menu
        }
        buf[strcspn(buf, "\r\n")] = '\0';   //Remove line breaks
        
        //Check for done signal
        {
            char *p2 = buf;   //Helper pointer
            while (*p2 == ' ' || *p2 == '\t')  //Skip leading whitespace
            p2++;
            if (*p2 == 'd' || *p2 == 'D') {   //Check for 'd' or 'D'
                break;  //Exit input loop
            }
        }

        char *p = buf;  //Pointer to buffer start
        while (*p != '\0' && count < 100) {   //Process until end of line or max count
            while (*p == ' ' || *p == '\t')    //Skip whitespace
            p++;
            if (*p == '\0')   
            break;   

            char *endptr = NULL;  //Reset endptr
            double value = strtod(p, &endptr);   //String to double conversion

            if (endptr == p) {  //Conversion failed
               
                char *q = p;   //Helper pointer to skip invalid token
                while (*q != '\0' && *q != ' ' && *q != '\t')   //Find token end
                 q++;
                if (*q == '\0')  //If reached end of line
                break;
                p = q;   //Move pointer to next token
                continue;
            }
            if (*endptr != '\0' && *endptr != ' ' && *endptr != '\t') {
                //Check if invalid characters follow the number
                if ((*endptr >= 'a' && *endptr <= 'z') || 
                    (*endptr >= 'A' && *endptr <= 'Z')) {
                    //Invalid token detected, skip it
                    char *q = p;
                    while (*q != '\0' && *q != ' ' && *q != '\t')   //Find token end
                        q++;
                    if (*q == '\0')
                        break;
                    p = q;
                    continue;
                }
            }
            data[count++] = value;   //Store valid number, increment count  
            p = endptr;
        }
    }
    
    //Check if any data was entered
    if (count == 0) {
        printf("No data entered.\n");
        return;   //Return to menu
    }
    //Copy data to sorted_data array
    for (int i = 0; i < count; i++) {
        sorted_data[i] = data[i];
    }

    sort_doubles(sorted_data, count);  //Sort the data

    double mean = calculate_mean(data, count);   //Calculate mean
    double median = calculate_median(sorted_data, count);   //Calculate median
    double variance = calculate_variance(data, count);   //Calculate variance
    double std_dev = calculate_std_dev(data, count);    //Calculate standard deviation

    printf("\n    Results    \n");
    printf("Count: %d\n", count);
    printf("Mean: %.6lf\n", mean);
    printf("Median: %.6lf\n", median);
    printf("Variance: %.6lf\n", variance);
    printf("Standard deviation: %.6lf\n", std_dev);
}

void menu_item_3(void) {
    char buf[128];   //Input buffer
    double R, C, V, tau, t;   //Resistance, Capacitance, Voltage, Time constant, Time
    int steps;   //Time steps

    printf("\n RC Circuit Transient Response Simulator\n");
    printf("===========================================\n\n");

    printf("Enter Resistance (R) in Ohms: ");   //Prompt for Resistance
    if (!fgets(buf, sizeof(buf), stdin)) {   //Read input to buffer
        printf("Input error.");
        return;   //Return to menu
    }
    R = strtod(buf, NULL);   //String to double conversion
    if (R <= 0) {
        printf("Resistance must be positive.\n");
        return;
    }

    printf("Enter Capacitance (C) in Farads: ");   //Prompt for Capacitance
    if (!fgets(buf, sizeof(buf), stdin)) {   //Read input to buffer
        printf("Input error.");
        return;
    }
    C = strtod(buf, NULL);   //String to double conversion
    if (C <= 0) {   //Validate positive capacitance
        printf("Capacitance must be positive.\n");
        return;
    }

    printf("Enter Voltage (V) in Volts: ");   //Prompt for Voltage
    if (!fgets(buf, sizeof(buf), stdin)) {    //Read input to buffer
        printf("Input error.");
        return;
    }
    V = strtod(buf, NULL);   //String to double conversion
    if (V < 0) {   //Validate non-negative voltage
        printf("Voltage cannot be negative.\n");
        return;
    }

    printf("Number of time steps to display: ");   //Prompt for time steps
    if (!fgets(buf, sizeof(buf), stdin)) {    //Read input to buffer
        printf("Input error.");
        return;
    }
    steps = (int)strtol(buf, NULL, 10);   //String to integer conversion
    if (steps <= 0 || steps > 100) {   //Check valid range
        printf("Steps must be between 1 and 100.\n");
        return;
    }

    tau = R * C;   //Calculate time constant
    printf("\nTime constant (tau = R*C): %.6lf seconds\n\n", tau);

    printf("    Capacitor Charging (0V -> %.2fV)    \n", V);
    printf("Time(s)\t\tVc(V)\t\tI(A)\n");
    printf("--------\t\t--------\t\t--------\n");

    for (int i = 0; i <= steps; i++) {   //Loop through time steps
        t = i * tau / (steps / 5.0);   //Calculate current time
        double Vc = V * (1.0 - exp(-t / tau));   //Calculate capacitor voltage with charging formula
        double I = (V / R) * exp(-t / tau);     //Calculate current with charging formula
        printf("%.6lf\t\t%.6lf\t\t%.6lf\n", t, Vc, I);
    }

    printf("\n    Capacitor Discharging (%.2fV -> 0V)    \n", V);
    printf("Time(s)\t\tVc(V)\t\tI(A)\n");
    printf("--------\t\t--------\t\t--------\n");

    for (int i = 0; i <= steps; i++) {    //Loop through time steps
        t = i * tau / (steps / 5.0);     //Calculate current time
        double Vc = V * exp(-t / tau);    //Calculate capacitor voltage with discharging formula
        double I = -(V / R) * exp(-t / tau);    //Calculate current with discharging formula
        printf("%.6lf\t\t%.6lf\t\t%.6lf\n", t, Vc, I);
    }

    printf("\nSimulation complete.\n");
}

void menu_item_4(void) {
    char buf[128];   //Input buffer
    double resistors[50];   //Array to hold resistor values
    int count = 0;   //Resistor count
    char type;    //Circuit type: 'S' for series, 'P' for parallel
    double R_total, V, I;     //Total Resistance, Voltage, Current

    printf("\n>> Basic Circuit Analyser\n");
    printf("==============================\n\n");

    printf("Series (S) or Parallel (P)? Enter S or P: ");   //Prompt for circuit type
    if (!fgets(buf, sizeof(buf), stdin)) {    //Read input
        printf("Input error.");
        return;   
    }
    buf[strcspn(buf, "\r\n")] = '\0';    //Remove newline and carriage return

    type = buf[0];   //Get first character
    if (type != 'S' && type != 's' && type != 'P' && type != 'p') {     //Validate input is valid
        printf("Invalid choice. Must be S or P.\n");
        return;
    }

    printf("Enter resistor values (in Ohms). Type 'd' to finish.\n");

    while (count < 50) {    //Max 50 resistors
        printf("R%d: ", count + 1);   //Prompt for resistor value
        if (!fgets(buf, sizeof(buf), stdin)) {
            printf("Input error.");
            return;
        }
        buf[strcspn(buf, "\r\n")] = '\0';    //Remove newline and carriage return

        {
            char *p2 = buf;    //Helper pointer to check for done signal
            while (*p2 == ' ' || *p2 == '\t')     //Skip leading whitespace
            p2++;  
            if (*p2 == 'd' || *p2 == 'D') {   //Check for 'd' or 'D'
                break;
            }
        }

        char *endptr = NULL;    //Clear endptr
        double R = strtod(buf, &endptr);  //String to double conversion
        if (endptr == buf || R <= 0) {    //Validate positive resistor value
            printf("Invalid resistor value. Must be positive.\n");
            continue;
        }

        resistors[count++] = R;    //Store valid resistor value and increment count
    }

    if (count == 0) {   //Check if any resistors were entered
        printf("No resistors entered.\n");
        return;
    }

    if (type == 'S' || type == 's') {    //Series calculation
        R_total = 0.0;   //Initialize total resistance
        for (int i = 0; i < count; i++) {
            R_total += resistors[i];    //Sum resistors for series
        }
        printf("\n    Series Configuration    \n");
        printf("R_total = ");
        for (int i = 0; i < count; i++) {
            printf("%.4lf", resistors[i]);   //Print each resistor
            if (i < count - 1) printf(" + ");   //Plus sign between resistors
        }
        printf(" = %.6lf Ohms\n", R_total);    //Print total resistance
    } else {   //Parallel calculation
        R_total = 0.0;     //Initialize total resistance
        for (int i = 0; i < count; i++) {
            R_total += 1.0 / resistors[i];    //Sum resistors for parallel
        }
        R_total = 1.0 / R_total;   //Calculate total resistance
        printf("\n    Parallel Configuration    \n");
        printf("1/R_total = ");
        for (int i = 0; i < count; i++) {
            printf("1/%.4lf", resistors[i]);   //Print each resistor in reciprocal form
            if (i < count - 1) printf(" + ");   //Plus sign between resistors
        }
        printf("\nR_total = %.6lf Ohms\n", R_total);   //Print total resistance
    }

    printf("\nEnter Voltage (V) in Volts: ");
    if (!fgets(buf, sizeof(buf), stdin)) {     //Read input
        printf("Input error.");
        return;
    }  
     buf[strcspn(buf, "\r\n")] = '\0';    //Remove newline and carriage return

     char *endptr = NULL;    //Clear endptr
    char *p = buf;      //Pointer to buffer start

    V = strtod(p, &endptr);   //String to double conversion
    if (endptr == p) {    //Check for valid number
            printf("Invalid input. Please enter a valid number for voltage.\n");   
            return;
        }
       
        while (*endptr != '\0') {   //Skip whitespace
            if (*endptr != ' ' && *endptr != '\t') {    //Check for non-numeric characters
                printf("Invalid input. '%s' contains non-numeric characters.\n", buf);
                return;
            }
            endptr++;
        }

    if (V < 0) {     //Validate non-negative voltage
        printf("Voltage cannot be negative.\n");
        return;
    }

    I = V / R_total;     //Calculate total current using Ohm's Law

    printf("\n    Results    \n");
    printf("Total Resistance: %.6lf Ohms\n", R_total);
    printf("Applied Voltage: %.6lf Volts\n", V);
    printf("Total Current (I = V / R): %.6lf Amperes\n", I);

    //Series and Parallel specific calculations
    if (type == 'S' || type == 's') {    
        printf("\n    Current through each resistor (Series)    \n");
        printf("All resistors carry the same current: %.6lf A\n", I);
        printf("\n    Voltage drops:    \n");
        for (int i = 0; i < count; i++) {
            double V_drop = I * resistors[i];
            printf("V_%d = %.6lf A times %.4lf Ohm = %.6lf V\n", i + 1, I, resistors[i], V_drop);    //Calculate voltage drop across each resistor
        }
    } else {
        printf("\n    Voltage across each resistor (Parallel)    \n");
        printf("All resistors have the same voltage: %.6lf V\n", V);
        printf("\n    Current through each resistor:    \n");
        for (int i = 0; i < count; i++) {
            double I_resistor = V / resistors[i];
            printf("I_%d = %.6lf V / %.4lf Ohm = %.6lf A\n", i + 1, V, resistors[i], I_resistor);   //Calculate current through each resistor
        }
    }
}