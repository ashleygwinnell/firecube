--fboTest
class 'MyApp' (Application)
function MyApp:__init() Application.__init(self) end
function MyApp:Init()
	Application.AddSearchPath("../Media/Textures")
	Application.AddSearchPath("../Media/Models")
	self:SetTitle("Lua FBO Example")
	self.font=Renderer.GetFontManager():Create("c:\\windows\\fonts\\arial.ttf",10)
	self.mm=ModelManager()
	self.model=self.mm:Create("1.3ds");
	self.program,self.program2=Program(),Program()
	self.vBuffer,self.uvBuffer=Buffer(),Buffer()
	self.program:Create(Renderer.GetShaderManager():Create("1.vshader"),Renderer.GetShaderManager():Create("1.fshader"))
	Renderer.UseProgram(self.program)
	self.program:SetUniformi("tex",0)
	self.model:SetProgram(self.program)
	local vb,uvb=vector_vec3(),vector_vec2()
	vb:push_back(vec3(-1,1,0))
	vb:push_back(vec3(-1,-1,0))
	vb:push_back(vec3(1,-1,0))
	vb:push_back(vec3(1,1,0))
	vb:push_back(vec3(-1,1,0))
	vb:push_back(vec3(1,1,0))
	vb:push_back(vec3(1,-1,0))
	vb:push_back(vec3(-1,-1,0))

	uvb:push_back(vec2(0,1))
	uvb:push_back(vec2(0,0))
	uvb:push_back(vec2(1,0))
	uvb:push_back(vec2(1,1))
	uvb:push_back(vec2(0,1))
	uvb:push_back(vec2(1,1))
	uvb:push_back(vec2(1,0))
	uvb:push_back(vec2(0,0))
	self.vBuffer:Create()
	self.vBuffer:LoadData(vb,BufferType.STATIC)
	self.uvBuffer:Create()
	self.uvBuffer:LoadData(uvb,BufferType.STATIC)
	self.program2:Create(Renderer.GetShaderManager():Create("v.vshader"),Renderer.GetShaderManager():Create("p.fshader"))
	self.model2=self.mm:Create("teapot2.3ds")
	self.model2:SetProgram(self.program2)
	self.fbo=FrameBuffer()
	self.fbo:Create(128,128)
	self.fbo:AddDepthBuffer()
	self.fbo:AddRenderTarget(0)
	self.fbo:GetRenderTarget(0):GenerateMipMaps()
	self.ang=0
	if self.fbo:IsValid()==false then
		Logger.Write("Error: couldn't create FBO.\n")
		return false
	end
	return true
end

function MyApp:Render(time)
	local t,t2=mat4(),mat4()
	t:Translate(vec3(1.3,0,-3))
	t:RotateX(self.ang)
	t:RotateY(self.ang)
	t2:Translate(vec3(-1.3,0,-3))
	t2:RotateX(self.ang)
	t2:RotateY(self.ang)
	Renderer.UseFrameBuffer(self.fbo)
	Renderer.Clear(vec4(0.2,0.4,0.4,1.0),1.0)
	Renderer.SetPerspectiveProjection(90.0,0.1,100)
	Renderer.SetModelViewMatrix(t)
	Renderer.Render(self.model)
	Renderer.SetModelViewMatrix(t2)
	Renderer.Render(self.model2)
	Renderer.SetOrthographicProjection()
	Renderer.SetModelViewMatrix(mat4())
	Renderer.RenderText(self.font,vec3(0,0,0),vec4(1,1,1,1),"FBO Test.")

	Renderer.RestoreFrameBuffer()
	Renderer.SetViewport(0,0,self:GetWidth(),self:GetHeight())
	t:Identity()
	t:Translate(vec3(0,0,-2.5))
	t:RotateY(self.ang/3.0)
	Renderer.SetModelViewMatrix(t)
	Renderer.Clear(vec4(0.4,0.4,0.4,1.0),1.0)
	Renderer.SetPerspectiveProjection(90,0.1,100)
	Renderer.UseProgram(self.program)
	self.vBuffer:SetVertexStream(3)
	self.uvBuffer:SetTexCoordStream(0)
	self.fbo:GetRenderTarget(0):GenerateMipMaps()
	Renderer.UseTexture(self.fbo:GetRenderTarget(0),0)
	Renderer.RenderStream(RenderMode.QUADS,8)

	Renderer.SetOrthographicProjection()
	Renderer.SetModelViewMatrix(mat4())
	local str="FPS:" .. string.format("%.1f",self:GetFps())
	Renderer.RenderText(self.font,vec3(0,0,0),vec4(1,1,1,1),str)
end
function MyApp:Update(time)
	self.ang=self.ang+time*1.5
end
function MyApp:HandleInput(time)
end
app=MyApp()
if app:Initialize()==true then
	app:Run()
end


