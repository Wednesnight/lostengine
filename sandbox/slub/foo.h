struct foo {
  
  int bar;
  foo* f;
  
  foo() {
    std::cout << "foo:ctor" << std::endl;
    this->bar = 111;
  }
  
  ~foo() {
    std::cout << "foo:dtor" << std::endl;
  }

  foo* getFoo(foo* f) {
    return f;
  }

  int getBar() {
    return bar;
  }
  
  void doStuff() {
    std::cout << "foo::doStuff()" << std::endl;
  }

  void doStuff2(int i) {
    std::cout << "foo::doStuff(" << i << ")" << std::endl;
  }

  std::ostream& operator<<(std::ostream& s) {
    s << "foo: " << "bar = " << bar;
    return s;
  }
  
  int operator+(int i) {
    return bar + i;
  }
  
  int operator-(int i) {
    return bar - i;
  }
  
  int operator*(int i) {
    return bar * i;
  }
  
  int operator/(int i) {
    return bar / i;
  }
  
  int operator%(int i) {
    return bar % i;
  }
  
  int operator^(int i) {
    return bar ^ i;
  }
  
  int operator-() {
    return -bar;
  }
  
  bool operator==(const foo& f) {
    return bar == f.bar;
  }
  
  bool operator<(const foo& f) {
    return bar < f.bar;
  }
  
  bool operator<=(const foo& f) {
    return bar <= f.bar;
  }
  
};
