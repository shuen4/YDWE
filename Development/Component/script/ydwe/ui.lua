
local function is_enable_japi()
	local ok, japi, x_japi = pcall(function ()
		local tbl = sys.ini_load(fs.ydwe_path() / 'plugin' / 'warcraft3' / 'config.cfg')
		return tbl['Enable']['yd_jass_api.dll'] ~= '0', tbl['Enable']['X_JAPI.dll'] ~= '0'
	end)
	if not ok then 
		return true, true
	end
	return japi, x_japi
end

local function get_ui()
	local ydwe = fs.ydwe_devpath()
	local list = {}
	local f, err = io.open(ydwe / 'ui' / 'config', 'r')
	if not f then
		log.error('Open ' .. (ydwe / 'ui' / 'config'):string() .. ' failed.')
		return list
	end
	local enable_japi, enable_x_japi = is_enable_japi()
	for line in f:lines() do
		line = line:gsub("^%s*(.-)%s*$", "%1")
		if (not enable_japi and line == 'japi') or (not enable_x_japi and line == 'X JAPI') then
			-- do nothing
		elseif line:sub(1, 1) == '#' then
			-- ignore
		else
			table.insert(list, ydwe / 'ui' / line)
		end
	end
	f:close()
	return list
end

local list = get_ui()
return list
