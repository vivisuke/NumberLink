#include <iostream>
#include "Board.h"
#include <random>

using namespace std;

typedef unsigned char uchar;
typedef const char cchar;

extern mt19937 g_mt;

cchar *q2x2 =	"11"
				"..";
cchar *q4x4 =	"3..3"
				"24.."
				".1.."
				".214";
cchar *q4x4_2 =	"1..."
				"...."
				"...."
				"1...";
cchar *q4x4_3 =	"1..."
				"...."
				"...."
				"...1";
cchar *q5x5 =	"32..4"
				"3...."
				"4...."
				"21.1."
				".....";
cchar *q5x5_2 =	"5.54."
				"1.1.."
				"3.4.2"
				"6..2."
				".63..";
cchar *q6x6 =	"3....."
				".4.31."
				"......"
				".2...."
				".1.4.2"
				"......";
cchar *q6x6_2 =	".3..2."
				"......"
				".1.3.."
				".2...."
				"......"
				".....1";
cchar *q6x6_3 =	"2...3."
				"......"
				"...11."
				"3....."
				"......"
				"2.....";

int main(int argc, char *argv[])
{
	int WD = 6;
	int NP = 5;		//	
	if( argc >= 2 ) {
		WD = atoi(argv[1]);
		NP = atoi(argv[2]);
	}
	cout << "WD = " << WD << ", NP = " << NP << "\n\n";
#if	0
	cchar *ptr;
#if	0
	Board bd(2); 
	bd.setNumber(ptr = q2x2);
#endif
#if	0
	Board bd(4); 
	bd.setNumber(ptr = q4x4);
#endif
#if	1
	Board bd(5); 
	bd.setNumber(ptr = q5x5_2);
#endif
#if	0
	Board bd(6); 
	bd.setNumber(ptr = q6x6_3);
#endif
	cout << bd.text() << "\n\n";
	bool rc = bd.doSolve2();
	if (rc)
		cout << "found.\n";
	else
		cout << "failed.\n";
	cout << bd.text2() << "\n";
	
	if( rc ) {
		bd.setNumber(ptr);
		if( bd.isUniq() ) {
			cout << "uniq.\n";
			cout << bd.textLink2() << "\n";
			if( bd.doesIncludeMeaninglessLink() ) {
				cout << "Meaningless included.\n";
			} else {
				cout << "Meaningless Not included.\n";
			}
		} else
			cout << "Not uniq.\n";
		cout << bd.text() << "\n\n";
	}
#endif
#if	0
	Board bd(4); 
	for (int ix1 = 0; ix1 < bd.size(); ++ix1) {
		if( bd.number(ix1) != 0 ) continue;
		bd.setNumber(ix1, 1);
		for (int ix12 = ix1 + 2; ix12 < bd.size(); ++ix12) {
			if( bd.number(ix12) != 0 ) continue;
			if( ix12 > bd.width() && bd.number(ix12-bd.width()) == 1 ) continue;
			bd.setNumber(ix12, 1);
			for (int ix2 = ix1 + 1; ix2 < bd.size(); ++ix2) {
				if( bd.number(ix2) != 0 ) continue;
				bd.setNumber(ix2, 2);
				for (int ix22 = ix2 + 2; ix22 < bd.size(); ++ix22) {
					if( bd.number(ix22) != 0 ) continue;
					if( ix22 > bd.width() && bd.number(ix22-bd.width()) == 2 ) continue;
					bd.setNumber(ix22, 2);
					for (int ix3 = ix2 + 1; ix3 < bd.size(); ++ix3) {
						if( bd.number(ix3) != 0 ) continue;
						bd.setNumber(ix3, 3);
						for (int ix32 = ix3 + 2; ix32 < bd.size(); ++ix32) {
							if( bd.number(ix32) != 0 ) continue;
							if( ix32 > bd.width() && bd.number(ix32-bd.width()) == 3 ) continue;
							bd.setNumber(ix32, 3);
							if( bd.isUniq() ) {
								cout << bd.text() << "\n\n";
								getchar();
							}
							bd.setNumber(ix32, 0);
						}
						bd.setNumber(ix3, 0);
					}
					bd.setNumber(ix22, 0);
				}
				bd.setNumber(ix2, 0);
			}
			bd.setNumber(ix12, 0);
		}
		bd.setNumber(ix1, 0);
	}
#endif

	Board bd(/*wd=*/WD); 
	int cnt[] = {0, 0, 0, 0};
	for (int i = 0; i < 10000; ++i) {
		bd.setRandom(/*NP=*/NP);
		bd.isUniq();
		int n = bd.nSolved();
		cnt[n] += 1;
		if( n == 1 ) {
			cout << bd.text() << "\n\n";
			if( !bd.doesIncludeMeaninglessLink() )
				cnt[3] += 1;
		}
	}
	for(auto c : cnt) {
		cout << c << "\n";
	}
	
#if	0
	Board bd(/*wd=*/WD); 
	for (int i = 0; i < 30; ++i) {
		bd.genRandom(/*NP=*/NP);
		cout << bd.text0() << "\n";
	}
#endif
#if	0
	Board bd(/*wd=*/WD); 
	vector<int> pos;	//	リンク伸張可能な端点リスト
	for (int i = 0; i < NP; ++i) {
		for(;;) {
			int ix = g_mt() % bd.size();
			if( bd.number(ix) == 0 ) {
				bd.setNumber(ix, i+1);
				pos.push_back(ix);
				break;
			}
		}
	}
	cout << bd.text2() << "\n\n";
	vector<uchar> t;		//	伸張可能先リスト
	t.reserve(4);
	while( !pos.empty() ) {
		int i = g_mt() % pos.size();
		int ix = pos[i];
		int x = ix % WD;
		int y = ix / WD;
		t.clear();
		if( y != 0 && bd.number(ix - WD) == 0 )
			t.push_back(LINK_UP);
		if( x != 0 && bd.number(ix - 1) == 0 )
			t.push_back(LINK_LEFT);
		if( x != WD - 1 && bd.number(ix + 1) == 0 )
			t.push_back(LINK_RIGHT);
		if( y != WD - 1 && bd.number(ix + WD) == 0 )
			t.push_back(LINK_DOWN);
		if( t.empty() ) {		//	伸張不可能だった場合
			pos.erase(pos.begin() + i);
		} else {
			int n = bd.number(ix);
			auto dir = t[g_mt() % t.size()];
			int ix2;
			for (;;) {
				switch( dir ) {
				case LINK_UP:
					ix2 = ix - WD;
					--y;
					break;
				case LINK_LEFT:
					ix2 = ix - 1;
					--x;
					break;
				case LINK_RIGHT:
					ix2 = ix + 1;
					++x;
					break;
				case LINK_DOWN:
					ix2 = ix + WD;
					++y;
					break;
				}
				if( x < 0 || x >= WD || y < 0 || y >= WD ||
					bd.number(ix2) != 0 )
				{
					break;
				}
				bd.orLink(ix, dir);
				ix = ix2;
				bd.orLink(ix, revDir(dir));
				bd.setNumber(ix, n);
				pos[i] = ix;
			}
			cout << bd.text2() << "\n\n";
		}
	}
#endif
	return 0;
}
