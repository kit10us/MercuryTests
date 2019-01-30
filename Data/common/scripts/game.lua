local scene1 = Scene()

local proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )

-- Add camera...
local camera = scene1:NewObject( "camera" )
local cameraComponent = CameraComponent()
camera:Attach( cameraComponent )
cameraComponent:SetProjection( proj )
camera:Transform():SetPosition( V3( 0, 5, -17 ) )
camera:Transform():LookAt( V3( 0, -2, 0 ) )	

-- Add geometry...

local cube = scene1:NewObject( "cube" )
cube:AddGeometry( Geometry( "cube", "models/ShapeCube.shape" ) )
cube:Transform():SetPosition( V3( -4.5, 3, 0 ) )
cube:AddScript( "rotate", "lua", "scripts/rotatex.lua" )

local pointfield = scene1:NewObject( "pointfield" )
pointfield:AddGeometry( Geometry( "pointfield", "models/ShapePointField.shape" ) )
pointfield:Transform():SetPosition( V3( -1.5, 3, 0 ) )
pointfield:AddScript( "rotate", "lua", "scripts/rotatey.lua" )
	
local pointring = scene1:NewObject( "pointring" )
pointring:AddGeometry( Geometry( "pointring", "models/ShapePointRing.shape" ) )
pointring:Transform():SetPosition( V3( 1.5, 3, 0 ) )
pointring:AddScript( "rotate", "lua", "scripts/rotatez.lua" )

local dashring = scene1:NewObject( "dashring" )
dashring:AddGeometry( Geometry( "dashring", "models/ShapeDashRing.shape" ) )
dashring:Transform():SetPosition( V3( 4.5, 3, 0 ) )
dashring:AddScript( "rotate", "lua", "scripts/rotatex.lua" )

local pyramid = scene1:NewObject( "pyramid" )
pyramid:AddGeometry( Geometry( "pyramid", "models/ShapePyramid.shape" ) )
pyramid:Transform():SetPosition( V3( -4.5, 0, 0 ) )
pyramid:AddScript( "rotate", "lua", "scripts/rotatey.lua" )
	
local circle = scene1:NewObject( "circle" )
circle:AddGeometry( Geometry( "circle", "models/ShapeCircle.shape" ) )
circle:Transform():SetPosition( V3( -1.5, 0, 0 ) )
circle:AddScript( "rotate", "lua", "scripts/rotatez.lua" )

local sphere = scene1:NewObject( "sphere" )
sphere:AddGeometry( Geometry( "sphere", "models/ShapeSphere.shape" ) )
sphere:Transform():SetPosition( V3( 1.5, 0, 0 ) )
sphere:AddScript( "rotate", "lua", "scripts/rotatex.lua" )
   
local cylinder = scene1:NewObject( "cylinder" )
cylinder:AddGeometry( Geometry( "cylinder", "models/ShapeCylinder.shape" ) )
cylinder:Transform():SetPosition( V3( 4.5, 0, 0 ) )
cylinder:AddScript( "rotate", "lua", "scripts/rotatey.lua" )

local tube = scene1:NewObject( "tube" )
tube:AddGeometry( Geometry( "tube", "models/ShapeTube.shape" ) )
tube:Transform():SetPosition( V3( -4.5, -3, 0 ) )
tube:AddScript( "rotate", "lua", "scripts/rotatez.lua" )
	
local plane = scene1:NewObject( "plane" )
plane:AddGeometry( Geometry( "plane", "models/ShapePlane.shape" ) )
plane:Transform():SetPosition( V3( -1.5, -3, 0 ) )
plane:AddScript( "rotate", "lua", "scripts/rotatex.lua" )
	
local cone = scene1:NewObject( "cone" )
cone:AddGeometry( Geometry( "cone", "models/ShapeCone.shape" ) )
cone:Transform():SetPosition( V3( 1.5, -3, 0 ) )
cone:AddScript( "rotate", "lua", "scripts/rotatey.lua" )


local terraParams = TerraParameters()
terraParams:SetEffect( Effect( "pit_image", "EffectPit.effect" ) )
terraParams:SetSize( Size2( 20, 20 ) )
terraParams:SetConstant( 0 )
terraParams:SetFaces( 20, 20 )
terraParams:SetHeightMap( Texture( "test256", "pit.bmp", true, true ), Grey( 1 ) )

local terraGeo = Terra( terraParams )
local terra = scene1:NewObject( "terra" )
terra:AddGeometry( terraGeo )
terra:Transform():SetPosition( V3( 0, -7, 0 ) );

Game.Command( "AddFPS" )

-- Add camera orbit motivator...
local cameraMotivator = ObjectOrbitMotivator( V3( 0, 0, 0 ), V3( 0, 1, 0 ), Angle.Degrees( 45 ) )
cameraMotivator:AttachTo( camera );	
