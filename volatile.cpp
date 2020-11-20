#include <immintrin.h>
#include <iostream>
#include <thread>
#include <Windows.h>

// importantly, set msvc before compile: 
// (1) Optimization -> Maximum Optimization (Favor Speed) (/O2)
// (2) Basic Runtime Checks -> Default
// (3) SDL checks -> No

// result:
// (1) use volatile modify g_stop,   fun1 will stop
// (2) not use volatile modify g_stop, fun1 will not stop

//use volatile disasm
/*
std::cout << "fun1 start" << std::endl;
while (!g_stop)
00007FF7341E1DE3  movzx       eax, byte ptr[g_stop(07FF7341EB3DEh)]
00007FF7341E1DEA  test        al, al
00007FF7341E1DEC  je          fun1 + 33h(07FF7341E1DE3h)
{

}
std::cout << "fun1 stop" << std::endl;
*/

// not use volatile disasm
/*
std::cout << "fun1 start" << std::endl;
while (!g_stop)
00007FF6BF7F1DF0  test        al, al
00007FF6BF7F1DF2  je          fun1 + 40h(07FF6BF7F1DF0h)
{

}
std::cout << "fun1 stop" << std::endl;
*/

// so volatile more a step, movzx eax, byte ptr[],  read from memory not register


static volatile bool g_stop = false;
//static  bool g_stop = false;
static void fun1()
{
	std::cout << "fun1 start" << std::endl;
	while (!g_stop)
	{

	}
	std::cout << "fun1 stop" << std::endl;

}

static void fun2()
{
	Sleep(10);
	std::cout << "fun2 start" << std::endl;
	Sleep(1000);
	g_stop = true;
	std::cout << "fun2 end" << std::endl;

}

void volatile_test()
{
	std::cout << "volatile test start" << std::endl;
	std::thread t1(fun1);
	SetThreadAffinityMask(t1.native_handle(), 1);
	std::thread t2(fun2);
	SetThreadAffinityMask(t2.native_handle(), 2);
	
	t1.join();
	t2.join();
	
}