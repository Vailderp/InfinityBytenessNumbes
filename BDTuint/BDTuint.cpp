#include <iostream>
#include <chrono>
#include "BDTnumeric.h"
#include <bitset>
#include "bdt_numerics.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main()
{

    const char* number_first = "5096"; //5096 - it is 2 bytes in BCD8421-system
    const char* number_second = "5096";

    // BDTN_CEIL_8421BCD_NUMBER - stores the memory of a BCD8421-number
    BDTN_CEIL_8421BCD_NUMBER* BCD1 = nullptr; // The result of BCD2 + BCD3
    BDTN_CEIL_8421BCD_NUMBER* BCD2 = nullptr; // The value of number_first
    BDTN_CEIL_8421BCD_NUMBER* BCD3 = nullptr; // The value of number_second

    // Descriptions of BCD8421-numbers
    BDTN_CEIL_8421BCD_NUMBER_DESC BCD_DESC1; // The description of BCD1
    BDTN_CEIL_8421BCD_NUMBER_DESC BCD_DESC2; // The description of BCD2
    BDTN_CEIL_8421BCD_NUMBER_DESC BCD_DESC3; // The description of BCD3

    // One byte in BCD8421-number - the number consists of two digits in Base10
    BCD_DESC2.byteness = BdtnCalculate8421BCDByteness(number_first);    // 5096 - it is 2 bytes in BCD8421-system
    BCD_DESC3.byteness = BdtnCalculate8421BCDByteness(number_second);   // 5096 - it is 2 bytes in BCD8421-system
    BCD_DESC1.byteness = BCD_DESC2.byteness + 1;        // 5096 + 5096 is can be maximum 3 bytes in BCD8421-system
  
    BdtnCreateCeil8421BCDNumber(&BCD1, BCD_DESC1); // Creation BCD1
    BdtnCreateCeil8421BCDNumber(&BCD2, BCD_DESC2); // Creation BCD2
    BdtnCreateCeil8421BCDNumber(&BCD3, BCD_DESC3); // Creation BCD3

    BdtnCeilDecStrToCeil8421BCDNumber(BCD2, number_first);  // Assigning the value of a number from a number_first
    BdtnCeilDecStrToCeil8421BCDNumber(BCD3, number_second); // Assigning the value of a number from a number_second

    // BDTN_CEIL_NUMBER - stores the memory of a Base2-number 
    BDTN_CEIL_NUMBER* CEIL1 = nullptr;
    BDTN_CEIL_NUMBER* CEIL2 = nullptr;
    BDTN_CEIL_NUMBER* CEIL3 = nullptr;

    // Descriptions of Base2-numbers
    BDTN_CEIL_NUMBER_DESC CEIL_DESC1;
    BDTN_CEIL_NUMBER_DESC CEIL_DESC2;
    BDTN_CEIL_NUMBER_DESC CEIL_DESC3;
    CEIL_DESC1.byteness = 2; // Byteness of Base2-number result
    CEIL_DESC2.byteness = 2; // Byteness of 5096
    CEIL_DESC3.byteness = 2; // Byteness of 5096
    BdtnCreateCeilNumber(&CEIL1, CEIL_DESC1); // Creation CEIL1
    BdtnCreateCeilNumber(&CEIL2, CEIL_DESC2); // Creation CEIL2
    BdtnCreateCeilNumber(&CEIL3, CEIL_DESC3); // Creation CEIL3

    auto t1 = high_resolution_clock::now();

    BdtnCeil8421BCDNumberToCeilNumber(BCD2, CEIL2); // Converting from BSD8421-system to Base2-system (BCD2 to CEIL2) 
    BdtnCeil8421BCDNumberToCeilNumber(BCD3, CEIL3); // Converting from BSD8421-system to Base2-system (BCD3 to CEIL3) 
    BdtnSumCeilNumber(CEIL1, CEIL2, CEIL3); // Write the sum of CEIL2 and CEIL3 in CEIL1
    BdtnCeilNumberToCeil8421BCDNumber(CEIL1, BCD1); // Converting from Base2-system to BSD8421-system (CEIL1 to BCD1)

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<std::chrono::nanoseconds>(t2 - t1);
    duration<double, std::nano> ms_double = t2 - t1;
    std::cout << ms_int.count() << "ns\n";
    std::cout << ms_double.count() << "ns\n";

    std::cout << "Binary : " << BdtnCeilNumberToBinStr(CEIL1) << std::endl; // Write binary value in console
    std::cout << "Decimal: " << BdtnCeil8421BCDNumberToDecStr(BCD1) << std::endl; // Write decimal value in console

    BdtnDestroyCeil8421BCDNumber(BCD1);
    BdtnDestroyCeil8421BCDNumber(BCD2);
    BdtnDestroyCeil8421BCDNumber(BCD3);
    BdtnDestroyCeilNumber(CEIL1);
    BdtnDestroyCeilNumber(CEIL2);
    BdtnDestroyCeilNumber(CEIL3);

    return (int)true;
}
