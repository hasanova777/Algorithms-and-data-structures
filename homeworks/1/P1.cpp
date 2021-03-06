#include <iostream>
using namespace std;

struct elem {
	int val;
	elem* next;
};

elem* add(elem* head, int x) {
	elem* p = new elem;
	(*p).val = x;
	p->next = head;
	return p;
}

void print_list(elem* head) {
	elem* p = head;
	while (p != NULL) {
		cout << p->val << " ";
		p = p->next;
	}
	cout << endl;
}

elem* empty_list() {
	return NULL;
}

bool isExist(elem* el, int n) {
	elem* p = el;
	while (p != NULL) {
		if (n % p->val == 0) {
			return true;
		}
		p = p->next;
	}
	return false;
}

elem* expand(elem* li) {
	elem* new_list = empty_list();
	while (li != NULL) {
		new_list = add(new_list, li->val);
		li = li->next;
	}
	return new_list;
}

int main() {
	int n, count = 1;
	cin >> n;
	if (n > 0) {
		elem* list = empty_list();
		int i = 2;
		list = add(list, i);
		while (count < n) {
			i++;
			if (!isExist(list, i)) {
				list = add(list, i);
				count++;
			}
		}
		list = expand(list);
		print_list(list);
	}
	return 0;
}
