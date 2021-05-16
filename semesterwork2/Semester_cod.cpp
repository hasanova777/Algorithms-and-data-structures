/* Примечание:

   Данная реализация предполагает, что в выпуклом многоугольнике отсутствуют
   повторяющиеся вершины, а площадь многоугольника ненулевая, так как нет не-
   обходмости проверять этот случай. Если точка лежит на границе многоуголь-
   ника, то считается, что точка принадлежит этому выпуклому многоугольнику.

   Задача:

   Дан выпуклый многоугольник с n вершинами, координаты всех вершин целочис-
   ленные; вершины заданы в порядке обхода против часовой стрелки (в против-
   ном случае нужно просто отсортировать их). Поступает запрос – точка, и 
   требуется для этой точки определить, лежит она внутри этого многоуголь-
   ника или нет. 

   Входные данные:

   В первой строке задано число n (3 ≤ n ≤ 10^5). Далее в n строках заданы 
   по паре чисел - координаты очередной вершины выпуклого многоугольника в
   порядке обхода или против часовой стрелки. В n+1 строке записаны коорди-
   наты рассматриваемой точки.

   Выходные данные:

   Вывести "YES", если заданная точка принадлежит выпуклому многоугольнику
   Вывести "NO", если заданная точка не принадлежит выпуклому многоугольнику

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

bool in = false;

struct point {
	long long x, y;
};

struct angle {
	long long a, b;
};

bool operator < (const angle& p, const angle& query) {
	if (p.b == 0 && query.b == 0)
		return p.a < query.a;
	return p.a * 1ll * query.b < p.b * 1ll * query.a;
}

long long square(point& a, point& b, point& c) {
	return a.x * 1ll * (b.y - c.y) + b.x * 1ll * (c.y - a.y) + c.x * 1ll * (a.y - b.y);
}

void tests() {
	long n;
	cout << "Enter n: ";
	cin >> n;
	vector<point> p(n);
	long zero_id = 0;
	for (int j = 0; j < n; ++j) {
		scanf_s("%d%d", &p[j].x, &p[j].y);
		if (p[j].x < p[zero_id].x || p[j].x == p[zero_id].x && p[j].y < p[zero_id].y) {
			zero_id = j;
		}
	}
	point zero_value = p[zero_id];
	rotate(p.begin(), p.begin() + zero_id, p.end());
	p.erase(p.begin());
	--n;

	vector<angle> a(n);
	for (int k = 0; k < n; ++k) {
		a[k].a = p[k].y - zero_value.y;
		a[k].b = p[k].x - zero_value.x;
		if (a[k].a == 0) {
			a[k].b = a[k].b < 0 ? -1 : 1;
		}
	}

	point query;
	cout << "Enter the coordinates of the point: ";
	cin >> query.x >> query.y;
	in = false;
	if (query.x >= zero_value.x) {
		if (query.x == zero_value.x && query.y == zero_value.y) {
			in = true;
		}
		else {
			angle my = { query.y - zero_value.y, query.x - zero_value.x };
			if (my.a == 0) {
				my.b = my.b < 0 ? -1 : 1;
			}
			vector<angle>::iterator it = upper_bound(a.begin(), a.end(), my);
			if (it == a.end() && my.a == a[n - 1].a && my.b == a[n - 1].b) {
				it = a.end() - 1;
			}
			if (it != a.end() && it != a.begin()) {
				long long p1 = int(it - a.begin());
				if (square(p[p1], p[p1 - 1], query) <= 0) {
					in = true;
				}
			}
		}
	}
}

int main() {

	for (int i = 1; i <= 50; i++) {
		
		cout << "Test #" << i << endl;

		auto startMain = high_resolution_clock::now();
		tests();
		auto stopMain = high_resolution_clock::now();

		if (in) {
			cout << "YES";
		}
		else {
			cout << "NO";
		}
		cout << "" << endl;

		cout << "Time taken by algorithm: "
			<< duration_cast<milliseconds>(stopMain - startMain).count() << endl << endl;
	}
}
