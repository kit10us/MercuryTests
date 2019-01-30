
--[[

double game.getwidth()
double game.getheight()
double game.getaspectratiohw()

resources.add( type, name, path )

scenes.load( xmlfile )
scenes.add( name )

scene.addobject( scene, name )
scene.setcamera( scene, object )
scene.setsize( scene, width, height )

object.setgeometry( scene:object, geometry )
object.setposition( scene:object, x, y, z )
object.lookat( scene:object, x, y, z )

camera.setprojection( scene, matrix )

matrix matrix.makeidentity()
matrix matrix.makeperspectivefovlh( w, h, zn, zf )

debug.write( data )
debug.writeline( data )

--]]

--[[
resources.add( "geometry", "cube", "media/ShapeCube.xml" )
resources.add( "geometry", "pointfield", "media/ShapePointField.xml" )
resources.add( "geometry", "pointring", "media/ShapePointRing.xml" )
resources.add( "geometry", "dashring", "media/ShapeDashRing.xml" )
resources.add( "geometry", "pyramid", "media/ShapePyramid.xml" )
resources.add( "geometry", "circle", "media/ShapeCircle.xml" )
resources.add( "geometry", "sphere", "media/ShapeSphere.xml" )
resources.add( "geometry", "cylinder", "media/ShapeCylinder.xml" )
resources.add( "geometry", "tube", "media/ShapeTube.xml" )
resources.add( "geometry", "plane", "media/ShapePlane.xml" )
resources.add( "geometry", "cone", "media/ShapeCone.xml" )


scenes.add( "scene1" )

scene.addobject( "scene1", "cube" )
object.setgeometry( "scene1:cube", "cube" )
object.setposition( "scene1:cube", -4.5, 3, 0 )
--]]

scene.addobject( "scene1", "pointfield" )
object.setgeometry( "scene1:pointfield", "pointfield" )
object.setposition( "scene1:pointfield", -1.5, 3, 0 )
    
scene.addobject( "scene1", "pointring" )
object.setgeometry( "scene1:pointring", "pointring" )
object.setposition( "scene1:pointring", 1.5, 3, 0 )

scene.addobject( "scene1", "dashring" )
object.setgeometry( "scene1:dashring", "dashring" )
object.setposition( "scene1:dashring", 4.5, 3, 0 )

scene.addobject( "scene1", "pyramid" )
object.setgeometry( "scene1:pyramid", "pyramid" )
object.setposition( "scene1:pyramid", -4.5, 0, 0 )
    
scene.addobject( "scene1", "circle" )
object.setgeometry( "scene1:circle", "circle" )
object.setposition( "scene1:circle", -1.5, 0, 0 )

scene.addobject( "scene1", "sphere" )
object.setgeometry( "scene1:sphere", "sphere" )
object.setposition( "scene1:sphere", 1.5, 0, 0 )
   
scene.addobject( "scene1", "cylinder" )
object.setgeometry( "scene1:cylinder", "cylinder" )
object.setposition( "scene1:cylinder", 4.5, 0, 0 )

scene.addobject( "scene1", "tube" )
object.setgeometry( "scene1:tube", "tube" )
object.setposition( "scene1:tube", -4.5, -3, 0 )
    
scene.addobject( "scene1", "plane" )
object.setgeometry( "scene1:plane", "plane" )
object.setposition( "scene1:plane", -1.5, -3, 0 )
    
scene.addobject( "scene1", "cone" )
object.setgeometry( "scene1:cone", "cone" )
object.setposition( "scene1:cone", 1.5, -3, 0 )

scene.addobject( "scene1", "camera" )
object.setposition( "scene1:camera", 0, 5, -17 )
object.lookat( "scene1:camera", 0, 0, 0 ) 

scene.setcamera( "scene1", "camera" )

scene.setsize( "scene1", game.getwidth(), game.getheight() )

proj = matrix.makeperspectivefovlh( math.pi / 4.0, game.getaspectratiohw(), 1, 1000 )

camera.setprojection( "scene1", proj ) 
