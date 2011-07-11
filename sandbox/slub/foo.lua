local f = foo()
print(f)

local f2 = foo()
print(f2)

f.bar = 1337
f2.bar = 1337

print(f.bar)
print(f2.bar)

f:doStuff()

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
