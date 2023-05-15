#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <iostream>
#include <conio.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace std;
using namespace std::chrono;

struct stats {
    size_t comparison_count = 0;
    size_t copy_count = 0;
    int time_count = 0;

    void operator+=(const stats& test) {
        comparison_count += test.comparison_count;
        copy_count += test.copy_count;
        time_count += test.time_count;
    }
};

template<typename Iterator>
stats bubble_sort(Iterator begin, Iterator end) {

    stats result;
    auto start = high_resolution_clock::now();

    for (auto i = begin; i != end; ++i) {
        for (auto j = i; j != end; ++j) {
            if (*i > *j) {
                std::iter_swap(i, j);
                result.copy_count += 3;
            }
            result.comparison_count++;
        }
    }
    auto finish = high_resolution_clock::now();
    auto times = duration_cast<microseconds>(finish - start);

    result.time_count = (int)times.count();
    return result;
}

template<typename Iterator>
stats shaker_sort(Iterator begin, Iterator end) {

    stats result;
    auto start = high_resolution_clock::now();

    bool swapped = true;
    Iterator left = begin, right = end - 1;
    while (left < right && swapped) {
        swapped = false;
        // сначала идем вправо
        for (Iterator i = left; i < right; ++i) {
            ++result.comparison_count;
            if (*i > *(i + 1)) {
                std::iter_swap(i, i+1);
                result.copy_count += 3;
                swapped = true;
            }
        }
        --right;
        // затем идем влево
        for (Iterator i = right; i > left; --i) {
            ++result.comparison_count;
            if (*i < *(i - 1)) {
                std::iter_swap(i, i-1);
                result.copy_count += 3;
                swapped = true;
            }
        }
        ++left;
    }

    auto finish = high_resolution_clock::now();
    auto times = duration_cast<microseconds>(finish - start);

    result.time_count = (int)times.count();

    return result;
}

template <typename Iterator>
stats merge(Iterator begin, Iterator middle, Iterator end)
{

    vector<typename Iterator::value_type> left_vec(begin, middle), right_vec(middle, end);
    Iterator left = left_vec.begin(), right = right_vec.begin(), temp = begin;

    stats tmp;

    while (left < left_vec.end() && right < right_vec.end())
    {
        tmp.comparison_count++;
        if (*left <= *right)
        {
            *temp = *left;
            left++;
        }
        else
        {
            *temp = *right;
            right++;
        }
        temp++;
        tmp.copy_count++;
    }
    while (left < left_vec.end())
    {
        *temp = *left;
        left++;
        temp++;
        tmp.copy_count++;
    }
    while (right < right_vec.end())
    {
        *temp = *right;
        right++;
        temp++;
        tmp.copy_count++;
    }
    return tmp;
}

template <typename Iterator>
stats natural_merge_sort(Iterator begin, Iterator end)
{
    auto start = high_resolution_clock::now();
    stats empty;
    if (end - begin <= 1)
    {
        return empty;
    }
    Iterator middle = begin + (end - begin) / 2;
    stats left_stats = natural_merge_sort(begin, middle);
    stats right_stats = natural_merge_sort(middle, end);
    stats result = merge(begin, middle, end);
    result.comparison_count += left_stats.comparison_count + right_stats.comparison_count;
    result.copy_count += left_stats.copy_count + right_stats.copy_count;

    auto finish = high_resolution_clock::now();
    auto times = duration_cast<microseconds>(finish - start);

    result.time_count = (int)times.count();

    return result;
}

size_t lcg() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

stats bubble_sort_random(int n) {
    stats result;

    for (int j = 0; j < 100; j++)
    {
        std::vector<int> array;
        for (int i = 0; i < n; i++)
            array.push_back(lcg());

        auto iterator1{ array.begin() };
        auto iterator2{ array.end() - 1 };

        auto start = std::chrono::high_resolution_clock::now();
        result += bubble_sort(iterator1, iterator2);
        auto stop = std::chrono::high_resolution_clock::now();

        auto times = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        result.time_count += times.count();
    }
    cout << "\t\tBUBBLE RANDOM SORT : ";
    cout << "length of random array - " << n << endl;
    cout << "time count: " << (double)result.time_count / 100 << " microseconds" << endl;
    cout << "comparison count : " << result.comparison_count / 100 << endl;
    cout << "copy count : " << result.copy_count / 100 << endl << endl;
    return result;
}

