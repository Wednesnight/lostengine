/*
The MIT License (MIT)
Copyright (c) 2011 Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

struct foo {
  
  int bar;

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
  
  baz* get() {
    return this;
  }
  
  void print(baz* b) {
    std::cout << "baz::print() -> " << b << std::endl;
  }

};
