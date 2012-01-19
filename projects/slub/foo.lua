--[[
Copyright (c) 2011 Timo Boll, Tony Kostanjsek

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
]]

local f = foo(111)
print(f)

local f2 = foo(111, 112)
print(f2)

f.bar = 1337
f2.bar = 1337

print(f.bar)
print(f2.bar)

f:doStuff()
f:doStuff(10)

function foo:luaMethod(arg)
  print(self.msg)
  print(arg)
end

print(f.luaMethod)
print(foo.luaMethod)

f2.msg = 'object message'
f2:luaMethod('hai from lua')

print(tostring(f))
print(tostring(f2))

print(f + 27)
print(f - 53)
print(f * 2)
print(f / 10)
print(f % 2)
print(f ^ 0)
print(-f)
print(f == f2)
print(f2 == f)
print(f < f2)
print(f2 < f)
print(f <= f2)
print(f2 <= f)

f2.bar = 1338
print(tostring(f))
print(tostring(f2))

print(f == f2)
print(f2 == f)
print(f < f2)
print(f2 < f)
print(f <= f2)
print(f2 <= f)

local f2_copy = f:getFoo(f2)
print(tostring(f2_copy))
print(f2 == f2_copy)
f2_copy:doStuff()

f2.f = f2_copy
print(tostring(f2.f))
print(f2.f == f2_copy)
f2.f:doStuff()