stats shaker_sort_random(int n) {
    stats result;

    for (int j = 0; j < 100; j++)
    {
        std::vector<int> array;
        for (int i = 0; i < n; i++)
            array.push_back(lcg());

        auto iterator1{ array.begin() };
        auto iterator2{ array.end() - 1 };

        auto start = std::chrono::high_resolution_clock::now();
        result += shaker_sort(iterator1, iterator2);
        auto stop = std::chrono::high_resolution_clock::now();

        auto times = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        result.time_count += times.count();
    }
    cout << "\t\tSHAKER RANDOM SORT : ";
    cout << "length of random array - " << n << endl;
    cout << "time count: " << (double)result.time_count / 100 << " microseconds" << endl;
    cout << "comparison count : " << result.comparison_count / 100 << endl;
    cout << "copy count : " << result.copy_count / 100 << endl << endl;
    return result;
}

stats bubble_sort_sorted(int n)
{
    stats result;
    std::vector<int> array;
    for (int i = 0; i < n; i++)
    {
        array.push_back(i);
    }
    auto iterator1{ array.begin() };
    auto iterator2{ array.end() - 1 };

    auto start = std::chrono::high_resolution_clock::now();
    result += bubble_sort(iterator1, iterator2);
    auto stop = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    result.time_count += time.count();

    cout << "\t\tBUBBLE SORT SORTED : ";
    cout << "length of sorted array - " << n << endl;
    cout << "time: " << (double)result.time_count << " microseconds" << endl;
    cout << "comparison count : " << result.comparison_count << endl;
    cout << "copy count : " << result.copy_count << endl << endl;
    return result;
}

stats shaker_sort_sorted(int n)
{
    stats result;
    std::vector<int> array;
    for (int i = 0; i < n; i++)
    {
        array.push_back(i);
    }
    auto iterator1{ array.begin() };
    auto iterator2{ array.end() - 1 };

    auto start = std::chrono::high_resolution_clock::now();
    result += shaker_sort(iterator1, iterator2);
    auto stop = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    result.time_count += time.count();

    cout << "\t\tSHAKER SORT SORTED : ";
    cout << "length of sorted array - " << n << endl;
    cout << "time: " << (double)result.time_count << " microseconds" << endl;
    cout << "comparison count : " << result.comparison_count << endl;
    cout << "copy count : " << result.copy_count << endl << endl;
    return result;
}

stats buble_sort_reverse(int n)
{
    stats result;
    std::vector<int> array;
    for (int i = n; i > 0; i--)
    {
        array.push_back(i);
    }
    auto iterator1{ array.begin() };
    auto iterator2{ array.end() - 1 };

    auto start = std::chrono::high_resolution_clock::now();
    result += bubble_sort(iterator1, iterator2);
    auto stop = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    result.time_count += time.count();
    cout << "\t\tBUBLLE SORT REVERSED : ";
    cout << "length of reverse sorted array - " << n << endl;
    cout << "time: " << (double)result.time_count << " microseconds" << endl;
    cout << "comparison count : " << result.comparison_count << endl;
    cout << "copy count : " << result.copy_count << endl << endl;
    return result;
}

stats shaker_sort_reverse(int n)
{
    stats result;
    std::vector<int> array;
    for (int i = n; i > 0; i--)
    {
        array.push_back(i);
    }
    auto iterator1{ array.begin() };
    auto iterator2{ array.end() - 1 };

    auto start = std::chrono::high_resolution_clock::now();
    result += shaker_sort(iterator1, iterator2);
    auto stop = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    result.time_count += time.count();
    cout << "\t\tSHAKER SORT REVERSED : ";
    cout << "length of reverse sorted array - " << n << endl;
    cout << "time: " << (double)result.time_count << " microseconds" << endl;
    cout << "comparison count : " << result.comparison_count << endl;
    cout << "copy count : " << result.copy_count << endl << endl;
    return result;
}

stats merge_sort_random(int n) {
    stats result;

    for (int j = 0; j < 100; j++)
    {
        std::vector<int> array;
        for (int i = 0; i < n; i++)
            array.push_back(lcg());

        auto iterator1{ array.begin() };
        auto iterator2{ array.end() - 1 };

        auto start = std::chrono::high_resolution_clock::now();
        result += natural_merge_sort(iterator1, iterator2);
        auto stop = std::chrono::high_resolution_clock::now();

        auto times = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        result.time_count += times.count();
    }
    cout << "\t\tNATURAL MERGE SORT RANDOM SORT : ";
    cout << "length of random array - " << n << endl;
    cout << "time count: " << (double)result.time_count / 100 << " microseconds" << endl;
    cout << "comparison count : " << result.comparison_count / 100 << endl;
    cout << "copy count : " << result.copy_count / 100 << endl << endl;
    return result;
}

