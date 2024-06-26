local load = load
local string = string
local table = table
local cEqual = string.byte('=', 1)

local function do_precompile(code)
    local c = {"local __JASS__={} "}
    local startPos
    local endPos = -1
    while true do
        startPos = code:find('<?', endPos + 2, true)
        if not startPos then
            break
        end
        c[#c+1] = ("__JASS__[#__JASS__+1]=%q "):format(code:sub(endPos + 2, startPos - 1))

        endPos = code:find('?>', startPos + 2, true)
        if not endPos then
            endPos = startPos
            break
        end
        if code:byte(startPos + 2) ~= cEqual then
            c[#c+1] = ("%s "):format(code:sub(startPos + 2, endPos - 1))
        else
            c[#c+1] = ("__JASS__[#__JASS__+1]=%s "):format(code:sub(startPos + 3, endPos - 1)) 
        end
    end
    c[#c+1] = ("__JASS__[#__JASS__+1]=%q "):format(code:sub(endPos + 2))
    c[#c+1] = "return table.concat(__JASS__)"
    return table.concat(c)
end

local function do_compile(code, env)
	local ok, res = pcall(do_precompile, code)
	if not ok then
		return false, res
	end
	local f, err = load(res, '@4_wave.j', 't', env)
	if not f then
		return f, err
	end
	local ok, res = xpcall(f, debug.traceback)
	if not ok then
		local pos = res:find("[C]: in function 'xpcall'", 1, true)
		if pos then
			res = res:sub(1, pos-1)
		end
    	return false, res
	end
    return true, res
end

local template = {}

function template:compile(op)
	log.trace("Template compilation start.")
	local code, err = io.load(op.input)
	if not code then
		log.error("Template read " .. op.input .. ". Error: " .. err)
		return false
	end
	__map_path__   = op.map_path
	local env = require 'compiler.computed.env'
	local ok, res = do_compile(code, env)
	if not ok then
		if res then
			gui.error_message(nil, res)
		else
			gui.error_message(nil, LNG.UNKNOWN)
		end
		log.error("Template error processing: " .. tostring(res))
		return false
	end

	local ok, err = io.save(op.output, res)
	if not ok then
		log.error("Template write " .. op.output .. ". Error: " .. err)
		return false
	end
	
	log.debug("Template compilation succeeded.")
	return true
end

return template
