local FourCC = {}
function FourCC.string2hex(s)
    return "0x" .. string.format("%X", (string.unpack(">I4", s)))
end
return FourCC