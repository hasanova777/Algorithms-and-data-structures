#include <iostream>
#include <vector>

using namespace std;

void print_prime_numbers (int n) {
	vector<int> primes;
	int k = 0;
	int p = 3;
	primes.push_back(2);
	while (k < n) {
		bool prime = true;
		for (int i : primes) {
			if (p % i == 0) {
				prime = false;
				break;
			}
		}
		if (prime) {
			primes.push_back(p);
			cout << primes[k] << " ";
			k++;
		}
		p++;
	}
	primes.~vector();
}

int main() {
	int n;
	cin >> n;
	print_prime_numbers(n);
}
