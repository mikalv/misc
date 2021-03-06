#define XBYAK_NO_OP_NAMES
#include <xbyak/xbyak_util.h>
#include <cybozu/benchmark.hpp>

const int N = 1000000;

int (*f1)(); // shrx
int (*f2)(); // shr
int (*f3)(); // rorx 1
int (*f4)(); // ror 1
int (*f5)(); // rorx n
int (*f6)(); // ror n
int (*f7)(); // bextr
int (*f8)(); // bzhi
int (*f9)(); // hash with rorx
int (*fa)(); // hash with ror

struct Code : Xbyak::CodeGenerator {
	Code()
	{
		f1 = getCurr<int (*)()>();
		gen1();
		align(16);
		f2 = getCurr<int (*)()>();
		gen2();
		align(16);
		f3 = getCurr<int (*)()>();
		gen3(1);
		align(16);
		f4 = getCurr<int (*)()>();
		gen4(1);

		align(16);
		f5 = getCurr<int (*)()>();
		gen3(5);
		align(16);
		f6 = getCurr<int (*)()>();
		gen4(5);

		align(16);
		f7 = getCurr<int (*)()>();
		gen5();
		align(16);
		f8 = getCurr<int (*)()>();
		gen6();

		align(16);
		f9 = getCurr<int (*)()>();
		gen7(true);
		align(16);
		fa = getCurr<int (*)()>();
		gen7(false);
	}
	void gen1()
	{
		mov(ecx, N);
		xor_(eax, eax);
		mov(edx, 1);
	L("@@");
		shrx(r8, rcx, rdx);
		add(rax, r8);
		dec(ecx);
		jnz("@b");
		ret();
	}
	void gen2()
	{
		mov(ecx, N);
		xor_(eax, eax);
	L("@@");
		mov(rdx, rcx);
		shr(rdx, 1);
		add(rax, edx);
		dec(ecx);
		jnz("@b");
		ret();
	}
	void gen3(int shift)
	{
		mov(ecx, N);
		xor_(eax, eax);
	L("@@");
		rorx(r8, rcx, shift);
		add(rax, r8);
		sub(ecx, 1);
		jnz("@b");
		ret();
	}
	void gen4(int shift)
	{
		mov(ecx, N);
		xor_(eax, eax);
	L("@@");
		mov(r8, rcx);
		ror(r8, shift);
		add(rax, r8);
		sub(ecx, 1);
		jnz("@b");
		ret();
	}
	void gen5()
	{
		mov(ecx, N);
		xor_(eax, eax);
		mov(edx, 3 << 8);
	L("@@");
		bextr(r8, rcx, rdx);
		add(rax, r8);
		sub(ecx, 1);
		jnz("@b");
		ret();
	}
	void gen6()
	{
		mov(ecx, N);
		xor_(eax, eax);
		mov(edx, 3);
	L("@@");
		bzhi(r8, rcx, rdx);
		add(rax, r8);
		sub(ecx, 1);
		jnz("@b");
		ret();
	}
	void gen7(bool useRorx)
	{
		mov(ecx, N);
		xor_(eax, eax);
	L("@@");
		if (useRorx) {
			rorx(r8, rcx, 2);
			xor_(rax, r8);
			rorx(r8, rcx, 13);
			xor_(rax, r8);
			rorx(r8, rcx, 22);
			xor_(rax, r8);
		} else {
			mov(r8, rcx);
			ror(r8, 2);
			xor_(rax, r8);
			ror(r8, 11);
			xor_(rax, r8);
			ror(r8, 9);
			xor_(rax, r8);
		}
		sub(ecx, 1);
		jnz("@b");
		ret();
	}
};

int main()
	try
{
	Code c;
#if 0
	printf("%x\n", f1());
	printf("%x\n", f2());
	CYBOZU_BENCH("f1", f1);
	CYBOZU_BENCH("r2", f2);

	printf("%x\n", f3());
	printf("%x\n", f4());
	CYBOZU_BENCH("f3", f3);
	CYBOZU_BENCH("r4", f4);

	printf("%x\n", f5());
	printf("%x\n", f6());
	CYBOZU_BENCH("f3", f3);
	CYBOZU_BENCH("r4", f4);

	printf("%x\n", f7());
	printf("%x\n", f8());
	CYBOZU_BENCH("f7", f7);
	CYBOZU_BENCH("r8", f8);
#endif

	printf("%x\n", f9());
	printf("%x\n", fa());
	CYBOZU_BENCH("f9", f9);
	CYBOZU_BENCH("ra", fa);
} catch (std::exception& e) {
	printf("err=%s\n", e.what());
}

