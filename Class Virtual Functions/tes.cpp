
class someclass {
	public:
		virtual  void func();
};
class anotherclass {
	public:
		void func();
}
void anotherclass::func() {
	cout << "This is in my derived class \n";
}
int main() {
	someclass a;
	anotherclass b;
 
	a.func();
	b.func();
	return 0;
}// end of main

