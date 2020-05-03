#include <iostream>
#include <vector>
#include <chrono>
#include "Board.h"
#include "Board2.h"
#include "BoardK.h"

using namespace std;

void test_numSolutions();
void test_numSolutionsTime();
void test_numSolutionsRate();
void test_genQuest();
void test_genLink3();		//	伸長法による盤面生成
void test_setRandom();
void test_BoardK();

int main()
{
	//test_numSolutions();
	//test_genQuest();
	//test_setRandom();
	//test_numSolutionsTime();
	//test_numSolutionsRate();
	//test_genLink3();
	test_BoardK();
	
#if	0
	BoardK bd(4, 4);
	bd.setNumbers("1..1............");
	//bd.setNumbers("1231.....23.....");
#endif
#if	0
	BoardK bd(7, 7);
	bd.setNumbers("....321""....1.."".......""..2....""......."".35..4.""4.....5");		//	7x7
#endif
#if	0
	BoardK bd(10, 10);
	bd.setNumbers(".........4"
		".....5.67."
		"..12......"
		".........."
		"...13....."
		".....43..."
		".........."
		"......52.."
		".78.8....."
		"6.........");		//	10x10
#endif
#if	0
	//bd.setRandomNoAdj(8);
	//bd.genUniqRandom(8);
	cout << bd.text() << "\n";
	//cout << bd.text() << "\n";
#if	0
	if( bd.canSolve() )
		cout << "can solve.\n";
	else
		cout << "can NOT solve.\n";
#endif
	cout << "numSolutions = " << bd.numSolutions() << "\n";
#endif
#if	0
	cout << "checkUniq(): ";
	if( bd.checkUniq() )
		cout << "is proper.\n";
	else
		cout << "is NOT proper.\n";
	cout << "checkUniqMate(): ";
	if( bd.checkUniqMate() )
		cout << "is proper.\n";
	else
		cout << "is NOT proper.\n";
#endif
#if	0
	Board bd(6, 6);
	//bd.setRandomSymmetrical(8);
	//cout << bd.text3() << "\n";
	bool b = false;
	for (int i = 0; i < 1000000; ++i) {
		bd.setRandomSymmetrical(4);
		if( bd.isUniqBT() ) {
			//cout << "i = " << i << "\n";
			b = true;
			break;
		}
	}
	if( b ) {
		//cout << bd.text2() << "\n";
		cout << bd.text3() << "\n";
	} else {
		cout << "faild.\n";
	}
#endif
#if	0
	Board bd(9, 9);
	for (int i = 0; i < 1000000; ++i) {
		bd.setRandom(9);
		if( bd.isUniqBT() ) {
			cout << "i = " << i << "\n";
			break;
		}
	}
	//cout << bd.text2() << "\n";
	cout << bd.text3() << "\n";
	//bd.setNumber("1234........1234");
	//bd.setNumber("1231.....23.....");
#endif
#if	0
	//Board bd(7, 7);
	//bd.setNumber("...4....3..25....31.....5............1....2...4..");
	//bd.setNumber("....321""....1.."".......""..2....""......."".35..4.""4.....5");		//	7x7
	Board bd(10, 10);
	bd.setNumber(	".........4"
					".....5.67."
					"..12......"
					".........."
					"...13....."
					".....43..."
					".........."
					"......52.."
					".78.8....."
					"6........." );		//	10x10
#endif
#if	0
	Board bd(13, 13);
	bd.setNumber(	"............."
					".12.......34."
					"............."
					"..56..7..78.."
					"......9......"
					"...A3...45...":e bo
					"2...........B"
					"...CD...B1..."
					"......8......"
					"..DC..9..FE.."
					"............."
					".6F.......EA."
					"............." );		//	13x13
#endif
#if	0
	Board bd(15, 10);
	bd.setNumber(	".....5........."
					"............17."
					"..2..8........."
					".........9....."
					".3......4..A.45"
					"12.3..9......6."
					".....7........."
					".........8..B.."
					".BA............."
					".........6....." );		//	15x10
#endif
#if	0
	cout << "text0():\n";
	cout << bd.text0() << "\n\n";
	cout << "text():\n";
	cout << bd.text() << "\n\n";
	cout << "text2():\n";
	cout << bd.text2() << "\n\n";
#endif
#if	0
	cout << "text():\n";
	cout << bd.text() << "\n\n";
#endif
	//cout << bd.textLink2() << "\n";
#if	0
	if( bd.doSolveBT() ) {
		cout << "solved.\n";
		cout << bd.text2() << "\n";
		cout << bd.text3() << "\n";
	} else {
		cout << "can't solved.\n";
	}
#endif
#if	0
	if( bd.isUniqBT() )
		cout << "is proper quest.\n";
	else
		cout << "is NOT proper quest.\n";
#endif
	//
    std::cout << "\nOK\n";
}
void test_numSolutions()
{
	if( false ) {
		BoardK bd(2, 2);
		bd.setNumbers(	"11" ".." );		//	2x2
		cout << bd.text() << "\n";
		cout << "numSolutions = " << bd.numSolutions() << "\n\n";
	}
	if( false ) {
		BoardK bd(2, 2);
		bd.setNumbers(	"1." ".1" );		//	2x2
		cout << bd.text() << "\n";
		cout << "numSolutions = " << bd.numSolutions() << "\n\n";
	}
	if( false ) {
		BoardK bd(3, 3);
		bd.setNumbers(	"1.1" "..." "...");		//	3x3
		cout << bd.text() << "\n";
		cout << "numSolutions = " << bd.numSolutions() << "\n\n";
	}
	if( false ) {
		BoardK bd(3, 3);
		bd.setNumbers(	"1.." "..." "..1");		//	3x3
		cout << bd.text() << "\n";
		cout << "numSolutions = " << bd.numSolutions() << "\n\n";
	}
	if( true ) {
		cout << "10x10:\n";
		BoardK bd(10, 10);
		bd.setNumbers(	".........4"
						".....5.67."
						"..12......"
						".........."
						"...13....."
						".....43..."
						".........."
						"......52.."
						".78.8....."
						"6........." );		//	10x10
		cout << bd.text() << "\n";
		cout << "numSolutions = " << bd.numSolutions() << "\n\n";
	}
	if( true ) {
		cout << "12x12:\n";
		BoardK bd(12, 12);
		bd.setNumbers(	"......1....."
						".8...32..B6."
						"...A........"
						"..2......B.."
						"..C......1.."
						"........5..."
						"...4........"
						"....7....A.."
						"....9....C.."
						"........6..."
						".87..45...9."
						".....3......" );		//	12x12
		cout << bd.text() << "\n";
		cout << "numSolutions = " << bd.numSolutions() << "\n\n";
	}
	if( true ) {
		cout << "13x13:\n";
		BoardK bd(13, 13);
		bd.setNumbers(	"............."
						".12.......34."
						"............."
						"..56..7..78.."
						"......9......"
						"...A3...45..."
						"2...........B"
						"...CD...B1..."
						"......8......"
						"..DC..9..FE.."
						"............."
						".6F.......EA."
						"............." );		//	13x13
		cout << bd.text() << "\n";
		cout << "numSolutions = " << bd.numSolutions() << "\n\n";
	}
	if( false ) {
		cout << "15x10:\n";
		BoardK bd(15, 10);
		bd.setNumbers(	".....5........."
						"............17."
						"..2..8........."
						".........9....."
						".3......4..A.45"
						"12.3..9......6."
						".....7........."
						".........8..B.."
						".BA............."
						".........6....." );		//	15x10
		cout << bd.text() << "\n";
		cout << "numSolutions = " << bd.numSolutions() << "\n\n";
	}
	if( true ) {
		cout << "15x15:\n";
		BoardK bd(15, 15);
		bd.setNumbers(	"..........4...."
						"......3.9...E5."
						"..2...D.5......"
						"..............."
						".A..........E.."
						".......2......."
						".1...D...9.8.7."
						"...A..........."
						"..............."
						"...CB.........."
						"......C..1...6."
						".........B....."
						"......6.....7.."
						".8...4........."
						"...3..........." );		//	15x15
		cout << bd.text() << "\n";
		cout << "numSolutions = " << bd.numSolutions() << "\n\n";
	}
}
string g_qstr;
void test_genQuest()
{
	const int wd = 9;
	cout << wd << "x" << wd << ":\n";
	Board2 b2(wd, wd);
	BoardK bk(wd, wd);
	for (;;) {
		b2.genRandomLink3(wd-1);		//	伸張法による盤面生成
		if( b2.isFilled() && !b2.isAnsCrowded() ) {
			g_qstr = b2.text0();
			bk.setNumbers(g_qstr.c_str());
			if( bk.numSolutions() == 1 ) {
				cout << bk.text() << "\n";
				return;
				//return g_qstr.c_str();
			}
		}
	}
#if	0
	const int N_LOOP = 100000;
	int cnt = 0;
	BoardK bd(8,8);
	for (int i = 0; i < N_LOOP; ++i) {
		bd.setRandomNoAdj(7);
		if( bd.numSolutions() == 1 ) {
			cout << ++cnt << ":\n";
			cout << bd.text() << "\n";
		}
	}
#endif
}
void test_setRandom()
{
	const int N_LOOP = 1000*1000;
	cout << "setRandom() x " << N_LOOP << "times: ";
	BoardK bd(8, 8);
	auto start = std::chrono::system_clock::now();      // 計測スタート時刻
	for (int i = 0; i < N_LOOP; ++i) {
		bd.setRandom(7);
	}
	auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
    auto dur = end - start;        // 要した時間を計算
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "dur = " << msec << "msec\n";
}
void test_numSolutionsTime()
{
	const int N_LOOP = 1000;
	cout << "numSolutions() x " << N_LOOP << "times: ";
	//vector<int> vcnt;
	BoardK bd(8, 8);
	auto start = std::chrono::system_clock::now();      // 計測スタート時刻
	for (int i = 0; i < N_LOOP; ++i) {
		bd.setRandom(7);
		int n = bd.numSolutions();
	}
	auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
    auto dur = end - start;        // 要した時間を計算
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "dur = " << msec << "msec\n";
}
void test_numSolutionsRate()
{
	const int N_LOOP = 1000;
	const int WD = 8;
	cout << "numSolutions() x " << N_LOOP << "times:\n";
	vector<int> vcnt;
	BoardK bd(WD, WD);
	for (int i = 0; i < N_LOOP; ++i) {
		bd.setRandom(WD-1);
		int n = bd.numSolutions();
		if( n >= vcnt.size() )
			vcnt.resize(n+1, 0);
		vcnt[n] += 1;
	}
	for (int i = 0; i != vcnt.size(); ++i) {
		if( vcnt[i] != 0 )
			cout << i << ": " << vcnt[i] << "\n";
	}
}
//	伸長法による盤面生成
void test_genLink3()
{
	const int N_LOOP = 100;
	const int WD = 8;
	cout << "genRandomLink3() x " << N_LOOP << "times:\n";
	Board2 b2(WD, WD);
	BoardK bk(WD, WD);
	vector<int> vcnt;
	int failcount = 0;
	for (int i = 0; i < N_LOOP;) {
		b2.genRandomLink3(WD-1);		//	伸張法による盤面生成
		if( b2.isFilled() &&			//	リンクが全セルを通っているか？
			!b2.isAnsCrowded() )		//	同じ数字が４つ密集している箇所があるか？
		{
			++i;
			g_qstr = b2.text0();
			bk.setNumbers(g_qstr.c_str());
			int n = bk.numSolutions();
			if( n >= vcnt.size() )
				vcnt.resize(n+1, 0);
			vcnt[n] += 1;
		} else
			++failcount;
	}
	cout << "failcount = " << failcount << "\n";
	for (int i = 0; i != vcnt.size(); ++i) {
		if( vcnt[i] != 0 )
			cout << i << ": " << vcnt[i] << "\n";
	}
}
void test_BoardK()
{
#if	0
	BoardK bd(4, 4);
	bd.setNumbers("1231.....23.....");
#endif
	BoardK bd(5, 5);
	bd.setNumbers("...14.2.....3..1.42.3....");
#if	0
	BoardK bd(7, 7);
	bd.setNumbers("....321""....1.."".......""..2....""......."".35..4.""4.....5");		//	7x7
#endif
	cout << bd.text() << "\n";
	cout << bd.mate() << "\n";
	bd.doSolveBT();
	cout << bd.textnl() << "\n";
}
