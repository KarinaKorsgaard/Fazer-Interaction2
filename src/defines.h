
//  Created by Karina Jensen on 08/06/16.
//
//


#define RES_W 3289
#define RES_H 720

// number of particles
#define SWARM_NUM 720
#define NUM_SOUND_PARTICLES 8 //time 3, 7*3 = 21 particles. Goes up to 13-> 13*3=39
// number of animals are defined by the amount of folders in "data/animals"


// sizes of particles
#define PARTICLES_MIN_RADIUS 10
#define  PARTICLES_MAX_RADIUS 50

#define SOUND_PARTICLES_MIN_RADIUS 30
#define  SOUND_PARTICLES_MAX_RADIUS 55

#define ANIMAL_PARTICLES_MIN_RADIUS 75
#define  ANIMAL_PARTICLES_MAX_RADIUS 85
#define  ANIMAL_CHANCE 1000 // 1/n chance pr frame (40fps) of showing up again after being hit

#define SOUND_ANIMATION 4 // inout time in frames
#define ANIMAL_ANIMATION 4 // sinus inout curve animating size
#define ANIMAL_FPS 24

// particle velocity : velocity can still get higher, it will just slow down high velocity particles a bit
#define MIN_VEL 0.5
#define MAX_VEL 10

#define CENTER_ATTRACTION 2 // attraction force on particles when outside of screen
