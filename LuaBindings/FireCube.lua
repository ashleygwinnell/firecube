do
	local initlib = package.loadlib("LuaFireCube.dll","luaopen_LuaFireCube")
	if (initlib) then
		initlib()
	else
		error("Error loading FireCube.")
	end
end
