color3d = Effect( "color3d", "EffectColor.effect" )
borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )

scene1 = Scene()

proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth() / Game.GetHeight(), 1, 1000 )

-- Add camera...
camera = scene1:AddCamera( "camera", proj )	
camera:Transform():SetPosition( V3( 0, 5, -17 ) )
camera:Transform():LookAt( V3() )

scene1:SetSize( Game.GetWidth(), Game.GetHeight() )

cubeParameters = ShapeParameters( "cube" )
cubeParameters:SetEffect( color3d )
cubeParameters:SetSize3( Size3( 2, 2, 2 ) )
cubeParameters:SetDiffuseFaces( Red(), Green(), Blue(), RGB( 1, 1, 0 ), RGB( 0, 1, 1 ), RGB( 1, 0, 1 ) )
cube = scene1:NewObject( "cube" )
cube:AddGeometry( Geometry( cubeParameters ) )
cube:Transform():SetPosition( V3( -4.5, 3, 0 ) )


pointfieldParameters = ShapeParameters( "pointfield" )
pointfieldParameters:SetEffect( color3d )
pointfieldParameters:SetMinorRadius( 0.5 )
pointfieldParameters:SetMajorRadius( 1.0 )
pointfieldParameters:SetCount( 1000 )
pointfieldParameters:SetDiffuse( White() )
pointfield = scene1:NewObject( "pointfield" )
pointfield:AddGeometry( Geometry( pointfieldParameters ) )
pointfield:Transform():SetPosition( V3( -1.5, 3, 0 ) )

pointringParameters = ShapeParameters( "pointring" )
pointringParameters:SetEffect( color3d )
pointringParameters:SetMinorRadius( 0.25 )
pointringParameters:SetMajorRadius( 0.75 )
pointringParameters:SetCount( 1000 )
pointringParameters:SetDiffuse( Red() )
pointring = scene1:NewObject( "pointring" )
pointring:AddGeometry( Geometry( pointringParameters ) )
pointring:Transform():SetPosition( V3( 1.5, 3, 0 ) )

dashringParameters = ShapeParameters( "dashring" )
dashringParameters:SetEffect( color3d )
dashringParameters:SetMinorRadius( 0.5 )
dashringParameters:SetMajorRadius( 1.0 )
dashringParameters:SetSize( 0.5 )
dashringParameters:SetCount( 5 )
dashringParameters:SetDiffuse( Green() )
dashring = scene1:NewObject( "dashring" )
dashring:AddGeometry( Geometry( dashringParameters ) )
dashring:Transform():SetPosition( V3( 4.5, 3, 0 ) )	

pyramidParameters = ShapeParameters( "pyramid" )
pyramidParameters:SetEffect( color3d )
pyramidParameters:SetSize3( Size3( 2, 2, 2 ) )
pyramidParameters:SetDiffuse( RGB( 0, 1, 1 ) )
pyramid = scene1:NewObject( "pyramid" )
pyramid:AddGeometry( Geometry( pyramidParameters ) )
pyramid:Transform():SetPosition( V3( -4.5, 0, 0 ) )	

circleParameters = ShapeParameters( "circle" )
circleParameters:SetEffect( color3d );
circleParameters:SetSegments( 24 );
circleParameters:SetRadius( 1.0 );
circleParameters:SetDiffuse( Blue() );
circle = scene1:NewObject( "circle" )
circle:AddGeometry( Geometry( circleParameters ) );
circle:Transform():SetPosition( V3( -1.5, 0, 0 ) )

sphereParameters = ShapeParameters( "sphere" )
sphereParameters:SetEffect( color3d )
sphereParameters:SetSegments( 24 )
sphereParameters:SetRadius( 1.0 )
sphereParameters:SetDiffuse( RGB( 1, 1, 0 ) )
sphere = scene1:NewObject( "sphere" )
sphere:AddGeometry( Geometry( sphereParameters ) )
sphere:Transform():SetPosition( V3( 1.5, 0, 0 ) )

cylinderParameters = ShapeParameters( "cylinder" )
cylinderParameters:SetEffect( color3d )
cylinderParameters:SetSegments( 24 )
cylinderParameters:SetRadius( 1.0 )
cylinderParameters:SetHeight( 2.0 )
cylinderParameters:SetDiffuse( RGB(1, 0, 1 ) )
cylinderParameters:SetCaps( true )
cylinder = scene1:NewObject( "cylinder" )
cylinder:AddGeometry( Geometry( cylinderParameters ) )
cylinder:Transform():SetPosition( V3( 4.5, 0, 0 ) )

tubeParameters = ShapeParameters( "tube" )
tubeParameters:SetEffect( color3d )
tubeParameters:SetSegments( 24 )
tubeParameters:SetMajorRadius( 1.0 )
tubeParameters:SetMinorRadius( 0.5 )
tubeParameters:SetHeight( 2.0 )
tubeParameters:SetDiffuse( Red() )
tube = scene1:NewObject( "tube" )
tube:AddGeometry( Geometry( tubeParameters ) )
tube:Transform():SetPosition( V3( -4.5, -3, 0 ) )



planeParameters = ShapeParameters( "plane" )
planeParameters:SetEffect( color3d )
planeParameters:SetSegments( 2 )
planeParameters:SetSize2( Size2.New( 2.0, 2.0 ) )
planeParameters:SetDiffuse( RGB( 0, 1, 1 ) )
plane = scene1:NewObject( "plane" )
plane:AddGeometry( Geometry( planeParameters ) )
plane:Transform():SetPosition( V3( -1.5, -3, 0 ) )


coneParameters = ShapeParameters( "cone" )
coneParameters:SetEffect( borgcubeEffect )
coneParameters:SetSegments( 24 )
coneParameters:SetRadius( 1 )
coneParameters:SetHeight( 2 )
coneParameters:SetCaps( true )
cone = scene1:NewObject( "cone" )
cone:AddGeometry( Geometry( coneParameters ) )
cone:Transform():SetPosition( V3( 1.5, -3, 0 ) )
