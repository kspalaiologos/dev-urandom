
-- An example stationary order-0 model.

local ctx = 1
local t = { }

for i = 0, 512 do
    t[i] = { }
    t[i][0] = 0
    t[i][1] = 0
end

function predictor()
    return math.floor(4096*(t[ctx][1]+1)/(t[ctx][0]+t[ctx][1]+2))
end

function update(bit)
    t[ctx][bit] = t[ctx][bit] + 1
    if t[ctx][bit] > 65534 then
        t[ctx][0] = bit32.rshift(t[ctx][0], 1)
        t[ctx][1] = bit32.rshift(t[ctx][1], 1)
    end
    ctx = 2 * ctx + bit
    if ctx >= 512 then
        ctx = 1
    end
end
