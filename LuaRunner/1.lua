class 'MyApp' (Application)
function MyApp:__init() Application.__init(self) end
function MyApp:Init()
	Application.AddSearchPath("../Media/Textures")
	self.font=Renderer.GetFontManager():Create("c:\\windows\\fonts\\cour.ttf",14)
	self.program=Program()
	self.program:Create(Renderer.GetShaderManager():Create("../Test/plainColor.vshader"),Renderer.GetShaderManager():Create("../Test/plainColor.fshader"))
	self.m=Model()
	self.m:Load("../Media/Models/1.3ds")
	self.m:SetProgram(self.program)
	self.ang=0.0
	self:SetTitle("Lua example")
	local vcount,fcount=0,0
	for i=0,self.m.object:size()-1 do
		vcount=vcount+self.m.object:at(i).vertex:size()
		for j=0,self.m.object:at(i).mesh:size()-1 do
			fcount=fcount+self.m.object:at(i).mesh:at(j).face:size()
		end
	end
	print("Number of vertices: " .. vcount)
	print("Number of faces   : " .. fcount)
	print("Material list: ")
	for i=0,self.m.material:size()-1 do
		print("\t"..self.m.material:at(i).name)
	end
	return true
end

function MyApp:Render(time)
	Renderer.Clear(vec4(0.7,0.7,0.7,1.0),1.0)
	Renderer.SetPerspectiveProjection(90,1,1000)
	local t=mat4();
	t:Translate(vec3(0,0,-5))
	t:RotateY(self.ang)
	Renderer.SetModelViewMatrix(t)
	Renderer.Render(self.m)
	Renderer.SetOrthographicProjection()
	local i=mat4()
	Renderer.SetModelViewMatrix(i)
	Renderer.RenderText(self.font,vec3(0,0,0),vec4(1,1,1,1),"Hello from Lua!")
	local str="FPS:" .. string.format("%.1f",self:GetFps())
	Renderer.RenderText(self.font,vec3(0,20,0),vec4(1,1,0,1),str)
end
function MyApp:Update(time)
	self.ang=self.ang+time;
end
function MyApp:HandleInput(time)
end
app=MyApp()
if app:Initialize()==true then
	app:Run()
end



