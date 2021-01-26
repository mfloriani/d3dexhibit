# 3D Exhibit

The aim is to create a virtual gallery

## Implementation

- DirectX 11 SDK
- DirectMath
- AssImp (<http://www.assimp.org/>)

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

The following exhibits are displayed (only a single exhibit is displayed at any one time)

1. Sphere
2. Dragon ( <http://graphics.stanford.edu/pub/3Dscanrep/dragon/dragon_recon.tar.gz> )
3. Asian dragon ( <http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_dragon.ply.gz> )

The normals for the Dragons are generated using Assimp.

>IMPORTANT: download the two Dragon models from the link above and move them into the Gallery/Assets folder.

### Lighting

The room have the following lighting options

1. Ambient + single local light
2. Ambient + single local light rotating around the top of the room
3. Ambient + four spot lights (one in each of the top corners of the room)

### Shadows

The exhibit and plinth generate the shadow effects.
Each object have a single shadow when illuminated by a single directional light, or four shadows when illuminated by the four spot lights.

1. Static shadow object on floor only
2. Basic shadow mapping

### Dust

Particles of dust are to fall from the above the exhibit

### Cameras

The following cameras are adjustable via keyboard controls

- Camera1: Initially looking at the front of the room.
- Camera2: Initially looking at the top of the room from overhead.

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
  - ‘left’/‘right’/’up’/’down’ rotate left/right/up/down, respectively
  - CTRL + ‘left’/‘right’/’up’/’down’/’page up’/’page down’ panning to left/right/forward/backward/up/down, respectively
  - Alternatively, w-a-s-d or i-j-k-l keys may be used instead of the “cursor” keys
- 't'/'T' decrease/increase a factor that globally slows/speeds-up time-dependent effects
- ‘r’ resets the scene

### Configuration File

Scene elements as well as lights and camera configurations are read from a configuration file.

### Shading

- Bump mapping of geometry

