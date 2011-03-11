t=
{
  {0,10}, --10
  {10,20}, --10
  {20,30}, --10
  {30,50},--20
  {50,60},--10
  {60,90},--30
  {90,110},--30
}

function _bsearch(data,left,right,val)
  local iters = 0
  while left <= right do
    iters = iters + 1
    local mid = left + math.floor((right - left) / 2)
    local cell = data[mid]
    if (val >= cell[1]) and (val < cell[2]) then
        return mid,iters
    elseif val >= cell[2] then
        left = mid +1
    else
        right = mid -1
    end
  end
  return nil
end

function bsearch(data,val)
 return _bsearch(data,1,#data,val)
end

function search(data, val)
  local i,iters = bsearch(data,val)
  local s = ""
  if i ~= nil then
    s = "( "..data[i][1].." "..data[i][2].." ) ["..iters.."]"
  end
  print("==> "..val.." in cell "..tostring(bsearch(data,val)).." "..s)
end

search(t,10)
search(t,0)
search(t,-1)
search(t,1234)
search(t,40)
search(t,45)
search(t,78)
search(t,93)
search(t,109)
search(t,110)

