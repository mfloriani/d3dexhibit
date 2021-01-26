# 700120-Lab-Final

## Concept

The aim is to create a virtual gallery

Dragon Statuette (<http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/dragon.jpg> )

## Mandatory Features

These features, when perfectly implemented, will provide 45% of the final mark.  
In addition, these features are a guide to the necessary building blocks, as well as the expected functionality.

### Room

A box with the opaque walls when viewed from inside and transparent walls when viewed from outside.

### Plinth

A raised section in the middle of the room upon which sits the exhibit.

### Exhibit

The exhibit is placed on the plinth in the center of the room.  
The rendering technique for the exhibit is to be selected from the list of techniques below, in sequence.  
Starting with Gouraud shading and proceeding down the list as follows:

1. Gouraud shading
2. Phong shading
3. Bump mapping (advanced feature, optional)
4. Displacement mapping (advanced feature, optional)
5. Toon shading (advanced feature, optional)
6. Transparency (advanced feature, optional)
7. Environment mapping of the room (advanced feature, optional)
8. Glowing using a post-processing technique (advanced feature, optional)

The following exhibits are to be displayed (only a single exhibit is displayed at any one time)

1. Sphere
2. Dragon ( <http://graphics.stanford.edu/pub/3Dscanrep/dragon/dragon_recon.tar.gz> )
3. Asian dragon ( <http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_dragon.ply.gz> ) (advanced feature, optional)

The dragon has 4 levels of detail from 11k to 870k polygons, whereas the Asian dragon consists of 7M polygons.  
The model will need to be scaled to fit the room.  
In addition neither model has normal, so these will need to be generated within your program.

### Lighting

The room should have the following lighting options

1. Ambient + single local light
2. Ambient + single local light rotating around the top of the room
3. Ambient + four spot lights (one in each of the top corners of the room)

### Shadows

The exhibit and plinth should exhibit the following shadow effects.  Note that the shadows will change based on the current lighting mode.  
Each object will have a single shadow when illuminated by a single directional light, or four shadows when illuminated by the four spot lights.

1. Static shadow object on floor only (not required to work with dynamic lights)
2. Basic shadow mapping (advanced feature, optional)
3. Advanced shadow mapping (advanced feature, optional)

### Dust

Particles of dust are to fall from the above the exhibit and land on any near horizontal surfaces
The dust must exhibit the following properties:

- Be subject to gravity and air resistance.  Air resistance can be approximated
- Collide with each of the objects, coming to rest on any near horizontal surfaces

### Cameras

The following cameras are to be adjustable via keyboard controls (see below)

- Camera1: Initially looking at the front of the room.
- Camera2: Initially looking at the top of the room from overhead.

### Controls

Note: the following key assignments are non-negotiable; they are expected to work in each submission to ease the demonstration process:

- ‘ESC’ exits the application
- F1 to F2 will select Camera1 and Camera2, respectively, as explained above
- Cameras are controlled by the cursor keys:
- ‘left’/‘right’/’up’/’down’ rotate left/right/up/down, respectively
- CTRL + ‘left’/‘right’/’up’/’down’/’page up’/’page down’ panning to left/right/forward/backward/up/down, respectively
- Alternatively, w-a-s-d or i-j-k-l keys may be used instead of the “cursor” keys
- F3 switches between exhibits
- F4 switches between exhibit render techniques
- F5 switches between lighting options
- F6 switches between shadow types
- F7 switches between scene level rendering modes (advanced feature, optional)
- F8 start/stop the dust falling
- 't'/'T' decrease/increase a factor that globally slows/speeds-up time-dependent effects
- ‘r’ resets the scene

### Configuration File

Scene elements as well as lights and camera configurations are read from a configuration file.

## Optional / Advanced Features

Each advanced feature has an associated difficultly point. The maximum mark available for each feature is this difficulty point. Only 30 marks are available for implementing advanced features. 
These 30 marks can be gained either by perfectly implementing features worth 30 points, or by partially implementing features worth up to 40 points. Any features implemented beyond 40 points will not be marked. 
Marks for advanced features will be capped at 30.

### Shading

- [4 points] bump mapping of geometry
- [2 points*] displacement mapping of geometry [*builds on bump mapping]
- [4 points] toon shading of geometry (e.g., toon/cel shading)
- [4 points] transparent geometry
- [4 points] environment cube mapping on all geometry

### Advanced Shadows

- [8 points] Shadow mapping
- [4 points*] Advanced shadow mapping (e.g., PCF, VSM) [*builds on shadow mapping]

### Scene level rendering modes

- [8 points] Deferred rendering, including the ability to switch the visualization for the various MRT buffers
- [6 points] Glowing effect
- [4 points] High-dynamic range rendering and tone-mapped post-processing

### Advanced Exhibit

- [4 points] Asian dragon ( <http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_dragon.ply.gz> )

## Hints

The following may help you to successfully complete your work:

- Start now!
- Produce a paper visualization (e.g., story-board sketches) of your project and check it with a member of the module team
- Produce a top-level software design and check it with a member of the module team.
- Prototype your ideas to help produce a more detailed design
- Test your software at each stage of development
- Document as you go

## Implementation

The final implementation must be able to run on a standard PC in the post-graduate graphics lab, i.e. FEN-177. Only the following libraries and APIs are permitted:

- DirectX 11 SDK
- DirectXTk (framework only)
- GLM or DirectMath
- AssImp (<http://www.assimp.org/>)

The use of any language feature within C++ 11, 14 or 17 is permitted (please remember the final product needs to run on a standard PC in FEN-177).

The use of Boost Libraries, DirectXTk (shaders) or WinRT are not permitted.

Prior approval is required for any other APIs and features, not specifically mentioned above.

## Lab Book

You are required to add the following two entries to your Lab Book:

### Design – [word limit 1000]

- Class diagram(s) containing main classes
- Class diagram(s) containing service/utility classes
- A textual description giving the name, role and responsibilities of each class
- Interaction diagram(s) for significant components of the software design
- A critique of the design, including details on:
- The merits of the design?
- Weaknesses of the design?
- What has changed in the design?
- What would you do in a different way?

### Graphics – [word limit 1000]

- Document and critique the algorithms used, including:
- Geometry representation and processing
- Shading and lighting
- Shadow generation
- Particle system
- Explain how application objects and their graphics representation are connected; discuss the advantages/disadvantages of your approach
- Explain how application-object behavior is updated and how these updates are propagated to the graphics representation
- Discuss potential extensions (especially with respect to non-implemented items from the advanced feature list) as well as potential scalability issues
- What feature (not necessarily mentioned before) would you have liked to add?

## Video

A 3 to 5 minute video with audio commentary.  The video should highlight each of the key elements of your implementation.  
It is strongly suggested that you work through the list of mandatory and optional features within the specification, showing each in turn.

## Demonstration

You will be required to both demonstrate your solution, and take part in a code review (RESIT only).  
The code review will consist of your examiners selecting parts of your code and requiring you to explain the implementation and the rationale behind the design.
