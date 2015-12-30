static unsigned char NUM_POSES = 9;

typedef struct {
	char shortname[10];
	char fullname[25];
	char description[190];
} Pose;

Pose poses[] = {
	{"tadasana", "Mountain Pose", "Place your feet hip-width apart, toes forward, and evenly balance on the four corners of your feet. Tuck stomach, straighten spine, and drop shoulders.\0"},
	{"forbend", "Forward Bend", "Bend forward at the hips, keeping your shoulders down and your back straight. Place your hands by or on your feet.\0"},
	{"downdog", "Downward Dog", "From your hands and knees, place hands in front of shoulders and knees below hips. Push up, creating a straight line from hands to shoulders.\0"},
	// High plank
	{"sphinx", "Sphinx", "Lay flat with your hands flat on the floor. With your elbows under your shoulders, lift your torso, forming a 90 degree angle with your elbows. \0"},
	{"fullcobra", "Full Cobra", "Lay flat with your hands flat on the floor in front of you. Press your torso upwards until your arms are straight.\0"},
	{"lowlunge", "Low Lunge", "With both hands on the ground in front of you, step a foot between your hands with your knee over your heel. Slide the other foot backwards and raise your arms.\0"},
	{"war1", "Warrior I", "Spread feet apart with one facing forward and the other at 60 degrees. Bend forward knee over your heel. Raise arms upwards and face forwards.\0"},
	{"ward", "Dancing Warrior", "Place your feet arm-distance apart and bend your front knee while keeping your back leg straight. Place hand on back leg behind the knee and lean back.\0"},
	{"war2", "Warrior II", "Place your feet arm-distance apart and bend your front knee while keeping your back leg straight. Spread your arms horizontally. Look forward while keeping hips to side.\0"}
};

//Pose workout[] = {poses[0], poses[1], poses[2], poses[3], poses[4], poses[2],}