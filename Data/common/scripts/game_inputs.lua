local scene1 = Scene()
local root = scene1:FindObject( "root" )

color3d = Effect( "color3d", "EffectColor.effect" )
borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )


local proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1200 )

-- Add camera...
camera = scene1:NewObject( "camera" )
cameraComponent = CameraComponent()
camera:Attach( cameraComponent )
cameraComponent:SetProjection( proj )
camera:Transform():SetPosition( V3( 0, 5, -17 ) )
camera:Transform():LookAt( V3() )	

local cube = scene1:NewObject( "cube" )
cube:AddGeometry( Geometry( "cube", "ShapeCube.shape" ) )
cube:Transform():SetPosition( V3( -4.5, 3, 0 ) )
cube:AddScript( "rotate", "lua", "rotatex.lua" )

local pointfield = scene1:NewObject( "pointfield" )
pointfield:AddGeometry( Geometry( "pointfield", "ShapePointField.shape" ) )
pointfield:Transform():SetPosition( V3( -1.5, 3, 0 ) )
pointfield:AddScript( "rotate", "lua", "rotatey.lua" )
	
local pointring = scene1:NewObject( "pointring" )
pointring:AddGeometry( Geometry( "pointring", "ShapePointRing.shape" ) )
pointring:Transform():SetPosition( V3( 1.5, 3, 0 ) )
pointring:AddScript( "rotate", "lua", "rotatez.lua" )

local dashring = scene1:NewObject( "dashring" )
dashring:AddGeometry( Geometry( "dashring", "ShapeDashRing.shape" ) )
dashring:Transform():SetPosition( V3( 4.5, 3, 0 ) )
dashring:AddScript( "rotate", "lua", "rotatex.lua" )

local pyramid = scene1:NewObject( "pyramid" )
pyramid:AddGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
pyramid:Transform():SetPosition( V3( -4.5, 0, 0 ) )
pyramid:AddScript( "rotate", "lua", "rotatey.lua" )
	
local circle = scene1:NewObject( "circle" )
circle:AddGeometry( Geometry( "circle", "ShapeCircle.shape" ) )
circle:Transform():SetPosition( V3( -1.5, 0, 0 ) )
circle:AddScript( "rotate", "lua", "rotatez.lua" )

local sphere = scene1:NewObject( "sphere" )
sphere:AddGeometry( Geometry( "sphere", "ShapeSphere.shape" ) )
sphere:Transform():SetPosition( V3( 1.5, 0, 0 ) )
sphere:AddScript( "rotate", "lua", "rotatex.lua" )
   
local cylinder = scene1:NewObject( "cylinder" )
cylinder:AddGeometry( Geometry( "cylinder", "ShapeCylinder.shape" ) )
cylinder:Transform():SetPosition( V3( 4.5, 0, 0 ) )
cylinder:AddScript( "rotate", "lua", "rotatey.lua" )

local tube = scene1:NewObject( "tube" )
tube:AddGeometry( Geometry( "tube", "ShapeTube.shape" ) )
tube:Transform():SetPosition( V3( -4.5, -3, 0 ) )
tube:AddScript( "rotate", "lua", "rotatez.lua" )
	
local plane = scene1:NewObject( "plane" )
plane:AddGeometry( Geometry( "plane", "ShapePlane.shape" ) )
plane:Transform():SetPosition( V3( -1.5, -3, 0 ) )
plane:AddScript( "rotate", "lua", "rotatex.lua" )
	
cone = scene1:NewObject( "cone" )
cone:AddGeometry( Geometry( "cone", "ShapeCone.shape" ) )
cone:Transform():SetPosition( V3( 1.5, -3, 0 ) )
cone:AddScript( "rotate", "lua", "rotatey.lua" )
print( "cone is " .. type(cone) )
