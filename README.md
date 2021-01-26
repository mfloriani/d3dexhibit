# 3D Gallery

It is a virtual gallery developed using DirectX 11.  
The scene is composed of a room, a plinth in the centre with an exhibit on it. 
When inside the room the walls are opaque when outside the walls are transparent.  

### Tools

- DirectX 11 SDK
- Visual Studio 2019
- DirectXMath
- Assimp (<http://www.assimp.org/>)

### Shading

There are two shading techniques:

1. Gouraud shading
2. Phong shading (Bump mapping on the walls)

Press F4 to switch between them.

### Exhibit

There are three different objects to display (only a single exhibit is displayed at any one time):

1. Sphere
2. Dragon ( <http://graphics.stanford.edu/pub/3Dscanrep/dragon/dragon_recon.tar.gz> )
3. Asian dragon ( <http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_dragon.ply.gz> )

The normals for the Dragon models are generated using Assimp.

>IMPORTANT: download the two Dragon models from the link above and move them into the Gallery/Assets folder.

Press F3 to switch between them.

### Lighting

The room have the following lighting options:

1. Ambient + single local light
2. Ambient + single local light rotating around the top of the room
3. Ambient + four spot lights (one in each of the top corners of the room)

Press F5 to switch between them.

### Shadows

The exhibit and plinth generate shadow effects.  
Each object has a single shadow when illuminated by a single directional light, or four shadows when illuminated by the four spotlights.

1. Static shadow object on floor only (directional light or spotlights)
2. Basic shadow mapping (directional light only)

Press F6 to switch between them.

### Dust

GPU Particles of dust are to fall from the above the exhibit.

Press F8 to toggle on/off the effect.

### Cameras

The following cameras are adjustable via keyboard controls

- Camera1: Initially looking at the front of the room.
- Camera2: Initially looking at the top of the room from overhead.

Press F1 or F2 to switch between them.

### Controls

- ‘ESC’ exits the application
- F1 to F2 will select Camera1 and Camera2, respectively, as explained above
- F3 switches between exhibits
- F4 switches between exhibit render techniques
- F5 switches between lighting options
- F6 switches between shadow types
- F7 switches between scene level rendering modes (advanced feature, optional)
- F8 start/stop the dust falling
- Cameras are controlled by the cursor keys:
  - LEFT, RIGHT, UP, DOWN rotate left/right/up/down, respectively
  - CTRL + LEFT, RIGHT, UP, DOWN, PAGE UP, PAGE DOWN move to left/right/forward/backward/up/down, respectively
  - Alternatively, W, A, S, D or I, J, K, L keys may be used instead of the “cursor” keys
- T / SHIFT + T decrease/increase a factor that globally slows/speeds-up time-dependent effects
- R resets the scene

### Configuration File

Scene elements, lights, and camera are read from a configuration file.
