function Entry(a)
  return a
end

function printtable(t)
  for k,v in pairs(t)
  do
    print(k.." : "..v)
  end
end

entry = Entry
{
  a = "suffa",
  13,
  [2] = 8237466,
  fuffzige = 50,
  "asldkjasldk",
  "gögö"
}

print(entry)
print("size: "..#entry)
print("---")
printtable(entry)
testing.greet()
r = testing.Rect(13)
print(r:val())