stats merge_sort_sorted(int n) {
    stats result;
    std::vector<int> array;
    for (int i = 0; i < n; i++)
    {
        array.push_back(i);
    }
    auto iterator1{ array.begin() };
    auto iterator2{ array.end() - 1 };

    auto start = std::chrono::high_resolution_clock::now();
    result += natural_merge_sort(iterator1, iterator2);
    auto stop = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    result.time_count += time.count();

    cout << "\t\tNATURAL MERGE SORT SORTED : ";
    cout << "length of sorted array - " << n << endl;
    cout << "time: " << (double)result.time_count << " microseconds" << endl;
    cout << "comparison count : " << result.comparison_count << endl;
    cout << "copy count : " << result.copy_count << endl << endl;
    return result;
}

stats merge_sort_reverse(int n) {
    stats result;

    std::vector<int> array;
    for (int i = n; i > 0; i--)
    {
        array.push_back(i);
    }
    auto iterator1{ array.begin() };
    auto iterator2{ array.end() - 1 };

    auto start = std::chrono::high_resolution_clock::now();
    result += natural_merge_sort(iterator1, iterator2);
    auto stop = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    result.time_count += time.count();
    cout << "\t\tNATURAL MERGE SORT REVERSED : ";
    cout << "length of reverse sorted array - " << n << endl;
    cout << "time: " << (double)result.time_count << " microseconds" << endl;
    cout << "comparison count : " << result.comparison_count << endl;
    cout << "copy count : " << result.copy_count << endl << endl;
    return result;
}

template<typename Iterator>
void PrintArray(Iterator begin, Iterator end) {
    std::vector<int>::iterator tmp_print = begin;
    while (tmp_print < end) {
        cout << *tmp_print << "; ";
        ++tmp_print;
    }
}

void PrintMenu()
{
    system("cls");
    std::cout << "Hello, here your menu, choose one:" << std::endl;
    std::cout << "1) Bubble sort " << std::endl;
    std::cout << "2) Shaker sort " << std::endl;
    std::cout << "3) Naturial two-way merge sort" << std::endl;
    std::cout << "4) Check stats" << std::endl;
    std::cout << "5) Go to charts" << std::endl;
    std::cout << "6) Exit" << std::endl;
    std::cout << "Choice: ";
}

void PrintMenu2()
{
    system("cls");
    std::cout << "Check stats to...:" << std::endl;
    std::cout << "1) Bubble sort " << std::endl;
    std::cout << "2) Shaker sort " << std::endl;
    std::cout << "3) Naturial two-way merge sort" << std::endl;
    std::cout << "4) Back" << std::endl;
    std::cout << "Choice: ";
}

void PrintMenu3()
{
    system("cls");
    std::cout << "Which array group should you choose?:" << std::endl;
    std::cout << "1) Random group " << std::endl;
    std::cout << "2) Sorted group " << std::endl;
    std::cout << "3) Revers group " << std::endl;
    std::cout << "4) Back" << std::endl;
    std::cout << "Choice: ";
}

