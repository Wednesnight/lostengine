Field =
{
  player = -1
}
Field_mt = { __index = Field }

function Field()
  local result = {}
  setmetatable(result, Field_mt)
  return result
end

game =
{
  currentPlayer = 0,
  board =
  {
    [0] =
    {
      [0] = Field(),
      [1] = Field(),
      [2] = Field()
    },
    [1] =
    {
      [0] = Field(),
      [1] = Field(),
      [2] = Field()
    },
    [2] =
    {
      [0] = Field(),
      [1] = Field(),
      [2] = Field()
    }
  }
}
