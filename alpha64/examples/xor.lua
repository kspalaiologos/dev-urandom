
transform(to_buffer(input_buffer), function(byte)
    return bit.bxor(byte, 42)
end)

return input_buffer
