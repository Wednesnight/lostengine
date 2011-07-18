struct foo {
  
  int bar;
  foo* f;

  foo(int i) {
    std::cout << "foo:ctor" << std::endl;
    this->bar = i;
  }
  
  foo(int i, int j) {
    std::cout << "foo:ctor2" << std::endl;
    this->bar = j;
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

  void doStuff(int i) {
    std::cout << "foo::doStuff(" << i << ")" << std::endl;
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

std::ostream& operator<<(std::ostream& s, const foo& f) {
  s << "foo: " << "bar = " << f.bar;
  return s;
}

struct baz : public foo {
  baz(int i, int j) : foo(i, j) {
  }
};
