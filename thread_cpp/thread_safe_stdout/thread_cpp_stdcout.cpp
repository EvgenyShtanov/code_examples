// g++ -c thread_cpp_stdcout.cpp -pthread -std=c++11         // generate target object file
// g++ thread_cpp_stdcout.o -o thread_cpp_stdcout.out -pthread -std=c++11  // link to target binary

#include <iostream>
#include <thread>  
#include <mutex>

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



struct some
{
    //--- метод будет запущен в разных потоках
    void foo()
    {
        // потоко-безопасный вывод данных в std::сout
        thread_cout( 
            std::cout<<"[thread] id = " << std::this_thread::get_id() << std::endl
        );
    }
};


int main() 
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
