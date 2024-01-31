local event = require 'ev'
local ffi = require 'ffi'
ffi.cdef[[
	int MessageBoxW(unsigned int hWnd, const wchar_t* lpText, const wchar_t* lpCaption, unsigned int uType);
]]
local uni = require 'ffi.unicode'
local report = {}
report.INFO = ""
report.WARN = ""
report.ERROR = ""
local e
e = event.on('编译地图', function (success)
    package.loaded['report'] = nil
    e:remove()
    log.info('Flushed report')
    if not success then
        return
    end
	local s = ""
	if #report.INFO > 0 then
		s = s .. "INFO:\n" .. report.INFO .. "\n"
	end
	if #report.WARN > 0 then
		s = s .. "WARN:\n" .. report.WARN .. "\n"
	end
	if #report.ERROR > 0 then
		s = s .. "ERROR:\n" .. report.ERROR .. "\n"
	end
	if s ~= "" then
		ffi.C.MessageBoxW(nil, uni.u2w(s), uni.u2w('YDWE'), 32)
		log.info(s)
	end
end)
function report.info(s)
	report.INFO = report.INFO .. s .. "\n"
end
function report.warn(s)
	report.WARN = report.WARN .. s .. "\n"
end
function report.error(s)
	report.ERROR = report.ERROR .. s .. "\n"
end
return report