
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

// -------------------------------------
class abstract {
	public:
		virtual void pure_v_func() = 0;
};
class derived: public abstract {
	public:
		void pure_v_func();
};
void derived::pure_v_func() {
	cout << "See how this works";
}
int main ()  {
	derived myclass;
	myclass.pure_v_func();

	return 0;
}
