// g++ -c thread_cpp_calc.cpp -pthread -std=c++11         // generate target object file
// g++ thread_cpp_stdcout.o -o thread_cpp_calc.out -pthread -std=c++11  // link to target binary
// time ./thread_cpp_calc.out

#include <iostream>
#include <thread>  
#include <mutex>
#include <cmath>

static const unsigned long int N = 100000000;

//--- глобальный мутекс используется 
// для обеспечения эксклюзивного доступа к std::cout
struct PCout
{
    static std::mutex& Mutex()
    {
        static std::mutex mut;
        return mut;
    }
};

#define thread_cout(msg) PCout::Mutex().lock(), msg, PCout::Mutex().unlock()

//---------------------------------------------------------- 
//----------------------------------------------------------
//----------------------------------------------------------


void calc() 
{
    double h = 1e-10;
    double Rieman_sum = 0;
    for(int i = 0; i < N / 2; ++i) {
        Rieman_sum += 0.5 * (sin(i + h) - sin(i)) * h;
    }
}

struct some
{
    //--- метод будет запущен в разных потоках
    void foo()
    {
        calc();
    }
};

int main() 
{
    calc();
    calc();

    return 0;
}

int main_par() 
{

    // --- объекты, методы которых хотим запускать в разных потоках
    some obj1, obj2;
  
    // --- запускаем методы в разных потоках
    std::thread first([&obj1](){ obj1.foo(); }); 
    std::thread second([&obj2](){ obj2.foo(); });


    //--- ждем завершения работы потоков
    first.join();  
    second.join(); 
}
