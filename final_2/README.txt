Liam Kolber and Eric Speaker
CSCI 4229
Final: Spidditch ---- "ITS QUIDDITCH IN SPACE!"
Due: December 12, 2017

How to run the program:
	When in the directory that holds all the code related
	files use the make command and then run ./final to open 
	the scene.
	Once the scene is open these are the various keys that can	
	be used at various times to move objects or alter and 
	change the scene and viewpoints

-----------------------------------------------------------------
|   KEYS 	  	|	        Description							|
-----------------------------------------------------------------
|  Left/Right 	|	Rotate view around x-axis					|
-----------------------------------------------------------------
|   Up/Down 	|	Rotate view around y-axis					|
-----------------------------------------------------------------
|    m/M     	| 	Toggle between planets/lighting hue			|
-----------------------------------------------------------------
|    f/F 	  	|	Toggle first person view/Gameplay			|
-----------------------------------------------------------------
|    w/W 	  	|	Forward										|
-----------------------------------------------------------------
|    s/S 	  	|	Backward									|
-----------------------------------------------------------------
|    a/A 	  	|	Look left									|
-----------------------------------------------------------------
|    d/D 	  	| 	Look right									|
-----------------------------------------------------------------
|    r/R     	| 	Up											|
-----------------------------------------------------------------
|    v/V     	| 	Down										|
-----------------------------------------------------------------
|    SPACE    	| 	Look Up 									|
-----------------------------------------------------------------
|    x/X     	|	Look Down									|
-----------------------------------------------------------------

The important files for our code are the final.c and the textures folder. 
The final.c holds all of the code that we created to generate the scene 
and game. The textures file holds all the textures used to texture various 
objects within the scene and games.

As for things we did within the scene/game itself, these are the various 
			aspects that you will see: 
	--The Stadium/Field:
		-- The stadium is created out of various cylinders,cubes & lines.
			These were used to represent a quidditch pitch from the
			Harry Potter series.
		-- The field is textured to look metalic
		-- The goal hoops are created out of cylinders and toruses
		-- The walls and cylinders attached to the walls are textured
			with a basic texture pattern to give some similarity
			to the walls of the Harry Potter Stadium
		-- The game functionality occurs within the stadium
		-- You will also see various representations of tie fighters
			and x-wings from Star Wars flying around the stadium
		-- There is also a representation of a golden snitch flying
			around the field. This is done with an object designed
			too look like the milenium falcon and colored gold
	--The Background:
		-- The background/"sky box" is textured with stars and an 
			image of a death star and is visable when the user
			opens the program
	--Scene outside the Field:
		-- The scene outside the field holds various objects and
			most are sent on an arbitrary path to just fly
			around the scene
			--the objects:
				--The giant planet that can be seen when the
					user opens the scene is an object
					and is not attached to the "sky box"
					This planet can also have its texture/color
					changed by pressing 'm'/'M' key			
				--There is a representation of a TARDIS from
					doctor who which is created through
					rectangles and textured. This object
					is flying around the scene and spinning 
				--There are two objects that represent two different
					space ships from star wars in the far background
				--There are various "fleets" containing a few tie fighters
					and x-wings that are also flying around the scene
					on created/predetermined paths. The tie fighters 
					and x-wings are also textured to add a little 
					flair to their design
	--Some Textures:
		-- The base of the field is textured to look metalic 
		-- The walls and 'stands' of the field are textured in a checkered pattern
		-- The tie fighters and x-wings are textured to fit representation of 
			those objects
		-- The giant plannet is textured and that texture can be changed through
			a few textures
		-- The spaceships in the far distance are colored to stand out a little more
	--Game Play:
		-- Game play is accessed through the button: 'f'/'F' which changes the camera
			view to first person from a fighter above the field 
		-- The user can then fly around the field with this fighter and attempt to 
			score by throwing a ball through one of the hoops
		-- The user throws a ball using the left mouse button
		-- If the user scores a goal -- meaning the ball goes through a hoop then 
			a message 'goal' is displayed and lasers will appears above 
			the hoops
		-- If the user does not score a goal/misses the hoop the ball will dissapear
			once it passed a designated axis line, the axis depending on where 
			the shot goes, and the user can shoot again



Who did what:
--------------------------------------------------------------------
|	Eric Speaker				|		Liam Kolber        		   |
--------------------------------------------------------------------
|  creation of most objects     |   created the xwing, tie fighter |
|      and basic shapes			|    and millenium falcon objects  |
--------------------------------------------------------------------
| was responsible for field and |  was responsible for flying and  | 
|	field design				|   first person view from objects |
--------------------------------------------------------------------
| came up with conceptual idea  | came up with conceptual idea for |
| for game play functionality	|     game play functionality 	   |
--------------------------------------------------------------------
|  built shapes used within     | took shapes and created lasers   |
|   parts of game play 			|  above the hoops on goals        |
--------------------------------------------------------------------
| helped with conceptual design |  	built the hud		   		   |
|	off the hud					|				  				   |
--------------------------------------------------------------------
| designed the flying path of	| designed the flying path of	   |
|   object around the scene		|   objects around the scene	   |
--------------------------------------------------------------------