//Module 2 Assignment Michael Owen David Wittsack

#include "Statistic.cpp"

using namespace std;


// program entry point
int main()
{

    Statistic statistic1;
    statistic1.add(1);
    statistic1.add(2);
    statistic1.add(3);
    statistic1.add(4);
    statistic1.add(5);
    statistic1.add(6);
    statistic1.add(7);
    statistic1.add(8);
    statistic1.add(9);
    statistic1.add(10);

    std::cout << "The average of all values is: " << statistic1.getAverage() << std::endl;
    std::cout << "The average standard deviation of all values is: " << statistic1.getStandardDeviation() << std::endl;

    Statistic statistic2;
    statistic2.add(1);
    statistic2.add(1);
    statistic2.add(2);
    statistic2.add(6);
    statistic2.add(24);
    statistic2.add(120);
    statistic2.add(720);
    statistic2.add(5040);
    statistic2.add(40320);
    statistic2.add(362880);
    statistic2.add(3628800);
    statistic2.add(39916800);
    statistic2.add(479001600);
    statistic2.add(6227020800);
    statistic2.add(87178291200);
    statistic2.add(1307674368000);
    statistic2.add(20922789888000);
    statistic2.add(355687428096000);
    statistic2.add(6402373705728000);
    statistic2.add(121645100408832000);
    statistic2.add(2432902008176640000);
    
    std::cout << "The average of all values is: " << statistic2.getAverage() << std::endl;
    std::cout << "The average standard deviation of all values is: " << statistic2.getStandardDeviation() << std::endl;

    vector<double> vectorTestCase3 {14223, 17368, 8450, 31077, 32453, 26742, 3493, 18505, 26358, 30606, 13982, 17115, 11787, 14386,
        1095, 14173, 4939, 11872, 25912, 10841, 22571, 26756, 30139, 12352, 17255, 17983, 15232, 2641, 23121, 7271, 18834,
        2748, 17513, 23119, 24714, 1032, 3375, 5192, 24323, 18354, 14389, 5991, 5709, 30743, 23416, 27273, 22374, 12691, 
        13296, 24725, 19493, 20203, 26726, 8193, 28857, 19688, 28294, 17304, 16722, 4421, 17973, 30842, 28702, 6348, 32549,
        15716, 14199, 20667, 15526, 10033, 7259, 8044, 11199, 20129, 13596, 24629, 8061, 19290, 18520, 22158, 18315, 24620, 
        22572, 23665, 13571, 15513, 28069, 20487, 8580, 30613, 14455, 1152, 11397, 10122, 22258, 14914, 744, 15263, 23089,
        25159, 9426 };
    
    Statistic statistic3;
    statistic3.addList(vectorTestCase3);

    std::cout << "The average of all values is: " << statistic3.getAverage() << std::endl;
    std::cout << "The average standard deviation of all values is: " << statistic3.getStandardDeviation() << std::endl;

    return 0;
};