int main()
{
    int choice = 0, choice_2 = 0, choice_3 = 0, back = 0;
    bool flag1 = true, flag2 = true, flag3 = true;

    std::vector<int> array = { 3, 2, 5, 10, 9, -1, -2 };

    while (flag1)
    {
        PrintMenu();
        std::cin >> choice;

        std::vector<int> tmp = array;
        auto iterator1{ tmp.begin() };
        auto iterator2{ tmp.end() };

        stats data;

        if (choice == 1 || choice == 2 || choice == 3)
        {

            cout << endl << "Old array: [";
            PrintArray(iterator1, iterator2);
            cout << "]" << endl;

            if (choice == 1) data = bubble_sort(iterator1, iterator2);
            if (choice == 2) data = shaker_sort(iterator1, iterator2);
            if (choice == 3) data = natural_merge_sort(iterator1, iterator2);
           
            cout << endl << "New array: [";
            PrintArray(iterator1, iterator2);
            cout << "]" << endl;
            cout << endl << endl;
            cout << "length of sorted array - " << array.size() << endl;
            cout << "time: " << (double)data.time_count << " microseconds" << endl;
            cout << "comparison count : " << data.comparison_count << endl;
            cout << "copy count : " << data.copy_count << endl << endl;

            cout << endl << endl;
            cout << endl << "Press 'Backspace' if want to back" << endl << endl;
            back = _getch();
        }
        else if (choice == 4)
        {
            std::vector<int> array_test = { 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 25000, 50000, 100000 };

            while (flag2) {
                PrintMenu2();
                std::cin >> choice_2;
                std::ofstream out1, out2, out3;

                if (choice_2 == 1) {      //bubble sort

                    while (flag3) {

                        PrintMenu3();
                        std::cin >> choice_3;

                        if (choice_3 == 1) {  //random bubble sort

                            out1.open("Bubble_random_comparison.txt");
                            out2.open("Bubble_random_copy.txt");
                            out3.open("Bubble_random_time.txt");

                            if (out1.is_open() && out2.is_open() && out3.is_open())
                            {
                                for (int i = 0; i < 13; i++)
                                {
                                    stats st = bubble_sort_random(array_test[i]);
                                    
                                    out1 << array_test[i] << ' ' << st.comparison_count << std::endl;
                                    out2 << array_test[i] << ' ' << st.copy_count << std::endl;
                                    out3 << array_test[i] << ' ' << st.time_count << std::endl;
                                }
                            }
                            out1.close();
                            out2.close();
                            out3.close();

                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();
                        }
                        else if (choice_3 == 2) { //bubble sort sorted

                            out1.open("Bubble_sorted_comparison.txt");
                            out2.open("Bubble_sorted_copy.txt");
                            out3.open("Bubble_sorted_time.txt");

                            if (out1.is_open() && out2.is_open() && out3.is_open())
                            {
                                for (int i = 0; i < 13; i++)
                                {
                                    stats st = bubble_sort_sorted(array_test[i]);

                                    out1 << array_test[i] << ' ' << st.comparison_count << std::endl;
                                    out2 << array_test[i] << ' ' << st.copy_count << std::endl;
                                    out3 << array_test[i] << ' ' << st.time_count << std::endl;
                                }

                            }
                            out1.close();
                            out2.close();
                            out3.close();

                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();
                        }
                        else if (choice_3 == 3) {  //bubble sort reverse

                            out1.open("Bubble_reversed_comparison.txt");
                            out2.open("Bubble_reversed_copy.txt");
                            out3.open("Bubble_reversed_time.txt");

                            if (out1.is_open() && out2.is_open() && out3.is_open())
                            {
                                for (int i = 0; i < 13; i++)
                                {
                                    stats st = buble_sort_reverse(array_test[i]);

                                    out1 << array_test[i] << ' ' << st.comparison_count << std::endl;
                                    out2 << array_test[i] << ' ' << st.copy_count << std::endl;
                                    out3 << array_test[i] << ' ' << st.time_count << std::endl;
                                }
                            }
                            out1.close();
                            out2.close();
                            out3.close();

                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();
                        }
                        else if (choice_3 == 4) {
                            break;
                        }
                        else {
                            cout << "invalid character entered!!!!!!!    try again" << endl;
                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();
                            if (back == 8) flag3 = true;
                        }
                    }
                }
                else if (choice_2 == 2) {   //shaker sort
                    while (flag3) {

                        PrintMenu3();

                        std::cin >> choice_3;

                        if (choice_3 == 1) {   //shaker sort random

                            out1.open("Shaker_random_comparison.txt");
                            out2.open("Shaker_random_copy.txt");
                            out3.open("Shaker_random_time.txt");

                            if (out1.is_open() && out2.is_open() && out3.is_open())
                            {
                                for (int i = 0; i < 13; i++)
                                {
                                    stats st = shaker_sort_random(array_test[i]);

                                    out1 << array_test[i] << ' ' << st.comparison_count << std::endl;
                                    out2 << array_test[i] << ' ' << st.copy_count << std::endl;
                                    out3 << array_test[i] << ' ' << st.time_count << std::endl;
                                }
                            }
                            out1.close();
                            out2.close();
                            out3.close();

                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();
                        }
                        else if (choice_3 == 2) {
                            
                            out1.open("Shaker_sorted_comparison.txt");
                            out2.open("Shaker_sorted_copy.txt");
                            out3.open("Shaker_sorted_time.txt");

                            if (out1.is_open() && out2.is_open() && out3.is_open())
                            {
                                for (int i = 0; i < 13; i++)
                                {
                                    stats st = shaker_sort_sorted(array_test[i]);

                                    out1 << array_test[i] << ' ' << st.comparison_count << std::endl;
                                    out2 << array_test[i] << ' ' << st.copy_count << std::endl;
                                    out3 << array_test[i] << ' ' << st.time_count << std::endl;
                                }
                            }
                            out1.close();
                            out2.close();
                            out3.close();

                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();

                        }
                        else if (choice_3 == 3) {

                            out1.open("Shaker_reversed_comparison.txt");
                            out2.open("Shaker_reversed_copy.txt");
                            out3.open("Shaker_reversed_time.txt");

                            if (out1.is_open() && out2.is_open() && out3.is_open())
                            {
                                for (int i = 0; i < 13; i++)
                                {
                                    stats st = shaker_sort_reverse(array_test[i]);

                                    out1 << array_test[i] << ' ' << st.comparison_count << std::endl;
                                    out2 << array_test[i] << ' ' << st.copy_count << std::endl;
                                    out3 << array_test[i] << ' ' << st.time_count << std::endl;
                                }
                            }
                            out1.close();
                            out2.close();
                            out3.close();

                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();
                        }
                        else if (choice_3 == 4) {
                            break;
                        }
                        else {
                            cout << "invalid character entered!!!!!!!    try again" << endl;
                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();
                            if (back == 8) flag3 = true;
                        }
                    }
                }
                else if (choice_2 == 3) {
                    while (flag3) {
                        PrintMenu3();
                        std::cin >> choice_3;
                        if (choice_3 == 1) {

                            out1.open("Merge_random_comparison.txt");
                            out2.open("Merge_random_copy.txt");
                            out3.open("Merge_random_time.txt");

                            if (out1.is_open() && out2.is_open() && out3.is_open())
                            {
                                for (int i = 0; i < 13; i++)
                                {
                                    stats st = merge_sort_random(array_test[i]);

                                    out1 << array_test[i] << ' ' << st.comparison_count << std::endl;
                                    out2 << array_test[i] << ' ' << st.copy_count << std::endl;
                                    out3 << array_test[i] << ' ' << st.time_count << std::endl;
                                }
                            }
                            out1.close();
                            out2.close();
                            out3.close();

                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();

                        }
                        else if (choice_3 == 2) {

                            out1.open("Merge_sorted_comparison.txt");
                            out2.open("Merge_sorted_copy.txt");
                            out3.open("Merge_sorted_time.txt");

                            if (out1.is_open() && out2.is_open() && out3.is_open())
                            {
                                for (int i = 0; i < 13; i++)
                                {
                                    stats st = merge_sort_sorted(array_test[i]);

                                    out1 << array_test[i] << ' ' << st.comparison_count << std::endl;
                                    out2 << array_test[i] << ' ' << st.copy_count << std::endl;
                                    out3 << array_test[i] << ' ' << st.time_count << std::endl;
                                }

                            }
                            out1.close();
                            out2.close();
                            out3.close();

                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();

                        }
                        else if (choice_3 == 3) {

                            out1.open("Merge_reversed_comparison.txt");
                            out2.open("Merge_reversed_copy.txt");
                            out3.open("Merge_reversed_time.txt");

                            if (out1.is_open() && out2.is_open() && out3.is_open())
                            {
                                for (int i = 0; i < 13; i++)
                                {
                                    stats st = merge_sort_reverse(array_test[i]);

                                    out1 << array_test[i] << ' ' << st.comparison_count << std::endl;
                                    out2 << array_test[i] << ' ' << st.copy_count << std::endl;
                                    out3 << array_test[i] << ' ' << st.time_count << std::endl;
                                }
                            }
                            out1.close();
                            out2.close();
                            out3.close();

                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();

                        }
                        else if (choice_3 == 4) {
                            break;
                        }
                        else {
                            cout << "invalid character entered!!!!!!!    try again" << endl;
                            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                            back = _getch();
                            if (back == 8) flag3 = true;
                        }
                    }
                }
                else if (choice_2 == 4) {
                    break;
                }
                else
                {
                    cout << "invalid character entered!!!!!!!   try again" << endl;
                    cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
                    back = _getch();
                    if (back == 8) flag2 = true;
                }
            }
        }
        else if (choice == 5) {
            system("C:\\PYTHON\\test_py\\stats_sort.ipynb");
            back = _getch();
            if (back == 8) flag1 = true;
        }
        else if (choice == 6)
            return 0;

        else
        {
            cout << "invalid character entered!!!!!!!    try again" << endl;
            cout << endl << endl << "Press 'Backspace' if want to back" << endl << endl;
            back = _getch();
            if (back == 8) flag1 = true;
        }
    }
}